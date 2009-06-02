# Makefile - libopengl.dsp

ProjectName=libopengl
ProjectDir=.
ConfigurationName=Debug

ifndef CFG
CFG=libopengl - Win32 Debug
endif
CC=gcc
CFLAGS=
CXX=g++
CXXFLAGS=$(CFLAGS)
RC=windres -O COFF
ifeq "$(CFG)"  "libopengl - Win32 Debug"
CFLAGS+=-g -W -O0 -D_DEBUG -D_LIB -D_CRT_TERMINATE_DEFINED -D_UNICODE -I ~/lib/include
AR=ar
TARGET=../lib/$(ConfigurationName)/liblibopengl.a
ARFLAGS=rus
else
ifeq "$(CFG)"  "libopengl - Win32 Release"
CFLAGS+=-g -W -DNDEBUG -D_LIB -D"_UNICODE" PRECOMP_VC7_TOBEREMOVED -I ~/lib/include
AR=ar
TARGET=../lib/$(ConfigurationName)/liblibopengl.a
ARFLAGS=rus
endif
endif

ifndef TARGET
TARGET=liblibopengl.a
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
	libopengl.cpp

ifeq "$(CFG)"  "libopengl - Win32 Debug"

BuildCmds= \
	cp libopengl.h ../include/libopengl.h

../include/libopengl.h: libopengl.h
	$(BuildCmds)

else
ifeq "$(CFG)"  "libopengl - Win32 Release"

BuildCmds= \
	cp libopengl.h ../include/libopengl.h

../include/libopengl.h: libopengl.h 
	$(BuildCmds)
endif
endif
HEADER_FILES= \
	libopengl.h \
	$(InputPath) \
	$(InputPath)

CUSTOM_OUTPUTS= \
	../include/libopengl.h 


RESOURCE_FILES=

SRCS=$(SOURCE_FILES) $(HEADER_FILES) $(RESOURCE_FILES)  \
	ReadMe.txt

OBJS=$(patsubst %.rc,%.res,$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.c %.cc %.cpp %.cxx %.rc,$(SRCS)))))))

$(TARGET): $(OBJS) $(CUSTOM_OUTPUTS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET) $(CUSTOM_OUTPUTS) libopengl.dep

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > libopengl.dep

-include libopengl.dep

