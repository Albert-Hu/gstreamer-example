#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/video/gstvideofilter.h>

#include "gstsimpleisp.h"

#include <opencv2/opencv.hpp>
using namespace cv;

void add_gaussian_noise(GstVideoFrame * inframe, GstVideoFrame * outframe)
{
  GstMapInfo inmap, outmap;
  gst_buffer_map(inframe->buffer, &inmap, GST_MAP_READ);
  gst_buffer_map(outframe->buffer, &outmap, GST_MAP_READ);
  Mat input(
    Size(GST_VIDEO_FRAME_WIDTH(inframe), GST_VIDEO_FRAME_HEIGHT(inframe)),
    CV_8UC3,
    inmap.data);
  Mat output(
    Size(GST_VIDEO_FRAME_WIDTH(outframe), GST_VIDEO_FRAME_HEIGHT(outframe)),
    CV_8UC3,
    outmap.data);
  Mat clone(
    Size(GST_VIDEO_FRAME_WIDTH(outframe), GST_VIDEO_FRAME_HEIGHT(outframe)),
    CV_32FC3);
  Mat noise(
    Size(GST_VIDEO_FRAME_WIDTH(outframe), GST_VIDEO_FRAME_HEIGHT(outframe)),
    CV_32FC3);

  randn(noise, 0., 1.);

  input.convertTo(clone, CV_32FC3);
  clone = clone / 255.;
  clone = clone + noise;

  normalize(clone, noise, 0., 255., NORM_MINMAX);

  noise.convertTo(output, CV_8UC3);

  gst_buffer_unmap(inframe->buffer, &inmap);
  gst_buffer_unmap(outframe->buffer, &outmap);
}
