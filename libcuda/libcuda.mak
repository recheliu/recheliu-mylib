# Makefile - libcuda.dsp

ConfigurationName=Debug

ifndef CFG
CFG=libcuda - Win32 Debug
endif
CC=gcc
CFLAGS=
CXX=g++
CXXFLAGS=$(CFLAGS)
RC=windres -O COFF
ifeq "$(CFG)"  "libcuda - Win32 Release"
CFLAGS+=-W -O2 -DWIN32 -DNDEBUG -D_LIB -D_MBCS
AR=ar
ARFLAGS=rus
else
ifeq "$(CFG)"  "libcuda - Win32 Debug"
CFLAGS+=-g -W -O0 -DWIN32 -D_DEBUG -D_LIB -D"_MBCS" PRECOMP_VC7_TOBEREMOVED
AR=ar
ARFLAGS=rus
endif
endif

ifndef TARGET
TARGET=liblibcuda.a
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

ifeq "$(CFG)"  "libcuda - Win32 Release"

BuildCmds= \
	cp cuda_macro.h ../include/cuda_macro.h


../include/cuda_macro.h: cuda_macro.h
	$(BuildCmds)
else
ifeq "$(CFG)"  "libcuda - Win32 Debug"

BuildCmds= \
	cp cuda_macro.h ../include/cuda_macro.h


../include/cuda_macro.h: cuda_macro.h
	$(BuildCmds)
endif
endif
HEADER_FILES= \
	cuda_macro.h \
	$(InputPath) \
	$(InputPath)

CUSTOM_OUTPUTS= \
	../include/cuda_macro.h

SOURCE_FILES=

SRCS=$(HEADER_FILES) $(SOURCE_FILES) 

OBJS=$(patsubst %.rc,%.res,$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.c %.cc %.cpp %.cxx %.rc,$(SRCS)))))))

$(TARGET): $(OBJS) $(CUSTOM_OUTPUTS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET) $(CUSTOM_OUTPUTS) libcuda.dep

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > libcuda.dep

-include libcuda.dep

