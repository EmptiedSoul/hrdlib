CC ?= gcc
export CC
DESTDIR=
ADDFLAGS =
CFLAGS = -Ofast -pipe -fomit-frame-pointer -flto=jobserver -fuse-linker-plugin -fuse-ld=gold -fPIC -floop-block -floop-interchange -ftree-loop-distribution -floop-strip-mine #-fanalyzer #-fanalyzer-verbosity=3
export CFLAGS
DBGFLAGS = -g3 -ggdb3
export DBGFLAGS
WARNFLAGS = -Wall -pedantic -Wextra -Wshadow -Werror -Wno-pointer-arith -Wno-analyzer-malloc-leak
LDFLAGS = -Wl,-O1  

MAKEFLAGS += --no-print-directory

FLAGS := ${CFLAGS} ${DBGFLAGS} ${WARNFLAGS} ${ADDFLAGS} 

objects = $(patsubst %.c,%.o,$(wildcard */*.c))

all: libhrd.so

libhrd.so: ipc-compile config-compile cache-compile file-compile strings-compile hashmap-compile misc-compile
	@echo "  CCLD  libhrd.so" 
	+@$(CC) $(FLAGS) $(LDFLAGS) -shared -o libhrd.so ${objects} 
strings-compile:
	@$(MAKE) -e -C strings
ipc-compile:
	@$(MAKE) -e -C ipc
config-compile:
	@$(MAKE) -e -C config
cache-compile:
	@$(MAKE) -e -C cache
file-compile:
	@$(MAKE) -e -C fs
hashmap-compile:
	@$(MAKE) -e -C hashmap
misc-compile:
	@$(MAKE) -e -C misc

clean: 
	@echo "  CLEAN */*.o"
	@rm -f ${objects}
	@rm -f libhrd.so
install: all
	@install -vm755 ./libhrd.so ${DESTDIR}/usr/lib
	@install -vm644 ./libhrd.h  ${DESTDIR}/usr/include
	@install -vm644 ./libhrd_pseudonames.h ${DESTDIR}/usr/include
	@install -vm644 ./libhrd.pc ${DESTDIR}/usr/lib/pkgconfig/libhrd.pc
