# Makefile - libbuf.dsp

ConfigurationName=Debug

ifndef CFG
CFG=libbuf - Win32 Release
endif
CC=gcc
CFLAGS=
CXX=g++
CXXFLAGS=$(CFLAGS)
RC=windres -O COFF
ifeq "$(CFG)"  "libbuf - Win32 Release"
CFLAGS+=-W -O2 -DWIN32 -DNDEBUG -D_LIB -D_MBCS
AR=ar
ARFLAGS=rus
else
ifeq "$(CFG)"  "libbuf - Win32 Debug"
CFLAGS+=-g -W -O0 -DWIN32 -D_DEBUG -D_LIB -D"_MBCS" PRECOMP_VC7_TOBEREMOVED
AR=ar
ARFLAGS=rus
endif
endif

ifndef TARGET
TARGET=liblibbuf.a
endif

.PHONY: all
all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: %.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

%.res: %.rc
	$(RC) $(CPPFLAGS) -o $@ -i $<

ifeq "$(CFG)"  "libbuf - Win32 Release"

BuildCmds= \
	cp libbuf.h ../include/libbuf.h

../include/libbuf.h: libbuf.h
	$(BuildCmds)
else
ifeq "$(CFG)"  "libbuf - Win32 Debug"

BuildCmds= \
	cp libbuf.h ../include/libbuf.h

../include/libbuf.h: libbuf.h
	$(BuildCmds)
endif
endif
HEADER_FILES= \
	libbuf.h \
	$(InputPath) \
	$(InputPath)

CUSTOM_OUTPUTS= \
	../include/libbuf.h

SOURCE_FILES=

SRCS=$(HEADER_FILES) $(SOURCE_FILES) 

OBJS=$(patsubst %.rc,%.res,$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.c %.cc %.cpp %.cxx %.rc,$(SRCS)))))))

$(TARGET): $(OBJS) $(CUSTOM_OUTPUTS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(CUSTOM_OUTPUTS) $(TARGET) libbuf.dep

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > libbuf.dep

-include libbuf.dep

