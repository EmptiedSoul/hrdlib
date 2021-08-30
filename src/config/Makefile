CC = gcc
ADDFLAGS =
CFLAGS = -Ofast -pipe -fomit-frame-pointer -flto -fuse-linker-plugin -fuse-ld=gold -floop-interchange -ftree-loop-distribution -floop-strip-mine -floop-block -fPIC
DBGFLAGS =
WARNFLAGS = -Wall -Wextra -Wshadow -Werror
LDFLAGS = -Wl,-O1  -lrt 

MAKEFLAGS += --no-print-directory

FLAGS := ${CFLAGS} ${DGBFLAGS} ${WARNFLAGS} ${ADDFLAGS} 

objects = $(patsubst %.c,%.o,$(wildcard *.c))

all: ${objects}

%.o: %.c
	@echo "  CC    $@"
	@$(CC) -o $@ -c $< $(FLAGS)
