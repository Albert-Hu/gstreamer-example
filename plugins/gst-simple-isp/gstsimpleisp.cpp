/*
 * GStreamer
 * Copyright (C) 2005 Thomas Vander Stichele <thomas@apestaart.org>
 * Copyright (C) 2005 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
 * Copyright (C) YEAR AUTHOR_NAME AUTHOR_EMAIL
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * SECTION: simple-isp-plugin
 *
 * This is a simple ISP(Image Sensor Processing) plugin.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch -v -m fakesrc ! plugin ! fakesink
 * ]|
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/video/gstvideofilter.h>

#include "gstsimpleisp.h"

#include <opencv2/opencv.hpp>
using namespace cv;

#define UNUSE(variable) if(variable)

GST_DEBUG_CATEGORY_STATIC (gst_simple_isp_debug);
#define GST_CAT_DEFAULT gst_simple_isp_debug

/* Filter signals and args */
enum
{
  LAST_SIGNAL
};

enum
{
  PROP_0,
  GAUSSIAN_NOISE
};

#define VIDEO_SRC_CAPS \
    GST_VIDEO_CAPS_MAKE("{ BGR }")

#define VIDEO_SINK_CAPS \
    GST_VIDEO_CAPS_MAKE("{ BGR }")

#define gst_simple_isp_parent_class parent_class
G_DEFINE_TYPE (GstSimpleIsp, gst_simple_isp, GST_TYPE_VIDEO_FILTER);

static void gst_simple_isp_set_property (GObject * object,
    guint prop_id, const GValue * value, GParamSpec * pspec);
static void gst_simple_isp_get_property (GObject * object,
    guint prop_id, GValue * value, GParamSpec * pspec);


static gboolean gst_simple_isp_start (GstBaseTransform * trans);
static gboolean gst_simple_isp_stop (GstBaseTransform * trans);
static gboolean gst_simple_isp_set_info (GstVideoFilter * filter, GstCaps * incaps,
    GstVideoInfo * in_info, GstCaps * outcaps, GstVideoInfo * out_info);
static GstFlowReturn gst_simple_isp_transform_frame (GstVideoFilter * filter,
    GstVideoFrame * inframe, GstVideoFrame * outframe);
static GstFlowReturn gst_simple_isp_transform_frame_ip (GstVideoFilter * filter,
    GstVideoFrame * frame);

/* initialize the plugin's class */
static void
gst_simple_isp_class_init (GstSimpleIspClass * klass)
{
  GObjectClass *gobject_class;
  GstBaseTransformClass *base_transform_class;
  GstVideoFilterClass *video_filter_class;

  gobject_class = G_OBJECT_CLASS(klass);
  base_transform_class = GST_BASE_TRANSFORM_CLASS (klass);
  video_filter_class = GST_VIDEO_FILTER_CLASS (klass);

  gobject_class->set_property = gst_simple_isp_set_property;
  gobject_class->get_property = gst_simple_isp_get_property;
  base_transform_class->start = GST_DEBUG_FUNCPTR (gst_simple_isp_start);
  base_transform_class->stop = GST_DEBUG_FUNCPTR (gst_simple_isp_stop);
  video_filter_class->set_info = GST_DEBUG_FUNCPTR (gst_simple_isp_set_info);
  video_filter_class->transform_frame = GST_DEBUG_FUNCPTR (gst_simple_isp_transform_frame);
  video_filter_class->transform_frame_ip = GST_DEBUG_FUNCPTR (gst_simple_isp_transform_frame_ip);

  gst_element_class_set_details_simple (GST_ELEMENT_CLASS(klass),
      "Plugin",
      "Generic",
      "Generic Template Element", "AUTHOR_NAME AUTHOR_EMAIL");

  gst_element_class_add_pad_template (GST_ELEMENT_CLASS(klass),
      gst_pad_template_new ("src", GST_PAD_SRC, GST_PAD_ALWAYS,
        gst_caps_from_string (VIDEO_SRC_CAPS)));
  gst_element_class_add_pad_template (GST_ELEMENT_CLASS(klass),
      gst_pad_template_new ("sink", GST_PAD_SINK, GST_PAD_ALWAYS,
        gst_caps_from_string (VIDEO_SINK_CAPS)));

  g_object_class_install_property (gobject_class, GAUSSIAN_NOISE,
      g_param_spec_boolean ("gaussian-noise", "Gaussian Noise", "Add Gaussian noise to image.",
          FALSE, G_PARAM_READWRITE));
}

/* initialize the new element
 * instantiate pads and add them to element
 * set pad calback functions
 * initialize instance structure
 */
static void
gst_simple_isp_init (GstSimpleIsp * filter)
{
}

static void
gst_simple_isp_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstSimpleIsp *filter = GST_SIMPLE_ISP (object);

  UNUSE(filter);

  switch (prop_id) {
    case GAUSSIAN_NOISE:
      filter->gaussian_noise = g_value_get_boolean(value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_simple_isp_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
  GstSimpleIsp *filter = GST_SIMPLE_ISP (object);

  UNUSE(filter);

  switch (prop_id) {
    case GAUSSIAN_NOISE:
      g_value_set_boolean (value, filter->gaussian_noise);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static gboolean
gst_simple_isp_start (GstBaseTransform * trans)
{
  GstSimpleIsp *videofilter = GST_SIMPLE_ISP (trans);

  GST_DEBUG_OBJECT (videofilter, "start");

  return TRUE;
}

static gboolean
gst_simple_isp_stop (GstBaseTransform * trans)
{
  GstSimpleIsp *videofilter = GST_SIMPLE_ISP (trans);

  GST_DEBUG_OBJECT (videofilter, "stop");

  return TRUE;
}

static gboolean
gst_simple_isp_set_info (GstVideoFilter * filter, GstCaps * incaps,
    GstVideoInfo * in_info, GstCaps * outcaps, GstVideoInfo * out_info)
{
  GstSimpleIsp *videofilter = GST_SIMPLE_ISP (filter);

  UNUSE(videofilter);

  GST_DEBUG_OBJECT (videofilter, "set_info");

  return TRUE;
}

/* transform */
static GstFlowReturn
gst_simple_isp_transform_frame (GstVideoFilter * filter, GstVideoFrame * inframe,
    GstVideoFrame * outframe)
{
  GstSimpleIsp *videofilter = GST_SIMPLE_ISP (filter);

  if (videofilter->gaussian_noise) {
    add_gaussian_noise(inframe, outframe);
  } else {
    gst_video_frame_copy(outframe, inframe);
  }

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_simple_isp_transform_frame_ip (GstVideoFilter * filter, GstVideoFrame * frame)
{
  GstSimpleIsp *videofilter = GST_SIMPLE_ISP (filter);

  GST_DEBUG_OBJECT (videofilter, "transform_frame_ip");

  return GST_FLOW_OK;
}

/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean
plugin_init (GstPlugin * plugin)
{
  GST_DEBUG_CATEGORY_INIT (gst_simple_isp_debug, "plugin",
      0, "Simple ISP");

  return gst_element_register (plugin, "simpleisp", GST_RANK_NONE, GST_TYPE_SIMPLE_ISP);
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    simpleisp,
    "Simple ISP",
    plugin_init,
    PACKAGE_VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN)
