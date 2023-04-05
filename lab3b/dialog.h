#ifndef dialog_h
#define dialog_h

#include <stdio.h>
#include "TableHD.h"

int dialog(const char *msgs[], int N, int *ifPagePrev);
int deleteKey_In(TableHD *table);
int deleteKeyVersion_In(TableHD *table);

int addInfo_In(TableHD *table, Table *searchTable);
int delete_In(TableHD *table, Table *search);
int search_In(TableHD *table, Table *search);
int printTable_In(TableHD *table, Table *search);

void printCat(void);
void printKoala(void);

#endif 
