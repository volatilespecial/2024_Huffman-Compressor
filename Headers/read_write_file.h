#ifndef READ_WRITE_FILE_H
#define READ_WRITE_FILE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "huffman_t.h"


FILE* openFile(char *filename);

void fileCompression(FILE* fileRead, huff_Nodes *tree, huff_Nodes *CharFreqArray, int nbChars);

void fileDecompression(FILE *fileRead, huff_Nodes tree);


#endif