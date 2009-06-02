# Makefile - libfps.dsp

ConfigurationName=Debug

ifndef CFG
CFG=libfps - Win32 Debug
endif
CC=gcc
CFLAGS=
CXX=g++
CXXFLAGS=$(CFLAGS)
RC=windres -O COFF
ifeq "$(CFG)"  "libfps - Win32 Release"
CFLAGS+=-fexceptions -O2 -DNDEBUG -D_MBCS -D_LIB -W
TARGET=../lib/$(ConfigurationName)/liblibfps.a
AR=ar
ARFLAGS=rus
else
ifeq "$(CFG)"  "libfps - Win32 Debug"
CFLAGS+=-fexceptions -g -O0 -D_DEBUG -D_MBCS -D_LIB -W
TARGET=../lib/$(ConfigurationName)/liblibfps.a
AR=ar
ARFLAGS=rus
endif
endif

ifndef TARGET
TARGET=liblibfps.a
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
	FPS_COUNTER.cpp

ifeq "$(CFG)"  "libfps - Win32 Release"

../include/FPS_COUNTER.h : FPS_COUNTER.h 
	cp FPS_COUNTER.h ../include/FPS_COUNTER.h

../include/libfps.h : libfps.h
	cp libfps.h ../include/libfps.h

else
ifeq "$(CFG)"  "libfps - Win32 Debug"

../include/FPS_COUNTER.h : FPS_COUNTER.h
	cp FPS_COUNTER.h ../include/FPS_COUNTER.h

../include/libfps.h : libfps.h 
	cp libfps.h ../include/libfps.h

endif
endif
HEADER_FILES= \
	FPS_COUNTER.h \
	libfps.h

CUSTOM_OUTPUTS= \
	../include/libfps.h \
	../include/FPS_COUNTER.h 

SRCS=$(SOURCE_FILES) $(HEADER_FILES) 

OBJS=$(patsubst %.rc,%.res,$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.c %.cc %.cpp %.cxx %.rc,$(SRCS)))))))

$(TARGET): $(OBJS) $(CUSTOM_OUTPUTS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET) $(CUSTOM_OUTPUTS) libfps.dep

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > libfps.dep

-include libfps.dep

