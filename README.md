Huffman Encoding - Decoding 1.1
Anparasan ANPUKKODY - L2A - 23000857 - UP8
13/12/2024

Pour compiler le projet, tapez depuis le répertoire principal : 
$ make
Le fichier "Huffman.exe" sera créé.
Pour compiler manuellement, tapez dans le terminal :
$ gcc main.c Sources/auxiliary.c Sources/huffman_t.c Sources read_write_file.c -o Huffman.exe

Pour exécuter le projet, tapez :
$ ./Huffman.exe

Le programme vous demandera le nom du fichier à encoder ou décoder. Si l'extension du fichier est ".huff",
le fichier sera décodé, sinon le fichier sera encodé.

Si le fichier est à encoder, et qu'un fichier "compressed.huff" existe déjà, le programme vous demandera si
vous voulez le remplacer ou quitter.

Une fois l'encodage terminé, le fichier "compressed.huff" contient les données encodées du fichier d'origine.

Un fichier .huff ressemble au suivant :

2
h:10;o:3;
0101010001

La première ligne contient le nombre de caractères différents présents. 
La deuxième ligne contient les caractères avec leur nombre d'apparitions.
La troisième ligne contient le fichier d'origine encodé.


Le compresseur de Huffman repose sur un arbre, créé avec les noeuds des caractères avec les nombres d'apparitions
les plus faibles. Le code binaire est donc créé selon le parcours nécessaire pour aller au noeud du caractère recherché.
0 si on part à gauche, 1 si on part à droite dans l'arbre.
Ainsi la structure suivante est utilisée :
typedef struct huffman_nodes_t{
    char *characters;
    int frequency;
    int nbCharsContained;
    struct huffman_nodes_t *left;
    struct huffman_nodes_t *right;
} huff_Nodes;
La structure est définie dans le fichier "huffman_t.h".

-> char *characters contient les caractères que contient ses enfants, ce qui facilite la recherche du code binaire.
-> int frequency contient le nombre de fois qu'apparait ce caractère. Un noeud parent fait la somme de ses enfants.
-> int nbCharsContained contient le nombre de caractère que contient char *characters. Si nbCharsContained == 1,
c'est un noeud de base, qui n'a aucun enfant. Ces noeuds sont dans le fichier "compressed.huff", ligne 2.
-> struct huffman_nodes_t *left et *right sont respectivements des pointeurs vers l'enfant gauche et l'enfant droit.


Les caractères spéciaux entre 0 et 32 ne peuvent pas être imprimés dans le fichier "compressed.huff. 
C'est pour cela que auxiliary.c contient le tableau specialChars.
Si un caractère est spécial, c'est le string correspondant dans le tableau qui est imprimé dans le fichier. 
Voici le tableau :

char *specialChars[] = {
    "NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL", "BS", "HT", "LF",
    "VT", "FF", "CR", "SO", "SI", "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN",
    "ETB", "CAN", "EM", "SUB", "ESC", "FS", "GS", "RS", "US"
};

Par exemple, si on rencontre le caractère '\n' dans le fichier d'origine, on imprime "LF:10;..." dans le fichier "compressed.huff".


Explication des fonctions :

auxiliary.c
-> int getAction(char *fileName): si le fichier contient ".huff", c'est un fichier à décompresser donc retourne 1, 
sinon c'est un fichier à compresser donc retourne 0.

-> int *countSymbols(FILE *fptr): crée un tableau de 256 (Extended ASCII) int pour récupérer le nombre d'apparitions de chaque caractère.

-> int getNumberCharactersComp(int *count): récupère le nombre de caractères unique avec le tableau *count. Uniquement pour la compression.

-> int getNumberNodes(int nbChars): retourne le nombre de noeuds nécessaire pour l'arbre = 2 * nbChars - 1

-> int getNumberCharactersDecomp(FILE *fptr): retourne la première ligne du fichier .huff, qui est nbChars. Uniquement pour la décompression.

-> void insertSort(huff_Nodes **CharFreqArray, int nbNodes): cherche la bonne position selon le nombre d'apparitions du nouveau 
noeud dans le tableau CharFreqArray et l'insère à cette position.

-> int matchAgainst(char *toCompare): retourne la position du caractère spécial dans le tableau specialChars.

-> void displayTree(huff_Nodes *tree): affiche l'arbre selon le parcours infixe.

-> int myStrlen(char *string): fonction similaire à strlen. Renvoie la longueur d'une chaine de caractères.

-> int myStrcmp(char *str1, char *str2): fonction similaire à strcmp. Compare deux chaines de caractères si elles sont similaires.

-> int compareByFrequency(const void* a, const void* b): fonction pour qsort, compare le nombre d'apparitions de deux noeuds.


read_write_file.c
-> FILE* openFile(char *filename): ouvre le fichier en mode lecture.

-> void fileCompression(FILE* fileRead, huff_Nodes *tree, huff_Nodes *CharFreqArray, int nbChars): crée le fichier "compressed.huff"
et écrit les éléments de CharFreqArray, et ensuite récupère le code binaire avec l'arbre de chaque caractère du fichier d'origine.

-> void fileDecompression(FILE *fileRead, huff_Nodes tree): crée le fichier "decompressed.txt", récupère le caractère d'origine en parcourant
l'arbre. Si neoud->nbCharsContained == 1, alors c'est un noeud de base et donc est le caractère recherché.


huffman_t.c
-> huff_Nodes *createCharFreqArray(int *count, int nbChars, int nbNodes): crée un tableau de la structure huff_Nodes avec nbNodes qui est le nombre de 
noeuds de l'arbre. nbChars est le nombre de noeuds de base. Pour les premiers nbChars cases, 2 char sont alloués dynamiquement pour le caractère unique 
du texte, et le caractère '\0'. Avec *count, CharFreqArray est remplie si count[k] != 0, pour le reste, on remplit avec INT_MAX pour faciliter le tri. 
Un tri est donc effectué en comparant le nombre d'apparitions de chaque caractère. Le tri est effectué du faible nombre d'apparitions au plus élevé.

-> huff_Nodes *createBackup(huff_Nodes *CharFreqArray, int nbChars): crée un tableau de la structure huff_Nodes avec nbChars pour sauvegarder les noeuds de base.

-> huff_Nodes createTree(huff_Nodes **CharFreqArray, int nbChars, int nbNodes): crée l'arbre binaire. Les noeuds les plus faibles sont au début du tableau. 
huff_Nodes *first récupère le premier noeud, huff_Nodes *second récupère le deuxième noeud, huff_Nodes *new contient le nouveau noeud parent. 
Le nouveau noeud est ensuite placé à la bonne position selon le nombre d'apparitions. L'arbre sera donc la dernière case du tableau CharFreqArray.

-> huff_Nodes *createCharFreqArrayDecomp(FILE *fptr, int nbChars, int nbNodes): crée un tableau de la structure huff_Nodes avec nbNodes si c'est une décompression
d'un fichier. Vérifie si le caractère encodé est un caractère spécial ou non comme les \n ou \t. Seulement les noeuds de base sont remplis.

-> char *getBinaryCode(huff_Nodes * tree, char c): récupère le code binaire du caractère dans l'arbre. Recherche si le caractère est dans *characters du 
noeud gauche ou droit, si oui, le code prend '0' ou '1'. Continue tant que nbCharsContained n'est pas égale à 1.


Performances :
Le programme a été testé avec plusieurs fichiers, notamment un fichier texte de 6.18Mo, soit environ 6 488 666 caractères, et le programme fonctionne correctement, 
compression et décompression sans aucune fuite mémoire (vérifié avec Valgrind).
Temps de compression sur machine AMD Ryzen 5 3500 avec 8GO de RAM : 0.765625 seconde.
Temps de décompression sur machine AMD Ryzen 5 3500 avec 8GO de RAM : 0.328125 seconde.
