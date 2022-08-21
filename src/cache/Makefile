CC ?= gcc
ADDFLAGS =
CFLAGS = -Ofast -pipe -fomit-frame-pointer -flto -fuse-linker-plugin -fuse-ld=gold -floop-interchange -ftree-loop-distribution -floop-strip-mine -floop-block -fPIC
DBGFLAGS =
WARNFLAGS = -Wall -pedantic -Wextra -Wshadow -Werror -Wno-unused-result -Wno-unused-but-set-parameter
LDFLAGS = -Wl,-O1  -lrt 

MAKEFLAGS += --no-print-directory

FLAGS := ${CFLAGS} ${DBGFLAGS} ${WARNFLAGS} ${ADDFLAGS} 

objects = $(patsubst %.c,%.o,$(wildcard *.c))

all: ${objects}

%.o: %.c
	@echo "  CC    $@"
	@$(CC) -o $@ -c $< $(FLAGS)
