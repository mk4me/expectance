
include ../../common.mk

PROJECT = generator
OUT_DIR = $(MAIN_DIR_FROM_PROJECT)/$(GENERATED_DIR)/$(PROJECT)

EXE_FILE = $(OUT_DIR)/$(PROJECT).exe

INCLUDE_PATH = 	\
	+C:/OpenSG/include  \
	+"C:/Program Files/Microsoft Platform SDK for Windows Server 2003 R2/Include" \
	+"../" \
	+"../engine/src"

INCLUDE_OPT = $(patsubst +%,-I%,$(INCLUDE_PATH))

LIB_PATH = \
	+C:/OpenSG/lib \
	+"../../bin/debug" \
	+../../$(GENERATED_DIR)/engine

LIB_PATH_OPT = $(patsubst +%,/LIBPATH:%,$(LIB_PATH))

DEFINES = -DWIN32


SRC_C = $(shell find src -name '*.c')
SRC_CPP = $(shell find src -name '*.cpp')
SRC_ALL = $(SRC_C) $(SRC_CPP)


OBJS = $(SRC_C:.c=.obj)
OBJS += $(SRC_CPP:.cpp=.obj)
OBJS_FILES = $(notdir $(OBJS))
OBJS_FILES_OUT = $(patsubst %,$(OUT_DIR)/%,$(OBJS_FILES)) 


#.SUFFIXES: .cpp .obj


#----------------------------
TEST = test.c abak.c
test:
	@echo $(OBJS_FILES_OUT)
#	set FILE := $(filter %.c,$(TEST))
#	if [ "$FILE" != "" ] ; then \
#		echo jest ; \
#	fi; \
#----------------------------

intro:
	@echo Building generator project ...

all: intro $(OUT_DIR) exe

exe: $(EXE_FILE)

$(OUT_DIR):
	if [ ! -d $@ ]; then mkdir -p $@; fi		

$(EXE_FILE): $(OBJS_FILES_OUT)
	cl $(OBJS_FILES_OUT) /link /OUT:"$(EXE_FILE)" $(LIB_PATH_OPT) cal3d_d.lib engine.lib glut32.lib glu32.lib opengl32.lib odbc32.lib odbccp32.lib cal3d_d.lib glew32d.lib kernel32.lib


# ---- NOTE that folowing dependency doesn't work - any source file change coauses compilation of all files
$(OUT_DIR)/%.obj: $(filter %$(@F:.obj=.c) %$(@F:.obj=.cpp),$(SRC_ALL))
	cl $(filter %$(@F:.obj=.c) %$(@F:.obj=.cpp),$(SRC_ALL)) -c $(INCLUDE_OPT) /Fo"$(OUT_DIR)/" /Fd"$(OUT_DIR)/" /EHsc  $(DEFINES) /nologo /Od /FD /RTC1 /MTd /W3  /ZI  /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS"


clean:
	rm -Rf $(OUT_DIR)
