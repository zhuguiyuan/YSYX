export ARCH = native

NAME = minirvEMU
SRCS = main.c state.c inst.c
include $(AM_HOME)/Makefile

CFLAGS      += $(shell pkg-config --cflags glib-2.0) -g
LDFLAGS_CXX += $(shell pkg-config --libs glib-2.0) -g
