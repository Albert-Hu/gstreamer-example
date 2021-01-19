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
	$(shell pkg-config --cflags gstreamer-base-1.0)

LDFLAGS := $(shell pkg-config --libs gstreamer-1.0)\
	$(shell pkg-config --libs gstreamer-base-1.0)

SRC := gst{{FILE_NAME}}.c
OBJ := $(SRC:.c=.o)

TARGET := libgst{{FILE_NAME}}.so

plugin: $(TARGET)

%.d: %.c
	$(CC) $(CFLAGS) -c -o $<

$(TARGET): $(OBJ)
	$(CC) -shared -o $@ $< $(LDFLAGS)

clean:
	rm -f *.o $(TARGET)
