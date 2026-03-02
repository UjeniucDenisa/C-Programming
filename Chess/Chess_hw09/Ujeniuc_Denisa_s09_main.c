#include <stdlib.h>
#include <stdio.h>
#include "sah.h"

int main()
{
    char table[8][8]=
    {
        {'r',' ',' ',' ',' ',' ',' ','k',},
        {' ',' ',' ',' ',' ',' ','n','b'},
        {' ',' ',' ',' ',' ',' ',' ',' '},
        {' ',' ',' ',' ',' ',' ',' ',' '},
        {' ',' ',' ',' ',' ',' ',' ',' '},
        {' ',' ',' ',' ',' ',' ',' ',' '},
        {' ',' ',' ',' ',' ',' ',' ',' '},
        {' ',' ',' ',' ',' ',' ',' ',' '}
    };
    char *f= (char*) malloc(sizeof(char)*72);
    encode(table, f);
    printf("%s\n", f);
    printf("Introdu stringul fen: ");
    scanf("%s",f);
    decode(table, f);
    for (int i=0; i<8; ++i)
    {
        for (int j = 0; j < 8; ++j)
            printf("%c ",table[i][j]);
        printf("\n");
    }
    free(f);
}
