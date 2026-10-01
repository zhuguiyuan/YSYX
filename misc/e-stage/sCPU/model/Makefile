CPPFLAGS += $(shell pkg-config --cflags glib-2.0)
LDLIBS   += $(shell pkg-config --libs glib-2.0)

all: sEMU

dev: CFLAGS += -g
dev: all


clean:
	rm -f sEMU

ide:
	bear -- $(MAKE)

.PHONY: all clean ide
