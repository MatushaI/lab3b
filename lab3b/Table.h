#ifndef Table_h
#define Table_h

#include <stdio.h>

typedef struct KeySpace {
    int key;
    unsigned long long latestVersionOffset;
} KeySpace;

typedef struct Table {
    int maxSize;
    int size;
    unsigned long long maxOffset;
    FILE *file;
    KeySpace *ks;
} Table;

typedef struct Node {
    int version;
    unsigned long long infoOffset;
    int lenght;
    unsigned long long nextOffset;
} Node;

#endif
