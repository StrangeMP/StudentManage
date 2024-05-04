CC = gcc
SRC_DIR := Source
LIB_DIR := Libraries
OBJ_DIR := Object
INCLUDE_PATH := ./Include $(shell dir /s /b /ad $(LIB_DIR))
CC_INCLUDE_FLAGS = $(addprefix -I,$(INCLUDE_PATH))
CFLAGS = -O3 $(CC_INCLUDE_FLAGS)
DEBUG_FLAGS = $(CC_INCLUDE_FLAGS) -DDEBUG -g
SRC_FILE := $(wildcard $(SRC_DIR)/*.c)
LIB_FILE := $(shell dir /s /b "$(LIB_DIR)\*.c")

.PHONY: all debug reload clean print

all: StudentManage.exe
debug: CFLAGS = $(DEBUG_FLAGS)
debug: all
reload: reload.exe

reload.exe: $(addprefix $(OBJ_DIR)/,$(filter-out StudentManage.o StuMan_Server.o,$(patsubst %.c, %.o,$(notdir $(SRC_FILE))))) $(patsubst %.c, %.o,$(LIB_FILE))
	$(CC) $^ -o $@

StudentManage.exe: $(addprefix $(OBJ_DIR)/,$(filter-out reload.o,$(patsubst %.c, %.o,$(notdir $(SRC_FILE))))) $(patsubst %.c, %.o,$(LIB_FILE))
	$(CC) $^ -lws2_32 -o $@

DEPDIR := .deps
DEPFLAGS = -MT $@ -MMD -MF $(DEPDIR)/$(notdir $*).d

COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) -c

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(DEPDIR)/%.d | $(DEPDIR) $(OBJ_DIR)
	$(COMPILE.c) $< -o $@

$(LIB_DIR)/%.o : $(LIB_DIR)/%.c
	$(COMPILE.c) $< -o $@

$(DEPDIR): ; @mkdir $@ >nul 2>&1

$(OBJ_DIR): ; @mkdir $@ >nul 2>&1

DEPFILES := $(patsubst %.c,$(DEPDIR)/%.d,$(notdir $(SRC_FILE)))
$(DEPFILES):
include $(wildcard $(DEPFILES))

clean :
	@del /s *.o > nul 2>&1

# only for makefile-debug purpose
print :
	$(info $(LIB_FILE))
