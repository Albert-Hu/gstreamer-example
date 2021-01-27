from __future__ import print_function
from __future__ import division

import argparse, json, re, os

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

def generate_file(template, out_file):
  current_path = os.path.dirname(os.path.abspath(__file__))
  with open(os.path.join(current_path, template), 'r') as source:
    content = re.split('(\\{\\{[a-zA-Z0-9_]+\\}\\})', source.read())
    for index, key in enumerate(content):
      if len(key) > 4 and key[:2] == '{{' and key[-2:] == '}}':
        key = key[2:-2]
        if key in default_config.keys():
          content[index] = default_config[key]
    content = ''.join(content)
    with open(out_file, 'w') as new_source:
      new_source.write(content)
  print(out_file)

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
  if not os.path.exists(out_dir):
    os.mkdir(out_dir)
  print('Generate the files to {}.'.format(out_dir))
  # generate the source files
  generate_file('template.c', os.path.join(out_dir, 'gst' + default_config['FILE_NAME'] + '.c'))
  generate_file('template.h', os.path.join(out_dir, 'gst' + default_config['FILE_NAME'] + '.h'))
  generate_file('Makefile', os.path.join(out_dir, 'Makefile'))
  generate_file('meson.build', os.path.join(out_dir, 'meson.build'))

if __name__ == "__main__":
  parser = argparse.ArgumentParser()
  parser.add_argument('--config', default='')
  parser.add_argument('--out-dir', default='gst-my-plugin')
  args = parser.parse_args()
  generate(args.config, args.out_dir)
