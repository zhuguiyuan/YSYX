export ARCH = native
NAME = screensaver
SRCS = screensaver.c
include $(AM_HOME)/Makefile
CFLAGS      += $(shell pkg-config --cflags glib-2.0)
LDFLAGS_CXX += $(shell pkg-config --libs glib-2.0)