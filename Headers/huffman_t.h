#ifndef HUFFMAN_T_H
#define HUFFMAN_T_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


#define COMPRESSION 0
#define DECOMPRESSION 1


typedef struct huffman_nodes_t{
    char *characters;
    int frequency;
    int nbCharsContained;
    struct huffman_nodes_t *left;
    struct huffman_nodes_t *right;
} huff_Nodes;


#include "auxiliary.h"


huff_Nodes *createCharFreqArray(int *count, int nbChars, int nbNodes);

huff_Nodes *createBackup(huff_Nodes *CharFreqArray, int nbChars);

huff_Nodes createTree(huff_Nodes **CharFreqArray, int nbChars, int nbNodes);

huff_Nodes *createCharFreqArrayDecomp(FILE *fptr, int nbChars, int nbNodes);

char *getBinaryCode(huff_Nodes * tree, char c);

#endif