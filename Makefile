CC = gcc
SRC_DIR := Source
LIB_DIR := Libraries
OBJ_DIR := Object
ifeq ($(OS),Windows_NT)
	INCLUDE_PATH := ./Include $(shell dir /s /b /ad $(LIB_DIR))
	LIB_FILE := $(shell dir /s /b "$(LIB_DIR)\*.c")
	EXE_SUFFIX := $(EXE_SUFFIX)
	LINK_FLAGS = -lws2_32
else
	INCLUDE_PATH := ./Include $(shell find $(LIB_DIR) -type d)
	LIB_FILE := $(shell find $(LIB_DIR) -type f -name "*.c")
	EXE_SUFFIX := 
	LINK_FLAGS = 
endif
CC_INCLUDE_FLAGS = $(addprefix -I,$(INCLUDE_PATH))
CFLAGS = -O3 $(CC_INCLUDE_FLAGS)
DEBUG_FLAGS = $(CC_INCLUDE_FLAGS) -DDEBUG -g
SRC_FILE := $(wildcard $(SRC_DIR)/*.c)

.PHONY: all debug reload clean print

all: StudentManage$(EXE_SUFFIX)
debug: CFLAGS = $(DEBUG_FLAGS)
debug: all
reload: reload$(EXE_SUFFIX)

reload$(EXE_SUFFIX): $(addprefix $(OBJ_DIR)/,$(filter-out StudentManage.o StuMan_Server.o,$(patsubst %.c, %.o,$(notdir $(SRC_FILE))))) $(patsubst %.c, %.o,$(LIB_FILE))
	$(CC) $^ -o $@

StudentManage$(EXE_SUFFIX): $(addprefix $(OBJ_DIR)/,$(filter-out reload.o,$(patsubst %.c, %.o,$(notdir $(SRC_FILE))))) $(patsubst %.c, %.o,$(LIB_FILE))
	$(CC) $^ $(LINK_FLAGS) -o $@

DEPDIR := .deps
DEPFLAGS = -MT $@ -MMD -MF $(DEPDIR)/$(notdir $*).d

COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) -c

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(DEPDIR)/%.d | $(DEPDIR) $(OBJ_DIR)
	$(COMPILE.c) $< -o $@

$(LIB_DIR)/%.o : $(LIB_DIR)/%.c
	$(COMPILE.c) $< -o $@

$(DEPDIR) $(OBJ_DIR):
ifeq ($(OS),Windows_NT)
	@mkdir $@ >nul 2>&1
else
	@mkdir -p $@
endif

DEPFILES := $(patsubst %.c,$(DEPDIR)/%.d,$(notdir $(SRC_FILE)))
$(DEPFILES):
include $(wildcard $(DEPFILES))

clean :
ifeq ($(OS),Windows_NT)
	@del /s *.o *.exe *.bin > nul 2>&1
else
	@find . -type f -name "*.o" -delete;rm -f StudentManage reload *.bin __log.txt
endif
	

# only for makefile-debug purpose
print :
	$(info $(OS))
