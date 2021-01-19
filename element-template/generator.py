from __future__ import print_function
from __future__ import division

import argparse, json, os

default_config = {
  'VERSION': '0.0.1',
  'PACKAGE_NAME': 'plugin_template',
  'PACKAGE_VERSION': '0.0.1',
  'PACKAGE': 'gst-template-plugin',
  'GST_LICENSE': 'LGPL',
  'GST_API_VERSION': '1.0',
  'GST_PACKAGE_NAME': 'plugin_template',
  'GST_PACKAGE_ORIGIN': 'https://gstreamer.freedesktop.org/',
  'FILE_NAME': 'template',
  'CLASS_NAME': 'plugin_template',
  'PLUGIN_NAME': 'myplugin'
}

def camel_string(s):
  return ''.join([_s.capitalize() for _s in s.split('_')])

def generate(config, out_dir):
  if os.path.isfile(config):
    with open(config, 'r') as cfg:
      js = json.load(cfg)
      for key in js.keys():
        if type(js[key]) is str:
          default_config[key] = js[key]
  default_config['UPPERCASE_CLASS_NAME'] = default_config['CLASS_NAME'].upper()
  default_config['LOWERCASE_CLASS_NAME'] = default_config['CLASS_NAME'].lower()
  default_config['CAMEL_CLASS_NAME'] = 'Gst' + camel_string(default_config['CLASS_NAME'])
  # create the output directory
  # os.mkdir(out_dir)
  # generate the source files
  with open('template.c', 'r') as source:
    content = source.read()
    for key in default_config.keys():
      content.replace('{{' + key + '}}', default_config[key])
    print(content)
  # os.path.join(outdir, default_config['FILE_NAME'] + '.c')

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument('--config', default='')
  parser.add_argument('--out-dir', default='gst-my-plugin')
  args = parser.parse_args()
  generate(args.config, args.out_dir)
