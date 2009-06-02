# Makefile - mylib.dsw

.PHONY: all
all: \
	libbuf \
	libfps \
	libopt \
	libgrid \
	libopengl \
	libgw \
	libfbo \
	libshader \
	shader2string \
	libcuda \
	libtfw \
	libdvr \
	libfbow

.PHONY: libfbo
libfbo:
	$(MAKE) -C ../libfbo -f libfbo.mak

.PHONY: libfps
libfps:
	$(MAKE) -C ../libfps -f libfps.mak

.PHONY: libgrid
libgrid:
	$(MAKE) -C ../libgrid -f libgrid.mak

.PHONY: libgw
libgw:
	$(MAKE) -C ../libgw/libgw -f libgw.mak

.PHONY: libopengl
libopengl:
	$(MAKE) -C ../libopengl -f libopengl.mak

.PHONY: libopt
libopt:
	$(MAKE) -C ../libopt -f libopt.mak

.PHONY: libshader
libshader:
	$(MAKE) -C ../libshader -f libshader.mak

.PHONY: libbuf
libbuf:
	$(MAKE) -C ../libbuf -f libbuf.mak

.PHONY: libcuda
libcuda:
	$(MAKE) -C ../libcuda -f libcuda.mak

.PHONY: libtfw
libtfw:
	$(MAKE) -C ../libtfw -f libtfw.mak

.PHONY: libdvr
libdvr:
	$(MAKE) -C ../libdvr -f libdvr.mak

.PHONY: shader2string
shader2string:
	$(MAKE) -C ../libshader/shader2string -f shader2string.mak

.PHONY: libfbow
libfbow:
	$(MAKE) -C ../libfbow -f libfbow.mak

.PHONY: clean
clean:
	$(MAKE) -C ../libbuf -f libbuf.mak clean
	$(MAKE) -C ../libfps -f libfps.mak clean
	$(MAKE) -C ../libopt -f libopt.mak clean
	$(MAKE) -C ../libgrid -f libgrid.mak clean
	$(MAKE) -C ../libgw/libgw -f libgw.mak clean
	$(MAKE) -C ../libopengl -f libopengl.mak clean
	$(MAKE) -C ../libshader -f libshader.mak clean
	$(MAKE) -C ../libfbo -f libfbo.mak clean
	$(MAKE) -C ../libcuda -f libcuda.mak clean
	$(MAKE) -C ../libtfw -f libtfw.mak clean
	$(MAKE) -C ../libdvr -f libdvr.mak clean
	$(MAKE) -C ../libshader/shader2string -f shader2string.mak clean
	$(MAKE) -C ../libfbow -f libfbow.mak clean

.PHONY: depends
depends:
	$(MAKE) -C ../libfbo -f libfbo.mak depends
	$(MAKE) -C ../libfps -f libfps.mak depends
	$(MAKE) -C ../libgrid -f libgrid.mak depends
	$(MAKE) -C ../libgw/libgw -f libgw.mak depends
	$(MAKE) -C ../libopengl -f libopengl.mak depends
	$(MAKE) -C ../libopt -f libopt.mak depends
	$(MAKE) -C ../libshader -f libshader.mak depends
	$(MAKE) -C ../libbuf -f libbuf.mak depends
	$(MAKE) -C ../libcuda -f libcuda.mak depends
	$(MAKE) -C ../libtfw -f libtfw.mak depends
	$(MAKE) -C ../libdvr -f libdvr.mak depends
	$(MAKE) -C ../libshader/shader2string -f shader2string.mak depends
	$(MAKE) -C ../libfbow -f libfbow.mak depends

