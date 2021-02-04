project('{{PACKAGE}}', 'c', version : '{{PACKAGE_VERSION}}', license : '{{GST_LICENSE}}')

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
cdata.set_quoted('PACKAGE_NAME', '{{PACKAGE_NAME}}')
cdata.set_quoted('PACKAGE_VERSION', gst_version)
cdata.set_quoted('PACKAGE', '{{PACKAGE}}')
cdata.set_quoted('GST_LICENSE', gst_license)
cdata.set_quoted('GST_API_VERSION', {{GST_API_VERSION}})
cdata.set_quoted('GST_PACKAGE_NAME', '{{PACKAGE_NAME}}')
cdata.set_quoted('GST_PACKAGE_ORIGIN', '{{GST_PACKAGE_ORIGIN}}')
configure_file(output : 'config.h', configuration : cdata)

plugin_sources = [
  '{{FILE_NAME}}.c'
  ]

gstpluginexample = library('{{FILE_NAME}}',
  plugin_sources,
  c_args: plugin_c_args,
  dependencies : [gst_dep, gstbase_dep, gstvideo_dep],
  install : true,
  install_dir : plugins_install_dir,
)
