project('gst-my-video-flip', 'c', version : '0.0.1', license : 'LGPL')

plugins_install_dir = join_paths(get_option('libdir'), 'gstreamer-1.0')

cc = meson.get_compiler('c')

gst_version = meson.project_version()

api_version = '1.0'

gst_dep = dependency('gstreamer-1.0',
    fallback : ['gstreamer', 'gst_dep'])

gstbase_dep = dependency('gstreamer-base-1.0',
  fallback : ['gstreamer', 'gst_base_dep'])

gstvideo_dep = dependency('gstreamer-video-1.0',
    fallback : ['gst-plugins-base', 'video_dep'])

plugin_c_args = ['-DHAVE_CONFIG_H']

cdata = configuration_data()
cdata.set_quoted('VERSION', gst_version)
cdata.set_quoted('PACKAGE_NAME', 'my_video_flip')
cdata.set_quoted('PACKAGE_VERSION', gst_version)
cdata.set_quoted('PACKAGE', 'gst-my-video-flip-plugin')
cdata.set_quoted('GST_LICENSE', 'LGPL')
cdata.set_quoted('GST_API_VERSION', api_version)
cdata.set_quoted('GST_PACKAGE_NAME', 'my_video_flip')
cdata.set_quoted('GST_PACKAGE_ORIGIN', 'https://github.com/albert-hu/gstreamer-example/gst-my-video-flip')
configure_file(output : 'config.h', configuration : cdata)

# Plugin 1
plugin_sources = [
  'gstmyvideoflip.c'
  ]

gstpluginexample = library('gstmyvideoflip',
  plugin_sources,
  c_args: plugin_c_args,
  dependencies : [gst_dep, gstbase_dep, gstvideo_dep],
  install : true,
  install_dir : plugins_install_dir,
)
