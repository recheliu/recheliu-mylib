# Makefile - shader2string.dsp

ProjectDir=.
ProjectName=shader2string
ConfigurationName=Debug

ifndef CFG
CFG=shader2string - Win32 Debug
endif
CC=gcc
CFLAGS=-I ~/mylib/include -I ~/lib/include
CXX=g++
CXXFLAGS=$(CFLAGS)
RC=windres -O COFF
ifeq "$(CFG)"  "shader2string - Win32 Debug"
CFLAGS+=-g -W -O0 -D_DEBUG -D_CONSOLE -D_UNICODE
LD=$(CXX) $(CXXFLAGS)
LDFLAGS=
TARGET=$(ProjectDir)/../../bin/$(ProjectName)
LDFLAGS+=-Wl
LIBS+=-L ~/lib/lib64 -lGLEW -L ~/mylib/lib/Debug -llibshader
else
ifeq "$(CFG)"  "shader2string - Win32 Release"
CFLAGS+=-g -W -DNDEBUG -D_CONSOLE -D"_UNICODE" PRECOMP_VC7_TOBEREMOVED
LD=$(CXX) $(CXXFLAGS)
LDFLAGS=
LDFLAGS+=-Wl,--subsystem,console
LIBS+=-lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32
endif
endif

ifndef TARGET
TARGET=shader2string
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
	shader2string.cpp

HEADER_FILES=

RESOURCE_FILES=

SRCS=$(SOURCE_FILES) $(HEADER_FILES) $(RESOURCE_FILES) 

OBJS=$(patsubst %.rc,%.res,$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.c %.cc %.cpp %.cxx %.rc,$(SRCS)))))))

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)
#	ln -s $@ $@.exe

.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET) shader2string.dep

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > shader2string.dep

-include shader2string.dep

