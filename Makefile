# Temporary makefile since I can't seem to get Raylib's official makefile to work properly.
# Before using, make sure you have a "bin" folder in the project directory.
# WARNING: DOES NOT WORK WHEN USING MINGW ON WINDOWS OR WHEN USING CLANG/TCC

CC = gcc
ifeq ($(findstring cmd.exe,$(SHELL)),cmd.exe)
	/ = \\#
	RM = del
else
	/ = /
	RM = rm
endif
SOURCE = src$/main.c
OUTPUT = bin$/RaylibSortingVisualizer.exe
GENERIC_COMMAND = ${CC} ${SOURCE} -o ${OUTPUT} -Iinclude -Llib -lraylib -lopengl32 -lgdi32 -lwinmm -pthread
RM = del

prod:
	${GENERIC_COMMAND} -O2
all:
	prod
debug:
	${GENERIC_COMMAND} -g
clean:
	${RM} bin$/main.exe
	${RM} bin$/main.ilk
	${RM} bin$/main.pdb
