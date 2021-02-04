project('gst-simple-isp', 'c', version : '0.0.1', license : 'LGPL')

plugins_install_dir = join_paths(get_option('libdir'), 'gstreamer-1.0')

cc = meson.get_compiler('c')

gst_version = meson.project_version()
gst_license = meson.project_license()

gst_dep = dependency('gstreamer-1.0',
    fallback : ['gstreamer', 'gst_dep'])

gstbase_dep = dependency('gstreamer-base-1.0',
  fallback : ['gstreamer', 'gst_base_dep'])

gstvideo_dep = dependency('gstreamer-video-1.0',
    fallback : ['gst-plugins-base', 'video_dep'])

plugin_c_args = ['-DHAVE_CONFIG_H']

cdata = configuration_data()
cdata.set_quoted('VERSION', gst_version)
cdata.set_quoted('PACKAGE_NAME', 'simple_isp')
cdata.set_quoted('PACKAGE_VERSION', gst_version)
cdata.set_quoted('PACKAGE', 'gst-simple-isp')
cdata.set_quoted('GST_LICENSE', gst_license)
cdata.set_quoted('GST_API_VERSION', 1.0)
cdata.set_quoted('GST_PACKAGE_NAME', 'simple_isp')
cdata.set_quoted('GST_PACKAGE_ORIGIN', 'https://gstreamer.freedesktop.org/')
configure_file(output : 'config.h', configuration : cdata)

plugin_sources = [
  'simpleisp.c'
  ]

gstpluginexample = library('simpleisp',
  plugin_sources,
  c_args: plugin_c_args,
  dependencies : [gst_dep, gstbase_dep, gstvideo_dep],
  install : true,
  install_dir : plugins_install_dir,
)
