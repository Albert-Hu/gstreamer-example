# The Simple Plugin to Flip Video Stream

This example intends to get the beginner started to learn image processing on GStreamer.

# How to Build

There are two ways to build the plugin:

- Meson
- Makefile

## Meson

```shell
meson build
ninja -C build
```

## Makefile

```shell
make all
```

## Test
```shell
export GST_PLUGIN_PATH_1_0=/your/plugin/path
gst-launch-1.0 -v videotestsrc ! video/x-raw,width=480,height=360,format=RGBA ! myvideoflip ! videoconvert ! autovideosink
```
