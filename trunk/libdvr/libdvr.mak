# Makefile - libdvr.dsp

ProjectDir=.
ProjectName=libdvr
ConfigurationName=Debug

ifndef CFG
CFG=libdvr - Win32 Debug
endif
CC=gcc
CFLAGS=
CXX=g++
CXXFLAGS=$(CFLAGS)
RC=windres -O COFF
ifeq "$(CFG)"  "libdvr - Win32 Release"
CFLAGS+=-fexceptions -O2 -DNDEBUG -D_MBCS -D_LIB -W
TARGET=../lib/$(ConfigurationName)/liblibdvr.a
AR=ar
ARFLAGS=rus
else
ifeq "$(CFG)"  "libdvr - Win32 Debug"
CFLAGS+=-fexceptions -g -O0 -D_DEBUG -D_MBCS -D_LIB -W
TARGET=../lib/$(ConfigurationName)/liblibdvr.a
AR=ar
ARFLAGS=rus
endif
endif

ifndef TARGET
TARGET=liblibdvr.a
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

../include/libdvr/DvrWin.h : DvrWin.h 
	cp DvrWin.h ../include/libdvr/DvrWin.h

tf_1d.frag.h : tf_1d.frag
	shader2string tf_1d.frag tf_1d.frag.h

SOURCE_FILES= \
	DvrWin.cpp \

HEADER_FILES= \
	DvrWin.h

CUSTOM_OUTPUTS= \
	../include/libdvr/DvrWin.h \
	tf_1d.frag.h

DvrWin.o: DvrWin.cpp DvrWin.h tf_1d.frag.h

SRCS=$(SOURCE_FILES) $(HEADER_FILES) 

OBJS=$(patsubst %.rc,%.res,$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.c %.cc %.cpp %.cxx %.rc,$(SRCS)))))))

$(TARGET): $(OBJS) $(CUSTOM_OUTPUTS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET) $(CUSTOM_OUTPUTS) libdvr.dep

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > libdvr.dep

-include libdvr.dep

