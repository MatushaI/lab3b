#include "getFunctions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <unistd.h>

FILE* getFile(const char* invite, char choose[4], int *check) {
    FILE *file = NULL;
    char *dir = NULL;
    dir = getcwd(dir, 1000);
    dir = strtok(dir, "\t");
    char *s = NULL;
    int lendir = (int) strlen(dir);
    s = readline(invite);
    if(s) {
        dir = realloc(dir, lendir*sizeof(char)+strlen(s)*sizeof(char) + 2);
        dir[lendir] = '/';
        dir[lendir+1] = '\0';
        dir = strcat(dir, s);
        file = fopen(dir, choose);
        free(s);
    } else {
        *check = -1;
    }
    printf("%s\n", dir);
    free(dir);
    return file;
}
