#include "../Headers/read_write_file.h"


FILE* openFile(char *filename){
    FILE *fptr;
    fptr = fopen(filename, "r");

    if(fptr == NULL){
        printf("FATAL ERROR - File couldn't be opened!\n");
        exit(1);
    }

    return fptr;
}


void fileCompression(FILE* fileRead, huff_Nodes *tree, huff_Nodes *CharFreqArray, int nbChars){
    FILE *fileWrite = fopen("compressed.huff", "r");

    int codeSize = 8;
    int codePosition = 0;
    int i = 0;
    int powerConv = 128;
    int toPrint = 0;

    char choice;

    int char_read;
    int char_write;

    if(fileWrite != NULL){
        fclose(fileWrite);
        printf("Warning : A file with the specified name already exists.\n");
        do{
            printf("Do you wish to replace the file? Y/N\n");
            scanf(" %c", &choice);
        } while(choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N');
        
        if(choice == 'n' || choice == 'N'){
            printf("File unmodified. Exiting...\n");
            exit(0);
        }
    }

    fileWrite = fopen("compressed.huff", "w+");

    printf("File compression...\n");

    rewind(fileRead);
    rewind(fileWrite);
    fprintf(fileWrite, "%d\n", nbChars);
    for(int i = 0; i<nbChars; ++i){
        fprintf(fileWrite, "%s:%d;", (CharFreqArray[i].characters[0] < 32 ? specialChars[CharFreqArray[i].characters[0]] : CharFreqArray[i].characters), CharFreqArray[i].frequency);
    }
    fprintf(fileWrite, "\n");

    char *currentBinaryFill = malloc(codeSize * sizeof(char));
    if(currentBinaryFill == NULL){
       printf("FATAL ERROR - Memory allocation failed!\n");
       exit(2);
    }

    char_read = fgetc(fileRead);

    while(char_read != EOF){
       char *tmp = getBinaryCode(tree, char_read);
       i = 0;
       
        while(tmp[i] != '\0'){
            if(codePosition < codeSize){
                currentBinaryFill[codePosition] = tmp[i];
                ++codePosition;
            } else {
                for(int j = 0; j<codeSize; ++j){
                    toPrint += (*(currentBinaryFill + j) - '0') * powerConv;
                    if(powerConv != 1) powerConv /= 2;
                }
                codePosition = 0;
                powerConv = 128;
                if(toPrint < 32) fprintf(fileWrite, "\n%s\n", specialChars[toPrint]);
                else fprintf(fileWrite, "%c", (char)toPrint);
                toPrint = 0;
                continue;
            }
            ++i;
        }

       char_read = fgetc(fileRead);

        if(codePosition != 0 && char_read == EOF){
            fprintf(fileWrite, "%c", '\n');
            for(int j = 0; j<codePosition; j++){
                fprintf(fileWrite, "%c", currentBinaryFill[j]);
            }
        }

        free(tmp);

    }
    printf("File compression done!\nYou can find the file in the same folder as \"compressed.huff\".\n");
    if(fileWrite){
        fclose(fileWrite);
        fileWrite = NULL;
    }
    if(currentBinaryFill){
        free(currentBinaryFill);
    }
}


void fileDecompression(FILE *fileRead, huff_Nodes tree){
    FILE *fileWrite = fopen("decompressed.txt", "w+");

    huff_Nodes *current_node = &tree;
    int c;

    printf("File decompression...\n");

    while((c = fgetc(fileRead)) != EOF){
        unsigned char byteToProcess;
        if(c == '\n'){
            char buf[32];
            if(fgets(buf, sizeof(buf), fileRead) != NULL){
                buf[strcspn(buf, "\r\n")] = 0;
                int specialVal = matchAgainst(buf);
                if(specialVal != -1){
                    byteToProcess = (unsigned char)specialVal;
                    for(int bit = 7; bit >= 0; --bit){
                        int bitVal = (byteToProcess >> bit) & 1;
                        current_node = (bitVal == 0) ? current_node->left : current_node->right;
                        if(current_node != NULL && current_node->nbCharsContained == 1){
                            fprintf(fileWrite, "%c", current_node->characters[0]);
                            current_node = &tree;
                        }
                    }
                } else {
                    for(int i = 0; buf[i] != '\0'; ++i){
                        current_node = (buf[i] == '0') ? current_node->left : current_node->right;
                        if(current_node != NULL && current_node->nbCharsContained == 1){
                            fprintf(fileWrite, "%c", current_node->characters[0]);
                            current_node = &tree;
                        }
                    }
                }
            }
        } else {
            byteToProcess = (unsigned char)c;
            for(int bit = 7; bit >= 0; --bit){
                int bitVal = (byteToProcess >> bit) & 1;
                current_node = (bitVal == 0) ? current_node->left : current_node->right;

                if(current_node != NULL && current_node->nbCharsContained == 1){
                    fprintf(fileWrite, "%c", current_node->characters[0]);
                    current_node = &tree;
                }
            }
        }
    }

    printf("File decompression done!\nYou can find the file in the same folder as \"decompressed.txt\".\n");

    fclose(fileWrite);
}
