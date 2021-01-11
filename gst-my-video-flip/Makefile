CC := gcc

DEFINES := -DVERSION=\"0.0.1\"\
	-DPACKAGE_NAME=\"my_video_flip\"\
	-DPACKAGE_VERSION=\"0.0.1\"\
	-DPACKAGE=\"gst-my-video-flip-plugin\"\
	-DGST_LICENSE=\"LGPL\"\
	-DGST_API_VERSION=\"1.0\"\
	-DGST_PACKAGE_NAME=\"my_video_flip\"\
	-DGST_PACKAGE_ORIGIN=\"https://github.com/albert-hu/gstreamer-example/gst-my-video-flip\"

CFLAGS := -Wall -Werror -fPIC $(DEFINES)\
	$(shell pkg-config --cflags gstreamer-1.0)\
	$(shell pkg-config --cflags gstreamer-base-1.0)\
	$(shell pkg-config --cflags gstreamer-video-1.0)

LDFLAGS := $(shell pkg-config --libs gstreamer-1.0)\
	$(shell pkg-config --libs gstreamer-base-1.0)\
	$(shell pkg-config --libs gstreamer-video-1.0)

SRC := gstmyvideoflip.c
OBJ := $(SRC:.c=.o)

TARGET := libgstmyvideoflip.so

plugin: $(TARGET)

%.d: %.c
	$(CC) $(CFLAGS) -c -o $<

$(TARGET): $(OBJ)
	$(CC) -shared -o $@ $< $(LDFLAGS)

test: $(TARGET)
	GST_PLUGIN_PATH_1_0=$(shell pwd) gst-launch-1.0 -v videotestsrc ! video/x-raw,width=480,height=360,format=RGBA ! myvideoflip ! videoconvert ! autovideosink

clean:
	rm -f *.o $(TARGET)
