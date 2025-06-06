#ifndef AUXILIARY_H
#define AUXILIARY_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "huffman_t.h"


extern char *specialChars[];

int getAction(char *fileName);

int *countSymbols(FILE* fptr);

int getNumberCharactersComp(int *count);

int getNumberNodes(int nbChars);

int getNumberCharactersDecomp(FILE *fptr);

void insertSort(huff_Nodes **CharFreqArray, int nbNodes);

int matchAgainst(char *toCompare);

void displayTree(huff_Nodes * tree);

int myStrlen(char *string);

int myStrcmp(char *str1, char *str2);

int compareByFrequency(const void* a, const void* b);

#endif