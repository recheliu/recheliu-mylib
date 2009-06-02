# Makefile - FboWin.dsp

ProjectDir=.
ProjectName=libfbow
ConfigurationName=Debug

ifndef CFG
CFG=FboWin - Win32 Debug
endif
CC=gcc
CFLAGS=
CXX=g++
CXXFLAGS=$(CFLAGS)
RC=windres -O COFF
ifeq "$(CFG)"  "FboWin - Win32 Release"
CFLAGS+=-W -O2 -DNDEBUG -D_LIB -D_MBCS
AR=ar
ARFLAGS=rus
else
ifeq "$(CFG)"  "FboWin - Win32 Debug"
CFLAGS+=-g -W -O0 -D_DEBUG -D_LIB -D"_MBCS" PRECOMP_VC7_TOBEREMOVED
AR=ar
ARFLAGS=rus
endif
endif

ifndef TARGET
TARGET=libFboWin.a
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

ifeq "$(CFG)"  "FboWin - Win32 Release"

BuildCmds= \
	cp FboWin.h ../include/FboWin.h

../include/FboWin.h: FboWin.h
	$(BuildCmds)
else
ifeq "$(CFG)"  "FboWin - Win32 Debug"

BuildCmds= \
	cp FboWin.h ../include/FboWin.h

../include/FboWin.h: FboWin.h
	$(BuildCmds)
endif
endif
HEADER_FILES= \
	FboWin.h \
	$(InputPath) \
	$(InputPath)

CUSTOM_OUTPUTS= \
	../include/FboWin.h

SOURCE_FILES=

SRCS=$(HEADER_FILES) $(SOURCE_FILES) 

OBJS=$(patsubst %.rc,%.res,$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.c %.cc %.cpp %.cxx %.rc,$(SRCS)))))))

$(TARGET): $(OBJS) $(CUSTOM_OUTPUTS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(CUSTOM_OUTPUTS) $(TARGET) FboWin.dep

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > FboWin.dep

-include FboWin.dep

