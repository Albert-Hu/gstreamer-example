/* GStreamer
 * Copyright (C) 2021
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Suite 500,
 * Boston, MA 02110-1335, USA.
 */
/**
 * SECTION:element-gstmyvideoflip
 *
 * The is a simple plugin to flip the video to both vertical or horizontal.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v videotestsrc ! video/x-raw,width=480,height=360,format=RGBA ! myvideoflip ! videoconvert ! autovideosink
 * ]|
 * Flip the video to either vertical or horizontal and also can be just passed the video.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/video/gstvideofilter.h>
#include "gstmyvideoflip.h"

GST_DEBUG_CATEGORY_STATIC (gst_my_video_flip_debug_category);
#define GST_CAT_DEFAULT gst_my_video_flip_debug_category

/* prototypes */


static void gst_my_video_flip_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_my_video_flip_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_my_video_flip_dispose (GObject * object);
static void gst_my_video_flip_finalize (GObject * object);

static gboolean gst_my_video_flip_start (GstBaseTransform * trans);
static gboolean gst_my_video_flip_stop (GstBaseTransform * trans);
static gboolean gst_my_video_flip_set_info (GstVideoFilter * filter, GstCaps * incaps,
    GstVideoInfo * in_info, GstCaps * outcaps, GstVideoInfo * out_info);
static GstFlowReturn gst_my_video_flip_transform_frame (GstVideoFilter * filter,
    GstVideoFrame * inframe, GstVideoFrame * outframe);
static GstFlowReturn gst_my_video_flip_transform_frame_ip (GstVideoFilter * filter,
    GstVideoFrame * frame);

enum
{
  PROP_0,
  PROP_VERTICAL,
  PROP_HORIZONTAL
};

/* pad templates */

#define VIDEO_SRC_CAPS \
    GST_VIDEO_CAPS_MAKE("{ RGBA }")

#define VIDEO_SINK_CAPS \
    GST_VIDEO_CAPS_MAKE("{ RGBA }")


/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstMyVideoFlip, gst_my_video_flip, GST_TYPE_VIDEO_FILTER,
  GST_DEBUG_CATEGORY_INIT (gst_my_video_flip_debug_category, "myvideoflip", 0,
  "debug category for myvideoflip element"));

static void
gst_my_video_flip_class_init (GstMyVideoFlipClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstBaseTransformClass *base_transform_class = GST_BASE_TRANSFORM_CLASS (klass);
  GstVideoFilterClass *video_filter_class = GST_VIDEO_FILTER_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_pad_template (GST_ELEMENT_CLASS(klass),
      gst_pad_template_new ("src", GST_PAD_SRC, GST_PAD_ALWAYS,
        gst_caps_from_string (VIDEO_SRC_CAPS)));
  gst_element_class_add_pad_template (GST_ELEMENT_CLASS(klass),
      gst_pad_template_new ("sink", GST_PAD_SINK, GST_PAD_ALWAYS,
        gst_caps_from_string (VIDEO_SINK_CAPS)));

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS(klass),
      "A Simple Video Flip", "Generic", "Flip the video to both vertical or horizontal.",
      "No");

  gobject_class->set_property = gst_my_video_flip_set_property;
  gobject_class->get_property = gst_my_video_flip_get_property;
  gobject_class->dispose = gst_my_video_flip_dispose;
  gobject_class->finalize = gst_my_video_flip_finalize;
  base_transform_class->start = GST_DEBUG_FUNCPTR (gst_my_video_flip_start);
  base_transform_class->stop = GST_DEBUG_FUNCPTR (gst_my_video_flip_stop);
  video_filter_class->set_info = GST_DEBUG_FUNCPTR (gst_my_video_flip_set_info);
  video_filter_class->transform_frame = GST_DEBUG_FUNCPTR (gst_my_video_flip_transform_frame);
  video_filter_class->transform_frame_ip = GST_DEBUG_FUNCPTR (gst_my_video_flip_transform_frame_ip);

  g_object_class_install_property (gobject_class, PROP_VERTICAL,
      g_param_spec_boolean ("vertical", "Vertical", "Flip the vertical of the video.",
          FALSE, G_PARAM_READWRITE));

  g_object_class_install_property (gobject_class, PROP_HORIZONTAL,
      g_param_spec_boolean ("horizontal", "Horizontal", "Flip the horizontal of the video.",
          FALSE, G_PARAM_READWRITE));
}

static void
gst_my_video_flip_init (GstMyVideoFlip *myvideoflip)
{
}

void
gst_my_video_flip_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMyVideoFlip *myvideoflip = GST_MY_VIDEO_FLIP (object);

  GST_DEBUG_OBJECT (myvideoflip, "set_property");

  switch (property_id) {
    case PROP_VERTICAL:
      myvideoflip->flip_vertical = g_value_get_boolean(value);
      break;
    case PROP_HORIZONTAL:
      myvideoflip->flip_horizontal = g_value_get_boolean(value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_my_video_flip_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstMyVideoFlip *myvideoflip = GST_MY_VIDEO_FLIP (object);

  GST_DEBUG_OBJECT (myvideoflip, "get_property");

  switch (property_id) {
    case PROP_VERTICAL:
      g_value_set_boolean (value, myvideoflip->flip_vertical);
      break;
    case PROP_HORIZONTAL:
      g_value_set_boolean (value, myvideoflip->flip_horizontal);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_my_video_flip_dispose (GObject * object)
{
  GstMyVideoFlip *myvideoflip = GST_MY_VIDEO_FLIP (object);

  GST_DEBUG_OBJECT (myvideoflip, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_my_video_flip_parent_class)->dispose (object);
}

void
gst_my_video_flip_finalize (GObject * object)
{
  GstMyVideoFlip *myvideoflip = GST_MY_VIDEO_FLIP (object);

  GST_DEBUG_OBJECT (myvideoflip, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_my_video_flip_parent_class)->finalize (object);
}

static gboolean
gst_my_video_flip_start (GstBaseTransform * trans)
{
  GstMyVideoFlip *myvideoflip = GST_MY_VIDEO_FLIP (trans);

  GST_DEBUG_OBJECT (myvideoflip, "start");

  return TRUE;
}

static gboolean
gst_my_video_flip_stop (GstBaseTransform * trans)
{
  GstMyVideoFlip *myvideoflip = GST_MY_VIDEO_FLIP (trans);

  GST_DEBUG_OBJECT (myvideoflip, "stop");

  return TRUE;
}

static gboolean
gst_my_video_flip_set_info (GstVideoFilter * filter, GstCaps * incaps,
    GstVideoInfo * in_info, GstCaps * outcaps, GstVideoInfo * out_info)
{
  GstMyVideoFlip *myvideoflip = GST_MY_VIDEO_FLIP (filter);

  GST_DEBUG_OBJECT (myvideoflip, "set_info");

  return TRUE;
}

/* transform */
static GstFlowReturn
gst_my_video_flip_transform_frame (GstVideoFilter * filter, GstVideoFrame * inframe,
    GstVideoFrame * outframe)
{
  guint8 const *s;
  guint8 *d;
  gint x, y, z;
  gint stride;
  gint sw;
  gint sh;
  gint bpp;

  GstMyVideoFlip *myvideoflip = GST_MY_VIDEO_FLIP (filter);

  s = GST_VIDEO_FRAME_PLANE_DATA (inframe, 0);
  d = GST_VIDEO_FRAME_PLANE_DATA (outframe, 0);

  sw = GST_VIDEO_FRAME_WIDTH (inframe);
  sh = GST_VIDEO_FRAME_HEIGHT (inframe);

  bpp = GST_VIDEO_FRAME_COMP_PSTRIDE (inframe, 0);
  stride = GST_VIDEO_FRAME_PLANE_STRIDE (inframe, 0);

  GST_DEBUG_OBJECT (myvideoflip, "transform_frame");

  if (myvideoflip->flip_vertical) {
    if (myvideoflip->flip_horizontal) {
      for (y = 0; y < sh; y++) {
        for (x = 0; x < sw; x++) {
          for (z = 0; z < bpp; z++) {
            d[y * stride + x * bpp + z] = 
              s[(sh - y - 1) * stride + (sw - x - 1) * bpp + z];
          }
        }
      }
    } else {
      for (y = 0; y < sh; y++) {
        for (x = 0; x < sw; x++) {
          for (z = 0; z < bpp; z++) {
            d[y * stride + x * bpp + z] = 
              s[(sh - y - 1) * stride + x * bpp + z];
          }
        }
      }
    }
  } else {
    if (myvideoflip->flip_horizontal) {
      for (y = 0; y < sh; y++) {
        for (x = 0; x < sw; x++) {
          for (z = 0; z < bpp; z++) {
            d[y * stride + x * bpp + z] = 
              s[y * stride + (sw - x - 1) * bpp + z];
          }
        }
      }
    } else {
      gst_video_frame_copy(outframe, inframe);
    }
  }

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_my_video_flip_transform_frame_ip (GstVideoFilter * filter, GstVideoFrame * frame)
{
  GstMyVideoFlip *myvideoflip = GST_MY_VIDEO_FLIP (filter);

  GST_DEBUG_OBJECT (myvideoflip, "transform_frame_ip");

  return GST_FLOW_OK;
}

static gboolean
plugin_init (GstPlugin * plugin)
{
  return gst_element_register (plugin, "myvideoflip", GST_RANK_NONE,
      GST_TYPE_MY_VIDEO_FLIP);
}

#ifndef VERSION
#define VERSION "0.0.1"
#endif
#ifndef PACKAGE
#define PACKAGE "gst-my-video-flip-plugin"
#endif
#ifndef PACKAGE_NAME
#define PACKAGE_NAME "my_video_flip"
#endif
#ifndef GST_PACKAGE_ORIGIN
#define GST_PACKAGE_ORIGIN "https://github.com/albert-hu/gstreamer-example/gst-my-video-flip"
#endif

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    myvideoflip,
    "My video flipping plugin.",
    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)

