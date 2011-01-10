# Makefile - libclock.dsp

ProjectName=libclock
ProjectDir=.
ConfigurationName=Debug

ifndef CFG
CFG=$(ProjectName) - Win32 Debug
endif
CC=gcc
CFLAGS=
CXX=g++
CXXFLAGS=$(CFLAGS)
RC=windres -O COFF
ifeq "$(CFG)"  "$(ProjectName) - Win32 Release"
CFLAGS+=-W -O2 -DNDEBUG -D_LIB -D_MBCS
AR=ar
ARFLAGS=rus
else
ifeq "$(CFG)"  "$(ProjectName) - Win32 Debug"
CFLAGS+=-g -W -O0 -D_DEBUG -D_LIB -D"_MBCS" PRECOMP_VC7_TOBEREMOVED
AR=ar
ARFLAGS=rus
endif
endif

ifndef TARGET
TARGET=lib$(ProjectName).a
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

ifeq "$(CFG)"  "$(ProjectName) - Win32 Release"

BuildCmds= \
	cp $(ProjectName).h ../include/$(ProjectName).h

../include/$(ProjectName).h: $(ProjectName).h
	$(BuildCmds)
else
ifeq "$(CFG)"  "$(ProjectName) - Win32 Debug"

BuildCmds= \
	cp $(ProjectName).h ../include/$(ProjectName).h

../include/$(ProjectName).h: $(ProjectName).h
	$(BuildCmds)
endif
endif
HEADER_FILES= \
	$(ProjectName).h 

CUSTOM_OUTPUTS= \
	../include/$(ProjectName).h

SOURCE_FILES=

SRCS=$(HEADER_FILES) $(SOURCE_FILES) 

OBJS=$(patsubst %.rc,%.res,$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.c %.cc %.cpp %.cxx %.rc,$(SRCS)))))))

$(TARGET): $(OBJS) $(CUSTOM_OUTPUTS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(CUSTOM_OUTPUTS) $(TARGET) $(ProjectName).dep

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > $(ProjectName).dep

-include $(ProjectName).dep

#
# $Log: not supported by cvs2svn $
#

