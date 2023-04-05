#include <stdio.h>
#include <stdlib.h>
#include "TableHD.h"
#include "getFunctions.h"


int main(void) {
    /*
        FILE *file = getFile("Введите название файла с таблицей\n", "w+r", NULL);
        if(file == NULL) {
            printf("error\n");
        }
        unsigned long long test = 184;
        fwrite(&test, sizeof(unsigned long long), 1, file);
        test = 10;
        fwrite(&test, sizeof(unsigned long long), 1, file);
        test = 0;
        fwrite(&test, sizeof(unsigned long long), 1, file);
    
    
        KeySpace *ks = calloc(10, sizeof(KeySpace));
    
        for (int i = 0; i < 10; i++) {
            ks[i].key = i*i;
            ks[i].latestVersionOffset = 0;
            ks[i].node = NULL;
        }
        
        fwrite(ks, sizeof(KeySpace), 10, file);
    
         */
        
        FILE *file = getFile("Введите название файла с таблицей\n", "r+", NULL);
        TableHD *table = importTableHD(file);
        
        printTableHD(table);
        printf("==========\n");
        addInfoHD(table, 20, "123456789");
        addInfoHD(table, 20, "123456789");
        printf("==========\n");
        printTableHD(table);
        deleteKeyVersionHD(table, 20, 1);
        
        
        //deleteKeyHD(table, 20);
        printf("==========\n");
        printTableHD(table);
    
        
        Table *search = tableCreate(1);
        searchKeyVersionDialog(table, search, 20, 200);
        printTable(search);
    
        exportTableHD(table);
        clearTable(search);
        
         
        
    
    return 0;
}
