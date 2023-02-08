# Flags
CFLAGS += -Wpedantic -Wall -Wextra -Wno-implicit-fallthrough -Wno-missing-field-initializers -Wno-unused-parameter -Wfatal-errors -Wno-strict-prototypes -std=c99 -D_POSIX_C_SOURCE=200809L -O2

OS := $(shell uname -s)
ifeq ($(OS),Darwin)
	CFLAGS += -D_DARWIN_C_SOURCE
endif
ifdef MARCHNATIVE
	CFLAGS += -march=native # Optimize for the current CPU, useful for people who don't share their binaries
endif

# Files lists
C_SRC := conf.c ex.c lbuf.c led.c nextvi_regex.c ren.c term.c uc.c vi.c
C_HEAD := $(C_SRC:%.c=%.h) kmap.h helper.h
C_OBJS := $(C_SRC:%.c=%.o)

# Install targets
PREFIX ?= /usr/local

# Commands
CC := gcc
CP := cp -f
RM := rm -rf
LN := ln -f
STRIP := strip

all: vi ex

%.o : %.c $(C_HEAD)
	$(CC) -c $< $(CFLAGS) -o $@

vi: $(C_OBJS)
	$(CC) $(C_OBJS) $(CFLAGS) -o $@

ex: vi
	$(LN) vi ex

install : | vi ex
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	$(CP) vi $(DESTDIR)$(PREFIX)/bin/
	$(STRIP) $(DESTDIR)$(PREFIX)/bin/vi
	$(LN) $(DESTDIR)$(PREFIX)/bin/vi $(DESTDIR)$(PREFIX)/bin/ex

uninstall :
	$(RM) $(DESTDIR)$(PREFIX)/bin/ex
	$(RM) $(DESTDIR)$(PREFIX)/bin/vi

clean : 
	$(RM) vi
	$(RM) ex
	$(RM) *.o
