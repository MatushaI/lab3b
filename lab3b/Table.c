#include "Table.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Table *importTable(FILE *file) {
    
    Table *table = calloc(sizeof(Table), 1);
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
    
    KeySpace *ks = calloc(table->maxSize, sizeof(KeySpace));
    fseek(file, 3*sizeof(unsigned long long), SEEK_SET);
    freadCheck = fread(ks, sizeof(KeySpace), table->maxSize, file);
    table->ks = ks;
    table->file = file;
    
    return table;
}

int addInfo(Table *table, int key, char *info) {
    int searchResult = searchPlace(table, key);
    int offset = 0;
    if (searchResult != -1) {
        KeySpace *element = &((table->ks)[searchResult]);
        Node *node = calloc(1, sizeof(Node));
        if(element->latestVersionOffset == 0) {
            element->key = key;
            node->version = 1;
            node->nextOffset = 0;
            node->lenght = (int) strlen(info) + 1;
            node->infoOffset = table->maxOffset + sizeof(Node);
            fseek(table->file, table->maxOffset, SEEK_SET);
            if(fwrite(node, sizeof(Node), 1, table->file)) {
                element->latestVersionOffset = table->maxOffset;
                offset += sizeof(Node);
            }
            table->maxOffset = table->maxOffset + offset;
            fseek(table->file, table->maxOffset, SEEK_SET);
            if((offset = (int) fwrite(info, sizeof(char), node->lenght, table->file)) < node->lenght) {
                element->latestVersionOffset = 0;
                
            }
            
        } else {
            element->key = key;
            
            Node swapNode;
            fseek(table->file, element->latestVersionOffset, SEEK_SET);
            fread(&swapNode, sizeof(Node), 1, table->file);
            unsigned long long prevOffset = element->latestVersionOffset;
            
            node->version = swapNode.version + 1;
            node->nextOffset = element->latestVersionOffset;
            node->lenght = (int) strlen(info) + 1;
            node->infoOffset = table->maxOffset + sizeof(Node);
            fseek(table->file, table->maxOffset, SEEK_SET);
            if(fwrite(node, sizeof(Node), 1, table->file)) {
                element->latestVersionOffset = table->maxOffset;
                offset += sizeof(Node);
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

int searchPlace(Table *table, int key) {
    KeySpace *keySpace = table->ks;
    for (int i = 0; i < table->maxSize; i++) {
        if(keySpace[i].key == key || keySpace[i].latestVersionOffset == 0) {
            return i;
        }
    }
    return -1;
}

void printTable(Table *table) {
    KeySpace *ks = table->ks;
    Node node;
    unsigned long long offset = 0;
    char *string = NULL;
    for (int i = 0; i < table->maxSize; i++) {
        if((offset = ks[i].latestVersionOffset) != 0) {
            printf("Ключ: %d\n", ks[i].key);
            while (offset) {
                fseek(table->file, offset, SEEK_SET);
                fread(&node, sizeof(Node), 1, table->file);
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

int deleteKey(Table *table, int key) {
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

int deleteKeyVersion(Table *table, int key, int version) {
    int flag = 0;
    unsigned long long prevOffset = 0;
    Node node;
    Node swapNode;
    for (int i = 0; i < table->maxSize; i++) {
        if(table->ks[i].key == key && table->ks[i].latestVersionOffset != 0) {
            unsigned long long offset = table->ks[i].latestVersionOffset;
            while (offset) {
                fseek(table->file, offset, SEEK_SET);
                fread(&node, sizeof(Node), 1, table->file);
                if(node.version == version) {
                    if(table->ks[i].latestVersionOffset == offset) {
                        table->ks[i].latestVersionOffset = node.nextOffset;
                        table->size = table->size - 1;
                    } else {
                        fseek(table->file, prevOffset, SEEK_SET);
                        fread(&swapNode, sizeof(Node), 1, table->file);
                        swapNode.nextOffset = node.nextOffset;
                        fseek(table->file, prevOffset, SEEK_SET);
                        fwrite(&swapNode, sizeof(Node), 1, table->file);
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

