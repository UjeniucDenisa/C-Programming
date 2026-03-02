#include <stdio.h>
#include <stdlib.h>
#include "sah.h"



int main()
{
    char c;
    char table[8][8]=
    {
        //a   b   c   d   e   f   g   h
        {'n',' ',' ',' ',' ',' ',' ',' '},//8
        {' ','b',' ',' ',' ',' ','n',' '},//7
        {' ',' ',' ',' ',' ','b',' ',' '},//6
        {' ','n',' ','n',' ',' ',' ',' '},//5
        {' ',' ',' ',' ',' ',' ',' ',' '},//4
        {' ',' ',' ',' ',' ',' ',' ',' '},//3
        {' ',' ',' ',' ',' ',' ',' ',' '},//2
        {' ',' ',' ',' ',' ',' ',' ',' '} //1
    };


    printf("Introduceti piesa: ");
    scanf("%c",c);
    move *mutari=all_moves(table,c);
    for (int i=0; mutari[i].r1!=0; ++i)
    {
        printf("%c%d-%c%d, ",mutari[i].c1,mutari[i].r1,mutari[i].c2,mutari[i].r2);
        if (mutari[i].r1!=mutari[i+1].r1 || mutari[i].c1!=mutari[i+1].c1)
            printf("\n");
    }
    return 0;
}
