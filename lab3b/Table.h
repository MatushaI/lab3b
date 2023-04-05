#ifndef Table_h
#define Table_h

#include <stdio.h>

typedef struct KeySpace {
    unsigned long long latestVersionOffset;
    int key;
} KeySpace;

typedef struct Table {
    unsigned long long maxOffset;
    int maxSize;
    int size;
    FILE *file;
    KeySpace *ks;
} Table;

typedef struct Node {
    unsigned long long infoOffset;
    unsigned long long nextOffset;
    int version;
    int lenght;
    
} Node;

Table *importTable(FILE *file);
int searchPlace(Table *table, int key);
void printTable(Table *table);
int addInfo(Table *table, int key, char *info);
int deleteKey(Table *table, int key);
int deleteKeyVersion(Table *table, int key, int version);

#endif
