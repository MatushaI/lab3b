#include "TableHD.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Импорт таблицы (внутренняя hard)

TableHD *importTableHD(FILE *file) {
    
    TableHD *table = calloc(sizeof(TableHD), 1);
    unsigned long freadCheck = 0;
    
    fseek(file, 0, SEEK_SET);
    freadCheck = fread(&table->maxOffset, sizeof(table->maxOffset), 1, file);
    
    if(freadCheck == 0){
        free(table);
        return NULL;
    } 
    
    fseek(file, sizeof(unsigned long long), SEEK_SET);
    freadCheck = fread(&(table->maxSize), sizeof(table->maxSize), 1, file);
    
    fseek(file, 2*sizeof(unsigned long long), SEEK_SET);
    freadCheck = fread(&(table->size), sizeof(table->size), 1, file);
    
    KeySpaceHD *ks = calloc(table->maxSize, sizeof(KeySpaceHD));
    fseek(file, 3*sizeof(unsigned long long), SEEK_SET);
    freadCheck = fread(ks, sizeof(KeySpaceHD), table->maxSize, file);
    table->ks = ks;
    table->file = file;
    
    return table;
}

// Добавление информации по ключу (внешняя hard)

int addInfoHD(TableHD *table, int key, char *info) {
    int searchResult = searchPlaceHD(table, key);
    int offset = 0;
    if (searchResult != -1) {
        KeySpaceHD *element = &((table->ks)[searchResult]);
        NodeHD *node = calloc(1, sizeof(NodeHD));
        if(element->latestVersionOffset == 0) {
            element->key = key;
            node->version = 1;
            node->nextOffset = 0;
            node->lenght = (int) strlen(info) + 1;
            node->infoOffset = table->maxOffset + sizeof(NodeHD);
            fseek(table->file, table->maxOffset, SEEK_SET);
            if(fwrite(node, sizeof(NodeHD), 1, table->file)) {
                element->latestVersionOffset = table->maxOffset;
                offset += sizeof(NodeHD);
            }
            table->maxOffset = table->maxOffset + offset;
            fseek(table->file, table->maxOffset, SEEK_SET);
            if((offset = (int) fwrite(info, sizeof(char), node->lenght, table->file)) < node->lenght) {
                element->latestVersionOffset = 0;
                
            }
            
        } else {
            element->key = key;
            
            NodeHD swapNode;
            fseek(table->file, element->latestVersionOffset, SEEK_SET);
            fread(&swapNode, sizeof(NodeHD), 1, table->file);
            unsigned long long prevOffset = element->latestVersionOffset;
            
            node->version = swapNode.version + 1;
            node->nextOffset = element->latestVersionOffset;
            node->lenght = (int) strlen(info) + 1;
            node->infoOffset = table->maxOffset + sizeof(NodeHD);
            fseek(table->file, table->maxOffset, SEEK_SET);
            if(fwrite(node, sizeof(NodeHD), 1, table->file)) {
                element->latestVersionOffset = table->maxOffset;
                offset += sizeof(NodeHD);
            }
            table->maxOffset = table->maxOffset + offset;
            fseek(table->file, table->maxOffset, SEEK_SET);
            if((offset = (int) fwrite(info, sizeof(char), node->lenght, table->file)) < node->lenght) {
                element->latestVersionOffset = prevOffset;
                
            }
        }
        
        table->maxOffset = table->maxOffset + offset;
        table->size = table->size + 1;
        //free(info);
        free(node);
    }
    
    return 0;
}

//Найти свободное место для вставки (внутренняя)

int searchPlaceHD(TableHD *table, int key) {
    KeySpaceHD *keySpace = table->ks;
    for (int i = 0; i < table->maxSize; i++) {
        if(keySpace[i].key == key || keySpace[i].latestVersionOffset == 0) {
            return i;
        }
    }
    return -1;
}

// Печать таблицы (внешняя)

void printTableHD(TableHD *table) {
    KeySpaceHD *ks = table->ks;
    NodeHD node;
    unsigned long long offset = 0;
    char *string = NULL;
    for (int i = 0; i < table->maxSize; i++) {
        if((offset = ks[i].latestVersionOffset) != 0) {
            printf("Ключ: %d\n", ks[i].key);
            while (offset) {
                fseek(table->file, offset, SEEK_SET);
                fread(&node, sizeof(NodeHD), 1, table->file);
                fseek(table->file, node.infoOffset, SEEK_SET);
                string = calloc(sizeof(char), node.lenght);
                fread(string, sizeof(char), node.lenght, table->file);
                printf("Версия: %d, информация: %s \n", node.version, string);
                free(string);
                offset = node.nextOffset;
            }
        }
    }
}

// Удаление по ключу (внешняя)

int deleteKeyHD(TableHD *table, int key) {
    int flag = 0;
    for (int i = 0; i < table->maxSize; i++) {
        if(table->ks[i].key == key && table->ks[i].latestVersionOffset != 0) {
            table->ks[i].latestVersionOffset = 0;
            flag++;
            table->size = table->size - 1;
            break;
        }
    }
    
    if(!flag) {
        return 0;
    }
    
    return 1;
}

// Удаление по ключу и версии (внешняя)

int deleteKeyVersionHD(TableHD *table, int key, int version) {
    int flag = 0;
    unsigned long long prevOffset = 0;
    NodeHD node;
    NodeHD swapNode;
    for (int i = 0; i < table->maxSize; i++) {
        if(table->ks[i].key == key && table->ks[i].latestVersionOffset != 0) {
            unsigned long long offset = table->ks[i].latestVersionOffset;
            while (offset) {
                fseek(table->file, offset, SEEK_SET);
                fread(&node, sizeof(NodeHD), 1, table->file);
                if(node.version == version) {
                    if(table->ks[i].latestVersionOffset == offset) {
                        table->ks[i].latestVersionOffset = node.nextOffset;
                        table->size = table->size - 1;
                    } else {
                        fseek(table->file, prevOffset, SEEK_SET);
                        fread(&swapNode, sizeof(NodeHD), 1, table->file);
                        swapNode.nextOffset = node.nextOffset;
                        fseek(table->file, prevOffset, SEEK_SET);
                        fwrite(&swapNode, sizeof(NodeHD), 1, table->file);
                    }
                    flag++;
                    break;
                }
                prevOffset = offset;
                offset = node.nextOffset;
                }
            }
        }
        
    
    if(!flag) {
        return 0;
    }
    
    return 1;
}

// Добавление узла (main)

int addNode(KeySpace *element){
    Node* node = element->node;
    Node *newnode = NULL;
    if(!(newnode = calloc(1, sizeof(Node)))) {
        return 0;
    } 
    
    if(element->isActive == -1) {
        element->node = newnode;
        newnode->prev = NULL;
        newnode->version = 1;
    } else {
        newnode->prev = node;
        newnode->version = node->version + 1;
        element->node = newnode;
    }
    return 1;
}

//Создание таблицы (специальная main)

Table *tableCreate(int size) {
    Table *table = NULL;
    table = calloc(1, sizeof(Table));
    table->realSize = 0;
    table->maxSize = size;
    table->ks = calloc(size, sizeof(KeySpace));
    for (int i = 0; i < size; i++) {
        table->ks[i].node = NULL;
        table->ks[i].isActive = -1;
    }
    return table;
}

// Очистка списка (main)

void clearList(Node *node) {
    Node *element = node;
    Node *del = NULL;
    while (element != NULL) {
        del = element;
        free(element->info);
        element = element->prev;
        free(del);
    }
}

//Очистка таблицы (main)

void clearTable(Table *table) {
    if(table != NULL) {
        for(int i = 0; i < table->maxSize; i++){
            clearList(table->ks[i].node);
        }
        free(table->ks);
        free(table);
    }
    table = NULL;
}

// Поиск по ключу

int searchKeyDialog(TableHD *table, Table *search, int key) {
    
    unsigned long long offset = 0;
    clearList(search->ks[0].node);
    search->realSize = 0;
    search->ks[0].isActive = -1;
    search->ks[0].node = NULL;
    NodeHD node;
    char *string = NULL;
    printf("тут\n");
    for (int i = 0; i < table->maxSize; i++) {
        if(table->ks[i].key == key && table->ks[i].latestVersionOffset != 0) {
            search->realSize = 1;
            KeySpace *ks = &search->ks[0];
            ks->key = key;
            
            
            offset = table->ks[i].latestVersionOffset;
            
            while (offset) {
                fseek(table->file, offset, SEEK_SET);
                fread(&node, sizeof(Node), 1, table->file);
                
                addNode(ks);
                string = calloc(node.lenght, sizeof(char));
                
                fseek(table->file, node.infoOffset, SEEK_SET);
                fread(string, sizeof(char), node.lenght, table->file);
                
                ks->node->version = node.version;
                ks->isActive = 1;
                ks->node->info = string;
                
                printf("%s\n", ks->node->info);
                offset = node.nextOffset;
                
            }
            return 1;
        }
    }
    return 0;
}

// Поиск по ключу и версии


int searchKeyVersionDialog(TableHD *table, Table *search, int key, unsigned int version) {
    
    clearList(search->ks[0].node);
    search->realSize = 0;
    search->ks[0].isActive = -1;
    search->ks[0].node = NULL;
    NodeHD node;
    unsigned long long offset;
    char *string = NULL;
    
    for (int i = 0; i < table->maxSize; i++) {
        if(table->ks[i].key == key && table->ks[i].latestVersionOffset != 0) {
            
            offset = table->ks[i].latestVersionOffset;
            
            while (offset) {
                fseek(table->file, offset, SEEK_SET);
                fread(&node, sizeof(NodeHD), 1, table->file);
                
                if(node.version == version) {
                    search->realSize = 1;
                    search->ks[0].key = key;
                    addNode(search->ks);
                    search->ks[0].isActive = 1;
                    string = calloc(node.lenght, sizeof(char));
                    fread(string, sizeof(char), node.lenght, table->file);
                    search->ks[0].node->info = string;
                    search->ks[0].node->version = node.version;
                    return 1;
                }
                offset = node.nextOffset;
            }
        }
    }
    
    return 0;
}

// Печать таблицы (main)

void printTable(Table *table) {
    if(table != NULL){
        for (int i = 0; i < table->maxSize; i++) {
            KeySpace key = table->ks[i];
            Node *element = NULL;
            if(key.isActive == 1){
                printf("Ключ: %d\n", key.key);
                element = key.node;
                while (element != NULL) {
                    printf("Версия: %d, информация: %s\n", element->version, element->info);
                    element = element->prev;
                }
            }
        }
    }
}

// Выгрузка таблицы (hard)

void exportTableHD(TableHD *table) {
    fseek(table->file, 3*sizeof(unsigned long long), SEEK_SET);
    fwrite(table->ks, sizeof(KeySpaceHD), table->maxSize, table->file);
    fseek(table->file, 2*sizeof(unsigned long long), SEEK_SET);
    fwrite(&table->size, sizeof(int), 1, table->file);
    fseek(table->file, 0, SEEK_SET);
    fwrite(&table->maxOffset, sizeof(int), 1, table->file);
    fclose(table->file);
    free(table->ks);
    free(table);
    
}
