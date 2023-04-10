#include <stdio.h>
#include <stdlib.h>
#include "TableHD.h"
#include "getFunctions.h"
#include "dialog.h"


int main(void) {
    
    FILE *file = importBinaryFile("Введите название файла, содержащего таблицу\n", "r+");
    if(file == NULL) {
        return 0;
    }
        
    TableHD *table = importTableHD(file);
    Table *search = tableCreate(1);
    
    const char *msgs[] = {"0. Выход", "1. Добавить элемент", "2. Поиск", "3. Удаление", "4. Печать таблицы", "5. Импорт из текстового файла"};
    const int Nmsgs = sizeof(msgs) / sizeof(msgs[0]);
    
    printKoala();
    
    int (*func[])(TableHD *, Table *) = {NULL, addInfo_In, search_In, delete_In, printTable_In, importFile};
    
    int rc;
    while((rc = dialog(msgs, Nmsgs, NULL))){
        if(!func[rc](table, search)) 
            break;
    }
    
    exportTableHD(table);
    clearTable(search);
    
    printf("\n*** Программа завершена ***\n");
    
    return 0;
}
