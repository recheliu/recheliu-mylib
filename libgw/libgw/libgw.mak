# Makefile - libgw.dsp

ProjectName=libgw
ProjectDir=.
ConfigurationName=Debug

ifndef CFG
CFG=libgw - Win32 Debug
endif
CC=gcc
CFLAGS=-I ~/lib/include -I ~/lib/include/opencv -I ~/mylib/include -I /usr/local/cuda/include
CXX=g++
CXXFLAGS=$(CFLAGS)
RC=windres -O COFF
ifeq "$(CFG)"  "libgw - Win32 Debug"
CFLAGS+=-g -W -O0 -D_DEBUG -D_LIB -D_CRT_TERMINATE_DEFINED -D_UNICODE
AR=ar
TARGET=../../lib/$(ConfigurationName)/liblibgw.a
ARFLAGS=rus
else
ifeq "$(CFG)"  "libgw - Win32 Release"
CFLAGS+=-g -W -DNDEBUG -D_LIB -D"_UNICODE" PRECOMP_VC7_TOBEREMOVED
AR=ar
TARGET=../../lib/$(ConfigurationName)/liblibgw.a
ARFLAGS=rus
endif
endif

ifndef TARGET
TARGET=liblibgw.a
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

SOURCE_FILES= \
	GlutWin.cpp GlutWin_static.cpp

ifeq "$(CFG)"  "libgw - Win32 Debug"

BuildCmds= \
	cp GlutWin.h ../../include/GlutWin.h

../../include/GlutWin.h: GlutWin.h
	$(BuildCmds)

else
ifeq "$(CFG)"  "libgw - Win32 Release"

BuildCmds= \
	cp GlutWin.h ../../include/GlutWin.h

../../include/GlutWin.h: GlutWin.h 
	$(BuildCmds)
endif
endif
HEADER_FILES= \
	GlutWin.h \
	$(InputPath) \
	$(InputPath)

CUSTOM_OUTPUTS= \
	../../include/GlutWin.h 


RESOURCE_FILES=

SRCS=$(SOURCE_FILES) $(HEADER_FILES) $(RESOURCE_FILES)  \
	ReadMe.txt

OBJS=$(patsubst %.rc,%.res,$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.c %.cc %.cpp %.cxx %.rc,$(SRCS)))))))

$(TARGET): $(OBJS) $(CUSTOM_OUTPUTS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET) $(CUSTOM_OUTPUTS) libgw.dep

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > libgw.dep

-include libgw.dep

