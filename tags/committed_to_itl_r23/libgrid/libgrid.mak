# Makefile - libgrid.dsp

ProjectDir=.
ProjectName=libgrid
ConfigurationName=Debug

ifndef CFG
CFG=libgrid - Win32 Debug
endif
CC=gcc
CFLAGS=
CXX=g++
CXXFLAGS=$(CFLAGS)
RC=windres -O COFF
ifeq "$(CFG)"  "libgrid - Win32 Release"
CFLAGS+=-fexceptions -O2 -DNDEBUG -D_MBCS -D_LIB -W
TARGET=../lib/$(ConfigurationName)/liblibgrid.a
AR=ar
ARFLAGS=rus
else
ifeq "$(CFG)"  "libgrid - Win32 Debug"
CFLAGS+=-fexceptions -g -O0 -D_DEBUG -D_MBCS -D_LIB -W
TARGET=../lib/$(ConfigurationName)/liblibgrid.a
AR=ar
ARFLAGS=rus
endif
endif

ifndef TARGET
TARGET=liblibgrid.a
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
	libgrid.cpp

ifeq "$(CFG)"  "libgrid - Win32 Release"
else
ifeq "$(CFG)"  "libgrid - Win32 Debug"

../include/libgrid.h : libgrid.h 
	cp libgrid.h ../include/libgrid.h

endif
endif
ifeq "$(CFG)"  "libgrid - Win32 Release"
else
ifeq "$(CFG)"  "libgrid - Win32 Debug"

../include/triangle.h : triangle.h 
	cp triangle.h ../include/triangle.h

endif
endif
HEADER_FILES= \
	libgrid.h \
	triangle.h

CUSTOM_OUTPUTS= \
	../include/libgrid.h \
	../include/triangle.h

SRCS=$(SOURCE_FILES) $(HEADER_FILES) 

OBJS=$(patsubst %.rc,%.res,$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.c %.cc %.cpp %.cxx %.rc,$(SRCS)))))))

$(TARGET): $(OBJS) $(CUSTOM_OUTPUTS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET) $(CUSTOM_OUTPUTS) libgrid.dep

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > libgrid.dep

-include libgrid.dep

