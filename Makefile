CC = gcc
INCLUDE_PATH = ./Include $(shell powershell -Command "Get-ChildItem -Path Libraries -Directory -Recurse | ForEach-Object { Resolve-Path -Path $$_.FullName -Relative } | %{$$_ -replace '\\','/'}")
CC_INCLUDE_FLAGS = $(addprefix -I,$(INCLUDE_PATH))
CC_FLAGS = -O2 $(CC_INCLUDE_FLAGS)
DEBUG_FLAGS = $(CC_INCLUDE_FLAGS) -DDEBUG -g
C_SRC_FILES = StuMan_Benefit.c StuMan_Binary.c StuMan_BuildIndex.c StuMan_Log.o\
 StuMan_Delete.c StuMan_Export.c StuMan_Handler.c StuMan_Import.c StuMan_Memory.c\
 StuMan_Node.c StuMan_Nouns.c StuMan_Search.c StuMan_Statistics.c StuMan_Student.c\
 Libraries/cJSON/cJSON.c StuMan_Server.c StuMan_Account.c
C_OBJ_FILES = $(patsubst %.c, %.o, $(C_SRC_FILES))

#	@echo Project Successfully Built

# target :
#	$(info $(CC_INCLUDE_FLAGS))
#	@echo Project Successfully Built
.PHONY: all debug reload
all: main.exe
debug: CC_FLAGS = $(DEBUG_FLAGS)
debug: all
reload: reload.exe

main.exe: $(C_OBJ_FILES) main.o
	$(CC) $^ -lws2_32 -o $@

reload.exe: $(C_OBJ_FILES) reload.o
	$(CC) $^ -lws2_32 -o $@

Libraries/cJSON/cJSON.o: Libraries/cJSON/cJSON.c Libraries/cJSON/cJSON.h
	$(CC) $(CC_FLAGS) -c $< -o $@

main.o: main.c Include/StuMan_Import.h Include/StuMan_Server.h
	$(CC) $(CC_FLAGS) -c $<

reload.o: reload.c Include/StuMan_Binary.h Include/StuMan_Memory.h Include/StuMan_Import.h
	$(CC) $(CC_FLAGS) -c $<

StuMan_Log.o: StuMan_Log.c Libraries/cJSON/cJSON.h Include/StuMan_Log.h
	$(CC) $(CC_FLAGS) -c $<

StuMan_Benefit.o: StuMan_Benefit.c Include/StuMan_Benefit.h Include/StuMan_Student.h\
  Include/StuMan_Search.h Include/StuMan_Node.h Include/StuMan_Import.h
	$(CC) $(CC_FLAGS) -c $<

StuMan_Binary.o: StuMan_Binary.c Include/StuMan_Binary.h \
 Include/StuMan_Memory.h Include/StuMan_Student.h Include/StuMan_Benefit.h
	$(CC) $(CC_FLAGS) -c $<

StuMan_BuildIndex.o: StuMan_BuildIndex.c Include/StuMan_BuildIndex.h Include/StuMan_Student.h\
 Include/StuMan_Benefit.h Include/StuMan_Node.h Include/StuMan_Search.h
	$(CC) $(CC_FLAGS) -c $<

StuMan_Delete.o: StuMan_Delete.c Include/StuMan_Delete.h Include/StuMan_Student.h \
 Include/StuMan_Benefit.h Include/StuMan_Search.h
	$(CC) $(CC_FLAGS) -c $<

StuMan_Export.o: StuMan_Export.c Include/StuMan_Export.h Include/StuMan_Student.h \
 Include/StuMan_Benefit.h Libraries/cJSON/cJSON.o Include/StuMan_Nouns.h Include/StuMan_Search.h
	$(CC) $(CC_FLAGS) -c $<

StuMan_Handler.o: StuMan_Handler.c Include/StuMan_Delete.h Include/StuMan_Export.h Include/StuMan_Student.h \
 Include/StuMan_Benefit.h Libraries/cJSON/cJSON.o Include/StuMan_Import.h Include/StuMan_Nouns.h Include/StuMan_Memory.h \
 Include/StuMan_Search.h Include/StuMan_Statistics.h Include/VECTOR.h Libraries/cJSON/cJSON.o
	$(CC) $(CC_FLAGS) -c $<

StuMan_Import.o: StuMan_Import.c Include/StuMan_Import.h Include/StuMan_BuildIndex.h Include/StuMan_Student.h \
 Include/StuMan_Benefit.h Include/StuMan_Memory.h Include/StuMan_Node.h \
 Include/StuMan_Nouns.h Include/StuMan_Search.h Include/StuMan_Statistics.h Libraries/cJSON/cJSON.o
	$(CC) $(CC_FLAGS) -c $<

StuMan_Memory.o: StuMan_Memory.c Include/StuMan_Memory.h
	$(CC) $(CC_FLAGS) -c $<

StuMan_Node.o: StuMan_Node.c Include/StuMan_Memory.h Include/StuMan_Node.h\
 Include/StuMan_Student.h Include/StuMan_Benefit.h
	$(CC) $(CC_FLAGS) -c $<

StuMan_Nouns.o: StuMan_Nouns.c Include/StuMan_Nouns.h
	$(CC) $(CC_FLAGS) -c $<

StuMan_Search.o: StuMan_Search.c Include/StuMan_Search.h \
 Include/StuMan_Student.h Include/StuMan_Benefit.h
	$(CC) $(CC_FLAGS) -c $<

StuMan_Statistics.o: StuMan_Statistics.c Include/StuMan_Statistics.h \
 Include/StuMan_Student.h Include/StuMan_Benefit.h Include/StuMan_Nouns.h \
 Include/StuMan_Search.h Include/VECTOR.h
	$(CC) $(CC_FLAGS) -c $<

StuMan_Student.o: StuMan_Student.c Include/StuMan_Student.h \
 Include/StuMan_Benefit.h Include/StuMan_Search.h
	$(CC) $(CC_FLAGS) -c $<

StuMan_Server.o: StuMan_Server.c Include/StuMan_Server.h \
 Include/StuMan_Handler.h Include/StuMan_Memory.h
	$(CC) $(CC_FLAGS) -c $< -o $@

StuMan_Account.o: StuMan_Account.c Include/StuMan_Account.h \
 Include/VECTOR.h Libraries/cJSON/cJSON.h Include/StuMan_Search.h \
 Include/StuMan_Student.h Include/StuMan_Benefit.h
	$(CC) $(CC_FLAGS) -c $< -o $@


clean :
	@powershell -Command "Get-ChildItem -Path . -Include *.o,*.exe -File -Recurse | Remove-Item -Force"