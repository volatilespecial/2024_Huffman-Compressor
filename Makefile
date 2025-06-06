# Makefile
# Author : Anparasan ANPUKKODY
# Date : 13/12/2024

CC = gcc
RM = rm -f
SRCDIR = ./Sources
HDRDIR = ./Headers

MAIN = ./main.c
SOURCES = $(wildcard $(SRCDIR)/*.c)
HEADERS = $(wildcard $(HDRDIR)/*.h)
TARGET = ./Huffman.exe

all : $(TARGET)
$(TARGET): $(SOURCES) $(HEADERS) $(MAIN)
	$(CC) $(MAIN) $(SOURCES) -o $(TARGET)


clean : $(TARGET)
	$(RM) $(TARGET)

