# -*- mode: Makefile; -*-
# -----------------------------------------
# project des_psml


export PATH := /opt/wx/2.8/bin:$(PATH)
export LD_LIBRARY_PATH := /opt/wx/2.8/lib:$(LD_LIBRARY_PATH)

_WX = /home/gr/projects/gui/codeblocks/wx
_WX.LIB = $(_WX)/lib
_WX.INCLUDE = $(_WX)/include

_CB = /home/gr/projects/gui/codeblocks/cb/src
_CB.INCLUDE = $(_CB)/include
_CB.LIB = $(_CB)/devel



CFLAGS_C = $(filter-out -include "sdk.h",$(CFLAGS))

# -----------------------------------------

# MAKE_DEP = -MMD -MT $@ -MF $(@:.o=.d)

CFLAGS = 
INCLUDES = 
LDFLAGS =  -s
RCFLAGS = 
LDLIBS = $(T_LDLIBS)  -lstdc++

LINK_exe = gcc -o $@ $^ $(LDFLAGS) $(LDLIBS)
LINK_con = gcc -o $@ $^ $(LDFLAGS) $(LDLIBS)
LINK_dll = gcc -o $@ $^ $(LDFLAGS) $(LDLIBS) -shared
LINK_lib = rm -f $@ && ar rcs $@ $^
COMPILE_c = gcc $(CFLAGS_C) -o $@ -c $< $(MAKEDEP) $(INCLUDES)
COMPILE_cpp = g++ $(CFLAGS) -o $@ -c $< $(MAKEDEP) $(INCLUDES)
COMPILE_rc = windres $(RCFLAGS) -J rc -O coff -i $< -o $@ -I$(dir $<)

%.o : %.c ; $(COMPILE_c)
%.o : %.cpp ; $(COMPILE_cpp)
%.o : %.cxx ; $(COMPILE_cpp)
%.o : %.rc ; $(COMPILE_rc)
.SUFFIXES: .o .d .c .cpp .cxx .rc

all: all.before all.targets all.after

all.before :

all.after : $(FIRST_TARGET)

all.targets : Debug_target 

clean :
	rm -fv $(clean.OBJ)
	rm -fv $(DEP_FILES)

.PHONY: all clean distclean

# -----------------------------------------
# Debug_target

Debug_target.BIN = bin/Debug/tests_psml.exe
Debug_target.OBJ = main_psml3.o 
DEP_FILES += main_psml3.d 
clean.OBJ += $(Debug_target.BIN) $(Debug_target.OBJ)

Debug_target : Debug_target.before $(Debug_target.BIN) Debug_target.after_always
Debug_target : CFLAGS += -fomit-frame-pointer -O3 -w -march=native  -Os
Debug_target : INCLUDES += -I../../../../Parvicursor
#-I/root/devel/systemc-2.3.1/src 
Debug_target : RCFLAGS += -I../../../../Parvicursor 
Debug_target : LDFLAGS += -s -lpthread -luuid -ldl -lcryptopp -L../../../../psml/bin/Debug/ -L../../../../xSim2Lib/bin/Debug/ -L../../../../ParvicursorLib/bin/Debug/ -L../../../../Parvicursor/System.Threading/Thread/
#-lsystemc -L/root/devel/systemc-2.3.1/buildtree/bin/lib-linux64  
Debug_target : T_LDLIBS = ../../../../psml/bin/Debug/psml.lib ../../../../xSim2Lib/bin/Debug/xSim2Lib.lib ../../../../ParvicursorLib/bin/Debug/ParvicursorLib.lib 
ifdef LMAKE
Debug_target : CFLAGS -= -O1 -O2 -g -pipe
endif

Debug_target.before :


Debug_target.after_always : $(Debug_target.BIN)

$(Debug_target.BIN) : $(Debug_target.OBJ)
	$(LINK_con)


# -----------------------------------------
ifdef MAKE_DEP
-include $(DEP_FILES)
endif

