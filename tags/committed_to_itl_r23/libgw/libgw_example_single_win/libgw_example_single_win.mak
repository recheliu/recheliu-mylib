# Makefile - libgw_example_single_win.dsp

ProjectDir=.
ProjectName=libgw_example_single_win
ConfigurationName=Debug

ifndef CFG
CFG=libgw_example_single_win - Win32 Debug
endif
CC=gcc
CFLAGS=
# -I . -I ~/lib/include -I ~/lib/include/opencv -I ~/mylib/include -I /usr/local/cuda/include
CXX=g++
CXXFLAGS=$(CFLAGS)
RC=windres -O COFF
ifeq "$(CFG)"  "libgw_example_single_win - Win32 Debug"
CFLAGS+=-g -W -O0 -D_DEBUG -D_CONSOLE -D_UNICODE
LD=$(CXX) $(CXXFLAGS)
LDFLAGS=
LDFLAGS+=-Wl
LIBS+=-lGL -lGLU -lglut -L ~/lib/lib -lcv -lcvaux -lcxcore -lhighgui -L ~/lib/lib64 -lGLEW -llibgw -L ~/lib/lib -lglui -L ~/mylib/lib/Debug -llibfps 
else
ifeq "$(CFG)"  "libgw_example_single_win - Win32 Release"
CFLAGS+=-g -W -DNDEBUG -D_CONSOLE -D"_UNICODE" PRECOMP_VC7_TOBEREMOVED
LD=$(CXX) $(CXXFLAGS)
LDFLAGS=
LDFLAGS+=-Wl,--subsystem,console
LIBS+=-lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32
endif
endif

ifndef TARGET
TARGET=libgw_example_single_win
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
	libgw_example.cpp

HEADER_FILES=

RESOURCE_FILES=

SRCS=$(SOURCE_FILES) $(HEADER_FILES) $(RESOURCE_FILES) 

OBJS=$(patsubst %.rc,%.res,$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.c %.cc %.cpp %.cxx %.rc,$(SRCS)))))))

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)
#	ln -s $@ $@.exe

.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET) libgw_example_single_win.dep

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > libgw_example_single_win.dep

-include libgw_example_single_win.dep

