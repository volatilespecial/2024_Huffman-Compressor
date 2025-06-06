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
    while((char_read = fgetc(fileRead)) != EOF){
       char *tmp = getBinaryCode(tree, char_read);
       fprintf(fileWrite, "%s", tmp);
       free(tmp);

    }
    printf("File compression done!\nYou can find the file in the same folder as \"compressed.huff\".\n");
    if(fileWrite){
        fclose(fileWrite);
        fileWrite = NULL;
    }
}


void fileDecompression(FILE *fileRead, huff_Nodes tree){
    FILE *fileWrite = fopen("decompressed.txt", "w+");

    huff_Nodes *new = &tree;
    char c;

    printf("File decompression...\n");

    while((c = fgetc(fileRead)) != EOF){
        if(c == '0'){
            new = new->left;
            if(new->nbCharsContained == 1){
                fprintf(fileWrite, "%c", new->characters[0]);
                new = &tree;
            }
        }else{
            new = new->right;
            if(new->nbCharsContained == 1){
                fprintf(fileWrite, "%c", new->characters[0]);
                new = &tree;
            }
        }
    }

    printf("File decompression done!\nYou can find the file in the same folder as \"decompressed.txt\".\n");

    fclose(fileWrite);
}