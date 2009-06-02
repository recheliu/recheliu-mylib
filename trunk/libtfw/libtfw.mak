# Makefile - libtfw.dsp

ProjectDir=.
ProjectName=libtfw
ConfigurationName=Debug

ifndef CFG
CFG=libtfw - Win32 Debug
endif
CC=gcc
CFLAGS=
CXX=g++
CXXFLAGS=$(CFLAGS)
RC=windres -O COFF
ifeq "$(CFG)"  "libtfw - Win32 Release"
CFLAGS+=-fexceptions -O2 -DNDEBUG -D_MBCS -D_LIB -W
TARGET=../lib/$(ConfigurationName)/liblibtfw.a
AR=ar
ARFLAGS=rus
else
ifeq "$(CFG)"  "libtfw - Win32 Debug"
CFLAGS+=-fexceptions -g -O0 -D_DEBUG -D_MBCS -D_LIB -W
TARGET=../lib/$(ConfigurationName)/liblibtfw.a
AR=ar
ARFLAGS=rus
endif
endif

ifndef TARGET
TARGET=liblibtfw.a
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

../include/libtfw/knot.h : knot.h 
	cp knot.h ../include/libtfw/knot.h

../include/libtfw/TransFunc.h : TransFunc.h 
	cp TransFunc.h ../include/libtfw/TransFunc.h

../include/libtfw/TfUi.h : TfUi.h 
	cp TfUi.h ../include/libtfw/TfUi.h

../include/libtfw/TfWin.h : TfWin.h 
	cp TfWin.h ../include/libtfw/TfWin.h

SOURCE_FILES= \
	TransFunc.cpp \
	TfUi.cpp \
	TfWin.cpp

HEADER_FILES= \
	knot.h \
	TransFunc.h \
	TfUi.h \
	TfWin.h

CUSTOM_OUTPUTS= \
	../include/libtfw/knot.h \
	../include/libtfw/TransFunc.h \
	../include/libtfw/TfUi.h \
	../include/libtfw/TfWin.h

SRCS=$(SOURCE_FILES) $(HEADER_FILES) 

OBJS=$(patsubst %.rc,%.res,$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.c %.cc %.cpp %.cxx %.rc,$(SRCS)))))))

$(TARGET): $(OBJS) $(CUSTOM_OUTPUTS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET) $(CUSTOM_OUTPUTS) libtfw.dep

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > libtfw.dep

-include libtfw.dep

