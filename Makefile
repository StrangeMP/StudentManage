CC = gcc
CC_INCLUDE_FLAGS = -I./Include/
CC_FLAGS = -O2 $(CC_INCLUDE_FLAGS)
C_SRC_FILES = $(shell powershell -Command "Get-ChildItem -Filter "*.c" -Recurse -File -Name | %{$$_ -replace '\\','/'}")
C_OBJ_FILES = $(patsubst %.c, %.o, $(C_SRC_FILES))

target : main.exe
#	$(info $(C_SRC_FILES))


debug : CC_FLAGS += -DDEBUG -g
debug : target

main.exe : $(C_OBJ_FILES)
	$(CC) $^ C:\Windows\System32\ws2_32.dll -o $@

cJSON.o: cJSON.c Include/cJSON.h
	$(CC) $(CC_FLAGS) -c $<

main.o: main.c Include/StuMan_Import.h Include/StuMan_Server.h
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
 Include/StuMan_Benefit.h Include/cJSON.h Include/StuMan_Nouns.h Include/StuMan_Search.h
	$(CC) $(CC_FLAGS) -c $<

StuMan_Handler.o: StuMan_Handler.c Include/StuMan_Delete.h Include/StuMan_Export.h Include/StuMan_Student.h \
 Include/StuMan_Benefit.h Include/cJSON.h Include/StuMan_Import.h Include/StuMan_Nouns.h Include/StuMan_Memory.h \
 Include/StuMan_Search.h Include/StuMan_Statistics.h Include/VECTOR.h Include/cJSON.h
	$(CC) $(CC_FLAGS) -c $<

StuMan_Import.o: StuMan_Import.c Include/StuMan_Import.h Include/StuMan_BuildIndex.h Include/StuMan_Student.h \
 Include/StuMan_Benefit.h Include/StuMan_Memory.h Include/StuMan_Node.h \
 Include/StuMan_Nouns.h Include/StuMan_Search.h Include/StuMan_Statistics.h Include/cJSON.h
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

Server/StuMan_Server.o: Server/StuMan_Server.c Include/StuMan_Server.h \
 Include/StuMan_Handler.h Include/StuMan_Student.h \
 Include/StuMan_Benefit.h Include/tcpSocket.h
	$(CC) $(CC_FLAGS) -c $< -o $@

Server/fileTrans.o: Server/fileTrans.c Include/fileTrans.h Include/tcpSocket.h
	$(CC) $(CC_FLAGS) -c $< -o $@

Server/tcpSocket.o: Server/tcpSocket.c Include/tcpSocket.h
	$(CC) $(CC_FLAGS) -c $< -o $@

clean :
	del $(subst /,\\,$(C_OBJ_FILES)) main.exe