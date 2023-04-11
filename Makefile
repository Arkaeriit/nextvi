# Flags
CFLAGS += -Wpedantic -Wall -Wextra -Wno-implicit-fallthrough -Wno-missing-field-initializers -Wno-unused-parameter -Wfatal-errors -Wno-strict-prototypes -Wno-maybe-uninitialized -std=c99 -D_POSIX_C_SOURCE=200809L -O2

OS := $(shell uname -s)
ifeq ($(OS),Darwin)
	CFLAGS += -D_DARWIN_C_SOURCE
endif
ifdef MARCHNATIVE
	CFLAGS += -march=native
endif
ifdef LTO
	CFLAGS += -flto
endif

# Files lists
SRC := conf.c ex.c lbuf.c led.c nextvi_regex.c ren.c term.c uc.c vi.c
OBJS := $(SRC:%.c=%.o)

# Install targets
PREFIX ?= /usr/local

# Commands
HASGCC := $(shell command -v $(CROSS_COMPILE)gcc 2> /dev/null)
ifdef HASGCC
	CC := $(CROSS_COMPILE)gcc
else
	HASCLANG := $(shell command -v $(CROSS_COMPILE)clang 2> /dev/null)
ifdef HASCLANG
	CC := $(CROSS_COMPILE)clang
else
	CC := $(CROSS_COMPILE)cc
endif
endif
RM := rm -rf
ifeq ($(shell uname -o),Android)
	CP := cp -f
else
	CP := cp -lf
endif
STRIP := $(CROSS_COMPILE)strip

all: vi ex

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

vi: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $@

ex: vi
	$(CP) vi ex

install: vi ex
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	$(CP) vi $(DESTDIR)$(PREFIX)/bin/vi
	$(STRIP) $(DESTDIR)$(PREFIX)/bin/vi
	$(CP) $(DESTDIR)$(PREFIX)/bin/vi $(DESTDIR)$(PREFIX)/bin/ex

uninstall:
	$(RM) $(DESTDIR)$(PREFIX)/bin/ex $(DESTDIR)$(PREFIX)/bin/vi

clean:
	$(RM) vi ex *.o
