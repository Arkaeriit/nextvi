# Flags
CFLAGS += -Wpedantic -Wall -Wextra -Wno-implicit-fallthrough -Wno-missing-field-initializers -Wno-unused-parameter -Wfatal-errors -std=c99 -D_POSIX_C_SOURCE=200809L


# Files lists
C_SRC := conf.c ex.c lbuf.c led.c regex.c ren.c term.c uc.c vi.c
C_OBJS := $(C_SRC:%.c=%.o)
TARGET := vi

# Install targets
TARGET_DIR_BIN ?= /usr/local/bin

# Commands
CC := gcc
CP := cp -f
RM := rm -rf

%.o : %.c
	$(CC) -c $< $(FLAGS) -o $@

$(TARGET): $(C_OBJS)
	$(CC) $(C_OBJS) $(FLAGS) -o $@

install :
	mkdir -p $(TARGET_DIR_BIN)
	$(CP) $(TARGET) $(TARGET_DIR_BIN)/ex
	$(CP) $(TARGET) $(TARGET_DIR_BIN)/vi

uninstall :
	$(RM) $(TARGET_DIR_BIN)/ex
	$(RM) $(TARGET_DIR_BIN)/vi

clean : 
	$(RM) $(TARGET)
	$(RM) *.o

