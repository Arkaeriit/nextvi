# Flags
CFLAGS += -Wall -Wextra -Wno-implicit-fallthrough -Wno-missing-field-initializers -Wno-unused-parameter -Wfatal-errors -Wno-strict-prototypes -std=c99 -D_POSIX_C_SOURCE=200809L -O2

# Files lists
C_SRC := conf.c ex.c lbuf.c led.c nextvi_regex.c ren.c term.c uc.c vi.c
C_HEAD := $(C_SRC:%.c=%.h) kmap.h helper.h
C_OBJS := $(C_SRC:%.c=%.o)
COSMO_OBJS := $(C_SRC:%.c=%.cosmo.o)

COSMO_CFLAGS := -g -Os -static -fno-pie -no-pie -nostdlib -nostdinc -gdwarf-4  -fno-omit-frame-pointer -pg -mnop-mcount -mno-tls-direct-seg-refs -Wl,--gc-sections -fuse-ld=bfd -Wl,--gc-sections -I./cosmopolitan  -Wl,-T,cosmopolitan/ape.lds -DNEXTVI_WITH_COSMO
COSMO_LDFLAGS := cosmopolitan/cosmopolitan.a cosmopolitan/ape-no-modify-self.o cosmopolitan/crt.o

NOCOSMO_CFLAGS := -Wpedantic

all : vi ex

OS := $(shell uname -s)
ifeq ($(OS),Darwin)
	CFLAGS += -D_DARWIN_C_SOURCE
endif
ifdef MARCHNATIVE
	CFLAGS += -march=native # Optimize for the current CPU, useful for people who don't share their binaries
endif

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
OBJCOPY := $(CROSS_COMPILE)objcopy
RM := rm -rf
ifeq ($(shell uname -o),Android)
	CP := cp -f
else
	CP := cp -lf
endif
STRIP := $(CROSS_COMPILE)strip

%.o : %.c $(C_HEAD)
	$(CC) -c $< $(CFLAGS) $(NOCOSMO_CFLAGS) -o $@

%.cosmo.o : %.c $(C_HEAD) cosmopolitan/cosmopolitan.h
	$(CC) -c $< $(CFLAGS) $(COSMO_CFLAGS) -o $@

vi: $(C_OBJS)
	$(CC) $(C_OBJS) $(LDFLAGS) $(NOCOSMO_CFLAGS) $(CFLAGS) -o $@

ex : vi
	$(CP) $< $@

ex.com : vi.com
	$(CP) $< $@

vi.com.dbg: $(COSMO_OBJS)
	$(CC) $(COSMO_OBJS) $(CFLAGS) $(COSMO_CFLAGS) $(LDFLAGS) $(COSMO_LDFLAGS) -o $@

vi.com: vi.com.dbg
	$(OBJCOPY) -S -O binary $< $@ 

cosmopolitan/cosmopolitan.h:
	mkdir -p cosmopolitan
	cd cosmopolitan && \
		wget https://justine.lol/cosmopolitan/cosmopolitan-amalgamation-2.2.zip && \
		unzip cosmopolitan-amalgamation-2.2.zip && \
		mkdir -p sys && \
		ln -s cosmopolitan.h stdlib.h && \
		ln -s cosmopolitan.h string.h && \
		ln -s cosmopolitan.h stdio.h && \
		ln -s cosmopolitan.h inttypes.h && \
		ln -s cosmopolitan.h stddef.h && \
		ln -s cosmopolitan.h stdint.h && \
		ln -s cosmopolitan.h stdbool.h && \
		ln -s cosmopolitan.h ctype.h && \
		ln -s cosmopolitan.h unistd.h && \
		ln -s cosmopolitan.h fcntl.h && \
		ln -s cosmopolitan.h stdarg.h && \
		ln -s cosmopolitan.h dirent.h && \
		ln -s cosmopolitan.h poll.h && \
		ln -s cosmopolitan.h termios.h && \
		ln -s cosmopolitan.h signal.h && \
		ln -s cosmopolitan.h time.h && \
		cd sys && \
		ln -s ../cosmopolitan.h stat.h && \
		ln -s ../cosmopolitan.h wait.h && \
		ln -s ../cosmopolitan.h ioctl.h

install : | vi ex
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	$(CP) vi $(DESTDIR)$(PREFIX)/bin/
	$(STRIP) $(DESTDIR)$(PREFIX)/bin/vi
	$(CP) $(DESTDIR)$(PREFIX)/bin/vi $(DESTDIR)$(PREFIX)/bin/ex

uninstall :
	$(RM) $(DESTDIR)$(PREFIX)/bin/ex
	$(RM) $(DESTDIR)$(PREFIX)/bin/vi

clean : 
	$(RM) vi
	$(RM) *.com
	$(RM) *.com.dbg
	$(RM) ex
	$(RM) *.o
	$(RM) cosmopolitan
