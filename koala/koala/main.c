#include <stdio.h>

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
    for (int i = 0; i < 50; i++) {
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
    }
}

int main(void) {
    printKoala();
    return 0;
}
