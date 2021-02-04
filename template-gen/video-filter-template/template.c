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
 * SECTION:element-plugin
 *
 * FIXME:Describe plugin here.
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

#include "gst{{FILE_NAME}}.h"

#define UNUSE(variable) if(variable)

GST_DEBUG_CATEGORY_STATIC (gst_{{LOWERCASE_CLASS_NAME}}_debug);
#define GST_CAT_DEFAULT gst_{{LOWERCASE_CLASS_NAME}}_debug

/* Filter signals and args */
enum
{
  LAST_SIGNAL
};

enum
{
  PROP_0
};

#define VIDEO_SRC_CAPS \
    GST_VIDEO_CAPS_MAKE("{ I420, Y444, Y42B, UYVY, RGBA }")

#define VIDEO_SINK_CAPS \
    GST_VIDEO_CAPS_MAKE("{ I420, Y444, Y42B, UYVY, RGBA }")

#define gst_{{LOWERCASE_CLASS_NAME}}_parent_class parent_class
G_DEFINE_TYPE ({{CAMEL_CLASS_NAME}}, gst_{{LOWERCASE_CLASS_NAME}}, GST_TYPE_VIDEO_FILTER);

static void gst_{{LOWERCASE_CLASS_NAME}}_set_property (GObject * object,
    guint prop_id, const GValue * value, GParamSpec * pspec);
static void gst_{{LOWERCASE_CLASS_NAME}}_get_property (GObject * object,
    guint prop_id, GValue * value, GParamSpec * pspec);


static gboolean gst_{{LOWERCASE_CLASS_NAME}}_start (GstBaseTransform * trans);
static gboolean gst_{{LOWERCASE_CLASS_NAME}}_stop (GstBaseTransform * trans);
static gboolean gst_{{LOWERCASE_CLASS_NAME}}_set_info (GstVideoFilter * filter, GstCaps * incaps,
    GstVideoInfo * in_info, GstCaps * outcaps, GstVideoInfo * out_info);
static GstFlowReturn gst_{{LOWERCASE_CLASS_NAME}}_transform_frame (GstVideoFilter * filter,
    GstVideoFrame * inframe, GstVideoFrame * outframe);
static GstFlowReturn gst_{{LOWERCASE_CLASS_NAME}}_transform_frame_ip (GstVideoFilter * filter,
    GstVideoFrame * frame);

/* initialize the plugin's class */
static void
gst_{{LOWERCASE_CLASS_NAME}}_class_init ({{CAMEL_CLASS_NAME}}Class * klass)
{
  GObjectClass *gobject_class;
  GstBaseTransformClass *base_transform_class;
  GstVideoFilterClass *video_filter_class;

  gobject_class = G_OBJECT_CLASS(klass);
  base_transform_class = GST_BASE_TRANSFORM_CLASS (klass);
  video_filter_class = GST_VIDEO_FILTER_CLASS (klass);

  gobject_class->set_property = gst_{{LOWERCASE_CLASS_NAME}}_set_property;
  gobject_class->get_property = gst_{{LOWERCASE_CLASS_NAME}}_get_property;
  base_transform_class->start = GST_DEBUG_FUNCPTR (gst_{{LOWERCASE_CLASS_NAME}}_start);
  base_transform_class->stop = GST_DEBUG_FUNCPTR (gst_{{LOWERCASE_CLASS_NAME}}_stop);
  video_filter_class->set_info = GST_DEBUG_FUNCPTR (gst_{{LOWERCASE_CLASS_NAME}}_set_info);
  video_filter_class->transform_frame = GST_DEBUG_FUNCPTR (gst_{{LOWERCASE_CLASS_NAME}}_transform_frame);
  video_filter_class->transform_frame_ip = GST_DEBUG_FUNCPTR (gst_{{LOWERCASE_CLASS_NAME}}_transform_frame_ip);

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
}

/* initialize the new element
 * instantiate pads and add them to element
 * set pad calback functions
 * initialize instance structure
 */
static void
gst_{{LOWERCASE_CLASS_NAME}}_init ({{CAMEL_CLASS_NAME}} * filter)
{
}

static void
gst_{{LOWERCASE_CLASS_NAME}}_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  {{CAMEL_CLASS_NAME}} *filter = GST_{{UPPERCASE_CLASS_NAME}} (object);

  UNUSE(filter);

  switch (prop_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_{{LOWERCASE_CLASS_NAME}}_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
  {{CAMEL_CLASS_NAME}} *filter = GST_{{UPPERCASE_CLASS_NAME}} (object);

  UNUSE(filter);

  switch (prop_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static gboolean
gst_{{LOWERCASE_CLASS_NAME}}_start (GstBaseTransform * trans)
{
  {{CAMEL_CLASS_NAME}} *videofilter = GST_{{UPPERCASE_CLASS_NAME}} (trans);

  GST_DEBUG_OBJECT (videofilter, "start");

  return TRUE;
}

static gboolean
gst_{{LOWERCASE_CLASS_NAME}}_stop (GstBaseTransform * trans)
{
  {{CAMEL_CLASS_NAME}} *videofilter = GST_{{UPPERCASE_CLASS_NAME}} (trans);

  GST_DEBUG_OBJECT (videofilter, "stop");

  return TRUE;
}

static gboolean
gst_{{LOWERCASE_CLASS_NAME}}_set_info (GstVideoFilter * filter, GstCaps * incaps,
    GstVideoInfo * in_info, GstCaps * outcaps, GstVideoInfo * out_info)
{
  {{CAMEL_CLASS_NAME}} *videofilter = GST_{{UPPERCASE_CLASS_NAME}} (filter);

  UNUSE(videofilter);

  GST_DEBUG_OBJECT (videofilter, "set_info");

  return TRUE;
}

/* transform */
static GstFlowReturn
gst_{{LOWERCASE_CLASS_NAME}}_transform_frame (GstVideoFilter * filter, GstVideoFrame * inframe,
    GstVideoFrame * outframe)
{
  {{CAMEL_CLASS_NAME}} *videofilter = GST_{{UPPERCASE_CLASS_NAME}} (filter);

  UNUSE(videofilter);

  gst_video_frame_copy(outframe, inframe);

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_{{LOWERCASE_CLASS_NAME}}_transform_frame_ip (GstVideoFilter * filter, GstVideoFrame * frame)
{
  {{CAMEL_CLASS_NAME}} *videofilter = GST_{{UPPERCASE_CLASS_NAME}} (filter);

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
  /* debug category for filtering log messages
   *
   * exchange the string 'Template plugin' with your description
   */
  GST_DEBUG_CATEGORY_INIT (gst_{{LOWERCASE_CLASS_NAME}}_debug, "plugin",
      0, "Template plugin");

  return gst_element_register (plugin, "{{PLUGIN_NAME}}", GST_RANK_NONE, GST_TYPE_{{UPPERCASE_CLASS_NAME}});
}

/* gstreamer looks for this structure to register plugins
 *
 * exchange the string 'Template plugin' with your plugin description
 */
GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    {{FILE_NAME}},
    "Template plugin",
    plugin_init,
    PACKAGE_VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN)
