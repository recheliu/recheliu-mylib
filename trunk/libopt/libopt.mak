# Makefile - libopt.dsp

ProjectDir=.
ProjectName=libopt
ConfigurationName=Debug

ifndef CFG
CFG=libopt - Win32 Debug
endif
CC=gcc
CFLAGS=
CXX=g++
CXXFLAGS=$(CFLAGS)
RC=windres -O COFF
ifeq "$(CFG)"  "libopt - Win32 Debug"
CFLAGS+=-g -W -O0 -D_DEBUG -D_LIB -D_UNICODE
AR=ar
TARGET=../lib/$(ConfigurationName)/liblibopt.a
ARFLAGS=rus
else
ifeq "$(CFG)"  "libopt - Win32 Release"
CFLAGS+=-g -W -DNDEBUG -D_LIB -D"_UNICODE" PRECOMP_VC7_TOBEREMOVED
AR=ar
ARFLAGS=rus
endif
endif

ifndef TARGET
TARGET=libopt.a
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
	libopt.cpp

ifeq "$(CFG)"  "libopt - Win32 Debug"

BuildCmds= \
	cp libopt.h ../include/libopt.h

../include/libopt.h: libopt.h
	$(BuildCmds)

else
ifeq "$(CFG)"  "libopt - Win32 Release"

BuildCmds= \
	cp libopt.h ../include/libopt.h

../include/libopt.h: libopt.h
	$(BuildCmds)

endif
endif
HEADER_FILES= \
	libopt.h \
	$(InputPath) \
	$(InputPath)

CUSTOM_OUTPUTS= \
	../include/libopt.h \

RESOURCE_FILES=

SRCS=$(SOURCE_FILES) $(HEADER_FILES) $(RESOURCE_FILES)  \
	ReadMe.txt

OBJS=$(patsubst %.rc,%.res,$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.c %.cc %.cpp %.cxx %.rc,$(SRCS)))))))

$(TARGET): $(OBJS) $(CUSTOM_OUTPUTS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(CUSTOM_OUTPUTS) $(TARGET) libopt.dep

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > libopt.dep

-include libopt.dep

