#include "../Headers/huffman_t.h"


huff_Nodes *createCharFreqArray(int *count, int nbChars, int nbNodes){
    huff_Nodes* CharFreqArray = malloc(nbNodes * sizeof(huff_Nodes));
    if(CharFreqArray == NULL){
        printf("FATAL ERROR - Memory allocation failed!\n");
        exit(2);
    }

    for(int i = 0; i<nbChars; ++i){
        CharFreqArray[i].characters = malloc(2 * sizeof(char));
        if(CharFreqArray[i].characters == NULL){
            printf("FATAL ERROR - Memory allocation failed!\n");
            exit(2);
        }
    }
    
    int i = 0;
    for(int k = 0; k < 256; ++k){
        if(count[k] != 0){
            CharFreqArray[i].characters[0] = k;
            CharFreqArray[i].characters[1] = '\0';
            CharFreqArray[i].frequency = count[k];
            CharFreqArray[i].nbCharsContained = 1;
            CharFreqArray[i].left = NULL;
            CharFreqArray[i].right = NULL;
            ++i;
        }
    }

    for(int j = i; j < nbNodes; ++j){
        CharFreqArray[j].frequency = INT_MAX;
    }

    qsort(CharFreqArray, i, sizeof(huff_Nodes), compareByFrequency);

    return CharFreqArray;
}


huff_Nodes *createBackup(huff_Nodes *CharFreqArray, int nbChars){
    huff_Nodes *backup = malloc(nbChars * sizeof(huff_Nodes));
    if(backup == NULL){
        printf("FATAL ERROR - Memory allocation failed!\n");
        exit(2);
    }
    for(int i = 0; i<nbChars; ++i){
        backup[i] = CharFreqArray[i];
    }

    return backup;
}


huff_Nodes createTree(huff_Nodes **CharFreqArray, int nbChars, int nbNodes){

    huff_Nodes *first;
    huff_Nodes *second;
    huff_Nodes tree;

    for(int i = 0; i<nbNodes; i+=2){
        if(i == nbNodes - 1){
            tree = (*CharFreqArray)[i];
            break;
        }else{
            huff_Nodes *new = &(*CharFreqArray)[nbChars];
            
            first = &(*CharFreqArray)[i];
            second = &(*CharFreqArray)[i+1];
            new->frequency = first->frequency + second->frequency;
            new->nbCharsContained = first->nbCharsContained + second->nbCharsContained;
            new->characters = malloc((new->nbCharsContained + 1) * sizeof(char));
            if(new->characters == NULL){
                printf("FATAL ERROR - Memory allocation failed!\n");
                exit(2);
            }
            
            strcpy(new->characters, first->characters);
            strcat(new->characters, second->characters);
            new->left = first;
            new->right = second;

            insertSort(CharFreqArray, nbChars);
            ++nbChars;
        }
    }
    return tree;
}


huff_Nodes *createCharFreqArrayDecomp(FILE *fptr, int nbChars, int nbNodes){
    huff_Nodes *CharFreqArray = malloc(nbNodes * sizeof(huff_Nodes));
    if(CharFreqArray == NULL){
        printf("FATAL ERROR - Memory allocation failed!\n");
        exit(2);
    }
    for(int i = 0; i < nbChars; ++i){
        CharFreqArray[i].characters = malloc(2 * sizeof(char));
        if(CharFreqArray[i].characters == NULL){
            printf("FATAL ERROR - Memory allocation failed!\n");
            exit(2);
        }
        CharFreqArray[i].characters[1] = '\0';
        CharFreqArray[i].nbCharsContained = 1;
        CharFreqArray[i].frequency = 0;
        CharFreqArray[i].left = NULL;
        CharFreqArray[i].right = NULL;
    }
    for(int i = nbChars; i < nbNodes; ++i){
        CharFreqArray[i].frequency = INT_MAX;
        CharFreqArray[i].left = NULL;
        CharFreqArray[i].right = NULL;
    }

    char c;
    int i = 0;
    int res = 0;

    char check[4];
    for(int j = 0; j<4; ++j)
        check[j] = '\0';
    int specialChar = 1;
    int checkSpecialChar = 1;
    int numberField = 0;
    int code;
    while((c = fgetc(fptr)) != '\n'){

        if(checkSpecialChar){
            for(int j = 0; j<3; ++j){
                if(j == 0){
                    check[j] = c;
                }else{
                    check[j] = fgetc(fptr);
                }  
                if(j == 1 && check[j] == ':'){
                    specialChar = 0;
                    checkSpecialChar = 0;
                    numberField = 1;
                    c = fgetc(fptr);
                    break;
                }
                if(j >= 1 && specialChar){
                    code = matchAgainst(check);
                    if(code != -1){
                        CharFreqArray[i].characters[0] = code;
                        specialChar = 1;
                        checkSpecialChar = 0;
                        numberField = 1;
                        c = fgetc(fptr);
                        if(c != ':'){
                            printf("FATAL ERROR - Unexpected character encountered!\n");
                            exit(4);
                        }
                        c = fgetc(fptr);
                        break;
                    }
                    if(j>=2 && code == -1){
                        printf("FATAL ERROR - Unexpected character encountered!\n");
                        exit(4);
                    } 
                }
            }

        }

        if(!specialChar){
            CharFreqArray[i].characters[0] = check[0];
            specialChar = 1;
        }

        if(c == ';'){
            CharFreqArray[i].frequency = res/10;
            if(CharFreqArray[i].frequency <= 0){
                printf("FATAL ERROR - Unexpected error!\n");
                exit(5);
            }
            res = 0;
            ++i;
            specialChar = 1;
            checkSpecialChar = 1;
            numberField = 0;
            for(int j = 0; j<4; ++j){
                check[j] = '\0';
            }
        }

        if(numberField){
            if(c >= '0' && c <= '9'){
                res = (res + (c - '0')) * 10;
            }
        }
    }
    return CharFreqArray;
}


char *getBinaryCode(huff_Nodes * tree, char c){
    int code_size = 8;
    int i = 0;
    char *binaryCode = malloc(code_size * sizeof(char));
    if(binaryCode == NULL){
        printf("FATAL - Memory allocation failed!\n");
        exit(2);
    }
    binaryCode[0] = '\0';
    huff_Nodes * act = tree;
    int oneChar = 0;
    while(act->nbCharsContained != 1){
        oneChar = 1;
        if(i == code_size){
            code_size = code_size * 2;
            binaryCode = realloc(binaryCode, code_size);
            if(binaryCode == NULL){
                printf("FATAL ERROR - Memory allocation failed!\n");
                exit(2);
            }
            binaryCode[i] = '\0';
        }
        if(act->left != NULL && strchr(act->left->characters, c) != NULL){
            act = act->left;
            binaryCode[i] = '0';
        }else {
            act = act->right;
            binaryCode[i] = '1'; 
        }
        ++i;
        if(i != code_size){
            binaryCode[i + 1] = '\0';
        }
    }
    if(oneChar == 0){
        binaryCode[i] = '0';
        binaryCode[i+1] = '\0';
    }
    return binaryCode;
}