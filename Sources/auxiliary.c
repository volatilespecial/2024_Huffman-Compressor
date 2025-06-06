#include "../Headers/auxiliary.h"


char *specialChars[] = {
    "NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL", "BS", "HT", "LF",
    "VT", "FF", "CR", "SO", "SI", "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN",
    "ETB", "CAN", "EM", "SUB", "ESC", "FS", "GS", "RS", "US"
};


int getAction(char *fileName){
    char *dot = strrchr(fileName, '.');
    if(dot == NULL)
        return COMPRESSION;
    char *check = malloc(5 * sizeof(char));
    //char *check = malloc(4 * sizeof(char));
    if(check == NULL){
        printf("FATAL ERROR - Memory allocation failed!\n");
        exit(2);
    }
    ++dot;
    int cmpt = 0;
    int res = COMPRESSION;
    while(*dot != '\0'){
        if(cmpt >= 4){
            free(check);
            return COMPRESSION;
        }
        *(check + cmpt) = *dot;
        ++dot;
        ++cmpt;
    }
    *(check + cmpt) = '\0';
    if(myStrcmp(check, "huff") == 0)
        res = DECOMPRESSION;
    free(check);
    return res;
}


int *countSymbols(FILE* fptr){
    int *count = calloc(256, sizeof(int));
    if(count == NULL){
        printf("FATAL ERROR - Memory allocation failed!\n");
        exit(2);
    }

    int c;
    while((c = fgetc(fptr)) != EOF){
        if(c >= 256){
            printf("ERROR - Character not recognized!\n");
            exit(3);
        }
        count[c]++;
    }

    return count;
}


int getNumberCharactersComp(int *count){
    int nbChars = 0;

    for(int k = 0; k<256; ++k){
        if(count[k] != 0)
            ++nbChars;
    }

    return nbChars;
}


int getNumberNodes(int nbChars){
    return 2*nbChars - 1;
}


int getNumberCharactersDecomp(FILE *fptr){
    char line[32];
    fgets(line, 32, fptr);
    return atoi(line);
}


void insertSort(huff_Nodes **CharFreqArray, int nbNodes) {
    int i = nbNodes - 1;
    huff_Nodes tmp = (*CharFreqArray)[i];
    while (i > 0 && tmp.frequency < (*CharFreqArray)[i - 1].frequency) {
        (*CharFreqArray)[i] = (*CharFreqArray)[i - 1];
        --i;
    }
    (*CharFreqArray)[i] = tmp;
}


int matchAgainst(char *toCompare){
    for(int i = 0; i<32; ++i){
        char *tmp = specialChars[i];
        if(myStrcmp(toCompare, tmp) == 0)
            return i;
    }
    return -1;
}


void displayTree(huff_Nodes * tree){
    if(tree == NULL){
        printf("N \n");
        return;
    }
    
    displayTree(tree->left);
    printf("Char %s : %d times\n", tree->characters, tree->frequency);
    displayTree(tree->right);
}


int myStrlen(char *string){
    int res = 0;
    while(string[res] != '\0'){
        ++res;
    }
    return res;
}


int myStrcmp(char *str1, char *str2){
    if(!str1 || !str2){
        return 1;
    }
    while(*str1 != '\0' && *str2 != '\0'){
        if(*str1 != *str2)
            return 1;
        ++str1;
        ++str2;
    }
    return 0;
}


int compareByFrequency(const void* a, const void* b){
    return ((huff_Nodes*)a)->frequency - ((huff_Nodes*)b)->frequency;
}