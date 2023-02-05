# Flags
CFLAGS += -Wpedantic -Wall -Wextra -Wno-implicit-fallthrough -Wno-missing-field-initializers -Wno-unused-parameter -Wfatal-errors -Wno-strict-prototypes -std=c99 -D_POSIX_C_SOURCE=200809L -Werror -O2

OS := $(shell uname -s)
ifeq ($(OS),Darwin)
	CFLAGS += -D_DARWIN_C_SOURCE
endif

# Files lists
C_SRC := conf.c ex.c lbuf.c led.c nextvi_regex.c ren.c term.c uc.c vi.c
C_HEAD := $(C_SRC:%.c=%.h) kmap.h helper.h
C_OBJS := $(C_SRC:%.c=%.o)

# Install targets
TARGET_DIR_BIN ?= /usr/local/bin

# Commands
CC := gcc
CP := cp -f
RM := rm -rf

all: vi ex

%.o : %.c $(C_HEAD)
	$(CC) -c $< $(CFLAGS) -o $@

vi: $(C_OBJS)
	$(CC) $(C_OBJS) $(CFLAGS) -o $@

ex: vi
	cp vi ex

install : | vi ex
	mkdir -p $(TARGET_DIR_BIN)
	$(CP) vi ex $(TARGET_DIR_BIN)/

uninstall :
	$(RM) $(TARGET_DIR_BIN)/ex
	$(RM) $(TARGET_DIR_BIN)/vi

clean : 
	$(RM) vi
	$(RM) ex
	$(RM) *.o

