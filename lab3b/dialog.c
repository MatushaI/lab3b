#include "dialog.h"
#include "getFunctions.h"
#include <stdlib.h>
#include <stdio.h>
#include "TableHD.h"
#include <time.h>
#include <unistd.h>

int dialog(const char *msgs[], int N, int *ifPagePrev) {
    
    char *errorMessage = "";
    int n = 0;
    char *string = NULL;
    
    do {
        printf("%s\n", errorMessage);
        
        for (int i = 0; i < N; i++) {
            printf("%s\n", msgs[i]);
        }
        
        string = myreadline("> ");
        
        if(string == NULL) {
            if(ifPagePrev != NULL) {
                *ifPagePrev = -1;
            }
            return 0;
        }
        
        n = getIntUnsignt(string);
        errorMessage = "\n(!) Пункт меню не найден. Попробуйте ещё раз\n";
        free(string);
        
    } while (n < 0 || n >= N);
    
    return n;
}

int addInfo_In(TableHD *table, Table *searchTable) {
    const char *msgs[] = {"0. В основное меню", "1. Вставить элемент"};
    char *errorMessage = "";
    int checkZero = 0;
    char *string = NULL;
    int n = dialog(msgs, sizeof(msgs) / sizeof(msgs[0]), &checkZero);
    
    if(checkZero == -1) {
        return 0;
    }
    
    if(n == 0) {
        return 1;
    }
    
    do {
        printf("%s\n", errorMessage);
        string = myreadline("Введите ключ вставляемого элемента\n> ");
        if(string == NULL) {
            return 0;
        }
        n = getIntUnsignt(string);
        errorMessage = "\n(!) Неверный формат ключа. Попробуйте ещё раз\n";
        free(string);
        
    } while(n == -1);

    if(searchPlaceHD(table, n) != -1) {
        string = myreadline("Введите информацию\n> ");
        if(string == NULL){
            return 0;
        }
        if(addInfoHD(table, n, string)) {
            printf("\n(OK) Успешно\n");
        }
    } else {
        printf("\n(X) Таблица переполнена\n");
    }
    
    return 1;
}

int deleteKey_In(TableHD *table) {
    
    char *errorMessage = "";
    char *string = NULL;
    int n = 0;
    do {
        printf("%s\n", errorMessage);
        string = myreadline("Введите ключ удаляемого элемента\n> ");
        if(string == NULL) {
            return 0;
        }
        n = getIntUnsignt(string);
        errorMessage = "\n(!) Неверный формат ключа. Попробуйте ещё раз\n";
        free(string);
    } while(n == -1);

    if(!deleteKeyHD(table, n)){
        printf("\n(X) Ключ не найден\n");
    } else {
        printf("\n(OK) Успешно\n");
    }
    
    return 1;
}

int deleteKeyVersion_In(TableHD *table) {
    char *errorMessage = "";
    char *string = NULL;
    int n = 0;
    int version = 0;
    do {
        printf("%s\n", errorMessage);
        string = myreadline("Введите ключ удаляемого элемента\n> ");
        if(string == NULL) {
            return 0;
        }
        n = getIntUnsignt(string);
        errorMessage = "\n(!) Неверный формат ключа. Попробуйте ещё раз\n";
        free(string);
    } while(n == -1);
    
    errorMessage = "";
    
    do {
        printf("%s\n", errorMessage);
        string = myreadline("Введите версию удаляемого элемента\n> ");
        if(string == NULL) {
            return 0;
        }
        version = getIntUnsignt(string);
        errorMessage = "\n(!) Неверный формат версии. Попробуйте ещё раз\n";
        free(string);
    } while(version == -1);
    
    if(!deleteKeyVersionHD(table, n, version)){
        printf("\n(X) Ключ или версия не найдены\n");
    } else {
        printf("\n(OK) Успешно\n");
    }
    
    return 1;
}

int delete_In(TableHD *table, Table *search) {
    int (*func[])(TableHD *) = {NULL, deleteKey_In, deleteKeyVersion_In};
    const char *msgs[] = {"0. В основное меню", "1. Удалить по значению ключа", "2. Удалить по значению ключа и версии"};
    int checkZero = 0;
    int n = dialog(msgs, sizeof(msgs) / sizeof(msgs[0]), &checkZero);
    
    if(checkZero == -1) {
        return 0;
    }
    
    if(n == 0) {
        return 1;
    }
    
    func[n](table);
    
    return 1;
    
}

int printTable_In(TableHD *table, Table *search) {
    printTableHD(table);
    return 1;
}

Table* printTableSearch_In(TableHD *table, Table *search) {
    if(search != NULL) {
        printTable(search);
    } else {
        printf("\n(X) Результат поиска пуст\n");
    }
    
    return NULL;
}

Table* searchKey_In(TableHD *table, Table *search) {
    char *errorMessage = "";
    char *string = NULL;
    int n = 0;
    do {
        printf("%s\n", errorMessage);
        string = myreadline("Введите ключ искомого элемента\n> ");
        if(string == NULL) {
            return NULL;
        }
        n = getIntUnsignt(string);
        errorMessage = "\n(!) Неверный формат ключа. Попробуйте ещё раз\n";
        free(string);
    } while(n == -1);
    
    if(!searchKeyDialog(table, search, n)) {
        printf("\n(X) Ключ не найден\n");
    } else {
        printf("Результат поиска:\n");
        printTable(search);
    }
    
    return search;
}

Table *searchKeyVersion_In(TableHD *table, Table *search) {
    char *errorMessage = "";
    char *string = NULL;
    int n = 0;
    int version = 0;
    do {
        printf("%s\n", errorMessage);
        string = myreadline("Введите ключ искомого элемента\n> ");
        if(string == NULL) {
            return 0;
        }
        n = getIntUnsignt(string);
        errorMessage = "\n(!) Неверный формат ключа. Попробуйте ещё раз\n";
        free(string);
    } while(n == -1);
    
    errorMessage = "";
    
    do {
        printf("%s\n", errorMessage);
        string = myreadline("Введите версию искомого элемента\n> ");
        if(string == NULL) {
            return NULL;
        }
        version = getIntUnsignt(string);
        errorMessage = "\n(!) Неверный формат версии. Попробуйте ещё раз\n";
        free(string);
    } while(version == -1);
    
    
    if(!searchKeyVersionDialog(table, search, n, version)){
        printf("\n(X) Ключ или версия не найдены\n");
    } else {
        printf("Результат поиска:\n\n");
        printTable(search);
    }
    
    return search;
}

int search_In(TableHD *table, Table *search) {
    Table* (*func[])(TableHD *, Table *) = {NULL, searchKey_In, searchKeyVersion_In, printTableSearch_In};
    const char *msgs[] = {"0. В основное меню", "1. Поиск по значению ключа", "2. Поиск по значению ключа и версии", "3. Результат поиска"};
    int checkZero = 0;
    int n = dialog(msgs, sizeof(msgs) / sizeof(msgs[0]), &checkZero);
    
    if(checkZero == -1) {
        return 0;
    }
    
    if(n == 0) {
        return 1;
    }
    
    search = func[n](table, search);
    
    return 1;
}
































void printCat(void) {
    
    const char *cat[] = {"  ╭━━━╮  ╱╲   ╱╲", "  ┃╭━━╯  ▏▔▔▔▔▔▕ ", "  ┃╰━━━━━▏╭▆┊╭▆▕ ", "  ╰┫╯╯╯╯╯▏╰╯▼╰╯▕ ", "   ┃╯╯╯╯╯▏╰━┻━╯▕", "   ╰┓┏┳━┓┏┳┳━━━╯","    ┃┃┃ ┃┃┃┃","    ┗┻┛ ┗┛┗┛\n\n"};
    
    for (int i = 0; i < 45; i++) {
        system("clear");
        for (int b = 0; b < 8; b++) {
            for (int c = 0; c < i; c++) {
                printf(" ");
            }
            printf("%s\n", cat[b]);
            
        }
        printf("  Бегу сдавать лабу после ночи с valgrind\n");
        usleep(50000);
    }
    system("clear");
}

void printKoala(void) {
    const char *koala[] = {"             |       :     . |",
"             | '  :      '   |",
"             |  .  |   '  |  |",
"   .--._ _...:.._ _.--. ,  ' |",
"  (  ,  `        `  ,  )   . |",
"   '-/              \\-'  |   |",
"     |  o   /\\   o  |       :|",
"     \\     _\\/_     / :  '   |",
"     /'._   ^^   _.;___      |",
"   /`    `\"\"\"\"\"\"`      `\\=   |",
" /`                     /=  .|",
";             '--,-----'=    |",
"|                 `\\  |    . |",
"\\                   \\___ :   |",
"/'.                     `\\=  |",
"\\_/`--......_            /=  |",
"            |`-.        /= : |",
"            | : `-.__ /` .   |",
"            |    .   ` |    '|",
"            |  .  : `   . |  |"};
    
    
    const char *tree[] = {"             |       :     . |",
        "             | '  :      '   |",
        "             |  .  |   '  |  |"};
    
    int print;
    int koalaSize = sizeof(koala)/sizeof(koala[0]);
    for (int i = 0; i < 25; i++) {
        system("clear");
        for (int b = 0; b < 50 - koalaSize - i; b++) {
            print = b%3;
            printf("%s", tree[print]);
            printf("\n");
        }
        for(int i = 0; i < sizeof(koala)/sizeof(koala[0]); i++) {
            printf("%s", koala[i]);
            printf("\n");
        }
        for (int b = 0; b < i; b++) {
            print = b%3;
            printf("%s", tree[print]);
            printf("\n");
        }
        printf("  Ползу на лабу не спавши ночь\n");
        usleep(50000);
    }
    system("clear");
}
