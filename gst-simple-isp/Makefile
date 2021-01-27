CXX := g++

DEFINES := -DVERSION=\"0.0.1\"\
	-DPACKAGE_NAME=\"simple_isp\"\
	-DPACKAGE_VERSION=\"0.0.1\"\
	-DPACKAGE=\"gst-simple-isp\"\
	-DGST_LICENSE=\"LGPL\"\
	-DGST_API_VERSION=\"1.0\"\
	-DGST_PACKAGE_NAME=\"simple_isp\"\
	-DGST_PACKAGE_ORIGIN=\"https://gstreamer.freedesktop.org/\"

CXXFLAGS := -Wall -Werror -fPIC $(DEFINES)\
	$(shell pkg-config --cflags gstreamer-1.0)\
	$(shell pkg-config --cflags gstreamer-base-1.0)\
	$(shell pkg-config --cflags gstreamer-video-1.0)\
	$(shell pkg-config --cflags opencv)

LDFLAGS := $(shell pkg-config --libs gstreamer-1.0)\
	$(shell pkg-config --libs gstreamer-base-1.0)\
	$(shell pkg-config --libs gstreamer-video-1.0)\
	$(shell pkg-config --libs opencv)

TARGET := libgstsimpleisp.so

SRC := gstsimpleisp.cpp\
			 isp.cpp

OBJ := $(SRC:.cpp=.o)

%.d: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $<

$(TARGET): $(OBJ)
	$(CXX) -shared -o $@ $^ $(LDFLAGS)

plugin: $(TARGET)

test: $(TARGET)
	GST_PLUGIN_PATH_1_0=$(shell pwd) gst-launch-1.0 -v videotestsrc ! video/x-raw,width=480,height=360 ! simpleisp ! videoconvert ! autovideosink

test_gaussian_noise: $(TARGET)
	GST_PLUGIN_PATH_1_0=$(shell pwd) gst-launch-1.0 -v videotestsrc ! video/x-raw,width=480,height=360 ! simpleisp gaussian-noise=true ! videoconvert ! autovideosink

clean:
	rm -f *.o $(TARGET)
