/*
Huffman Encoding - Decoding 1.0
Anparasan ANPUKKODY
L2-A / 23000857 / UP8
13/12/2024
*/

#include "./Headers/huffman_t.h"
#include "./Headers/read_write_file.h"
#include "./Headers/auxiliary.h"


int main(){
    int choiceED;

    printf("Huffman Encoding - Decoding 1.0 / Anparasan ANPUKKODY - 2024\n\n");
    printf("Enter a valid filename that you wish to encode.\n\
If you wish to decode a file, you must input a .huff file\n\
that was generated using this program.\n\
/!\\ An incompatible .huff file may result in a program crash. /!\\\n\
Max : 32 characters.\n");

    char fileName[32];
    scanf("%32s", fileName);

    int actionHuffman = getAction(fileName);

    FILE *fptr = openFile(fileName);

    int nbChars;

    int nbNodes;

    huff_Nodes *CharFreqArray;

    huff_Nodes *backup = NULL;

    huff_Nodes tree;

    if(actionHuffman == COMPRESSION){

        printf("Action : compressing file.\n");
        
        int *count = countSymbols(fptr);
        
        nbChars = getNumberCharactersComp(count);

        nbNodes = getNumberNodes(nbChars);

        CharFreqArray = createCharFreqArray(count, nbChars, nbNodes);

        backup = createBackup(CharFreqArray, nbChars);
        
        tree = createTree(&CharFreqArray, nbChars, nbNodes);

        free(count);

        fileCompression(fptr, &tree, backup, nbChars);

    } else {

        printf("Action : decompressing file.\n");

        nbChars = getNumberCharactersDecomp(fptr);

        nbNodes = getNumberNodes(nbChars);

        CharFreqArray = createCharFreqArrayDecomp(fptr, nbChars, nbNodes);

        tree = createTree(&CharFreqArray, nbChars, nbNodes);

        fileDecompression(fptr, tree);

    }

    if(fptr){
        fclose(fptr);
        fptr = NULL;
    }
    
    for(int i = 0; i<nbNodes; ++i){
        if(CharFreqArray[i].characters != NULL)
            free(CharFreqArray[i].characters);
    }
   
    free(CharFreqArray);
    
    if(backup != NULL){
        free(backup);
    }

    printf("Exiting Huffman Encoding - Decoding 1.0 / Anparasan ANPUKKODY - 2024\n\n");

    return 0;
}
