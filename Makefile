# ------------------------------------------------------------------
# Makefile - TP-Git-Collaboration
# Usage : make          -> compile securemsg.exe
#         make run      -> compile puis execute
#         make clean    -> supprime les binaires et objets
# ------------------------------------------------------------------
CC      ?= gcc
CFLAGS  ?= -std=c11 -Wall -Wextra -O2
LDFLAGS ?=
TARGET   = securemsg.exe

SRCS = main.c module1.c module2.c module3.c
OBJS = $(SRCS:.c=.o)
HDRS = security.h

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(OBJS) $(TARGET)
