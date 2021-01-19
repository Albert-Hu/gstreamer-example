CC := gcc

DEFINES := -DVERSION=\"{{VERSION}}\"\
	-DPACKAGE_NAME=\"{{PACKAGE_NAME}}\"\
	-DPACKAGE_VERSION=\"{{PACKAGE_VERSION}}\"\
	-DPACKAGE=\"{{PACKAGE}}\"\
	-DGST_LICENSE=\"{{GST_LICENSE}}\"\
	-DGST_API_VERSION=\"{{GST_API_VERSION}}\"\
	-DGST_PACKAGE_NAME=\"{{GST_PACKAGE_NAME}}\"\
	-DGST_PACKAGE_ORIGIN=\"{{GST_PACKAGE_ORIGIN}}\"

CFLAGS := -Wall -Werror -fPIC $(DEFINES)\
	$(shell pkg-config --cflags gstreamer-1.0)\
	$(shell pkg-config --cflags gstreamer-base-1.0)\
	$(shell pkg-config --cflags gstreamer-video-1.0)

LDFLAGS := $(shell pkg-config --libs gstreamer-1.0)\
	$(shell pkg-config --libs gstreamer-base-1.0)\
	$(shell pkg-config --libs gstreamer-video-1.0)

SRC := gst{{FILE_NAME}}.c
OBJ := $(SRC:.c=.o)

TARGET := libgst{{FILE_NAME}}.so

plugin: $(TARGET)

%.d: %.c
	$(CC) $(CFLAGS) -c -o $<

$(TARGET): $(OBJ)
	$(CC) -shared -o $@ $< $(LDFLAGS)

test:
	GST_PLUGIN_PATH_1_0=$(shell pwd) gst-launch-1.0 -v videotestsrc ! video/x-raw,width=480,height=360 ! {{PLUGIN_NAME}} ! videoconvert ! autovideosink

clean:
	rm -f *.o $(TARGET)
