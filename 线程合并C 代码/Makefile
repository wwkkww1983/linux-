

ACH   = arm-linux-gnueabihf-


AS		= $(ACH)as
LD		= $(ACH)ld
CC		= $(ACH)gcc
CPP		= $(CC) -E
AR		= $(ACH)ar
NM		= $(ACH)nm

STRIP		= $(ACH)strip
OBJCOPY	= $(ACH)objcopy
OBJDUMP	= $(ACH)objdump

export AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP

CFLAGS := -Wall -O2 -g
CFLAGS += -I $(shell pwd)/inc

LDFLAGS := -lm  -lpthread 

export CFLAGS LDFLAGS

TOPDIR := $(shell pwd)
export TOPDIR

TARGET := card


obj-y += main.o
obj-y += can/
obj-y += dec/
obj-y += proc/
obj-y += serial/
obj-y += socket/
obj-y += system/



all : 
	make -C ./ -f $(TOPDIR)/Makefile.build
	$(CC) $(LDFLAGS) -o $(TARGET) built-in.o
	

clean:
	rm -f $(shell find -name "*.o")
	rm -f $(TARGET)

distclean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(TARGET)
	
