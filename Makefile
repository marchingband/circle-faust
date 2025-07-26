#
# Makefile
#

CIRCLEHOME = ../..

OBJS = $(patsubst %.c, %.o, $(wildcard *.c)) $(patsubst %.cpp, %.o, $(wildcard *.cpp))

LIBS	= $(CIRCLEHOME)/lib/sound/libsound.a \
		$(CIRCLEHOME)/lib/libcircle.a \
		$(CIRCLEHOME)/addon/sensor/libsensor.a \
		$(CIRCLEHOME)/lib/usb/libusb.a \
		$(CIRCLEHOME)/lib/usb/gadget/libusbgadget.a \
		$(CIRCLEHOME)/lib/fs/libfs.a \
		$(CIRCLEHOME)/lib/input/libinput.a 

include $(CIRCLEHOME)/Rules.mk

-include $(DEPS)