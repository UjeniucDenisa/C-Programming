#include <stdio.h>
#include <stdlib.h>
#include "sah.h"


void pe_linii(int i, int j, char table[][8], move *mutari, int *cate)
{
    for(int p=j+1; p<8; ++p) //aceasi linie, meregem pe coloane in dreapta
    {
        if(table[i][p]==' ')
        {
            mutari[*(cate)].c1='a'+j;
            mutari[*(cate)].r1=8-i;
            mutari[*(cate)].c2='a'+p;
            mutari[*(cate)].r2=8-i;
            ++(*cate);
        }
        else break;
    }
    for(int p=j-1; p>=0; --p) //aceasi linie, meregem pe coloane in stanga
    {
        if(table[i][p]==' ')
        {
            mutari[*(cate)].c1='a'+j;
            mutari[*(cate)].r1=8-i;
            mutari[*(cate)].c2='a'+p;
            mutari[*(cate)].r2=8-i;
            ++(*cate);
        }
        else break;
    }
    for(int p=i+1; p<8; ++p) //aceasi coloana, meregem pe linii in jos
    {
        if(table[p][j]==' ')
        {
            mutari[*(cate)].c1='a'+j;
            mutari[*(cate)].r1=8-i;
            mutari[*(cate)].c2='a'+j;
            mutari[*(cate)].r2=8-p;
            ++(*cate);
        }
        else break;
    }
    for(int p=i-1; p>=0; --p) //aceasi coloana, meregem pe linii in sus
    {
        if(table[p][j]==' ')
        {
            mutari[*(cate)].c1='a'+j;
            mutari[*(cate)].r1=8-i;
            mutari[*(cate)].c2='a'+j;
            mutari[*(cate)].r2=8-p;
            ++(*cate);
        }
        else break;
    }
}
void pe_diagonala(int i, int j, char table[][8], move *mutari, int *cate)
{
    for(int pi=i-1,pj=j+1; pi>=0&&pj<8; --pi,++pj) //pe diagonala in dreapta sus
    {
        if(table[pi][pj]==' ')
        {
            mutari[(*cate)].c1='a'+j;
            mutari[(*cate)].r1=8-i;
            mutari[(*cate)].c2='a'+pj;
            mutari[(*cate)].r2=8-pi;
        }
    }
    for(int pi=i+1,pj=j+1; pi<8&&pj<8; ++pi,++pj) //pe diagonala in dreapta jos
    {
        if(table[pi][pj]==' ')
        {
            mutari[(*cate)].c1='a'+j;
            mutari[(*cate)].r1=8-i;
            mutari[(*cate)].c2='a'+pj;
            mutari[(*cate)].r2=8-pi;
        }
    }

    for(int pi=i-1,pj=j-1; pi>=0&&pj>=8; --pi,--pj) //pe diagonala in stanga sus
    {
        if(table[pi][pj]==' ')
        {
            mutari[(*cate)].c1='a'+j;
            mutari[(*cate)].r1=8-i;
            mutari[(*cate)].c2='a'+pj;
            mutari[(*cate)].r2=8-pi;
        }
    }
    for(int pi=i+1,pj=j-1; pi<8&&pj>=8; ++pi,--pj) //pe diagonala in stanga jos
    {
        if(table[pi][pj]==' ')
        {
            mutari[(*cate)].c1='a'+j;
            mutari[(*cate)].r1=8-i;
            mutari[(*cate)].c2='a'+pj;
            mutari[(*cate)].r2=8-pi;
        }
    }
}
move* all_moves(char table[][8], char tomove)
{
    move *mutari=(move*)malloc(sizeof(move)*(13+14)*32); //13 maxim pe diagonale+14 maxim pe linie+coloana, 32 in caz ca jumate de tabla are piese pe ea
    int cate=0;
    for (int i=0; i<8; i++)
    {
        for (int j=0; j<8; j++)
        {

            if (tomove=='b') //ne uitam la piesele r,b,q
            {
                if (table[i][j]=='q')  //piesa e regina neagra
                {
                    pe_linii(i,j,table,mutari, &cate);
                    pe_diagonala(i,j,table,mutari, &cate);
                }
                else if (table[i][j]=='r')   //piesa e turn negru
                {
                    pe_linii(i,j,table,mutari, &cate);
                }
                else if (table[i][j]=='b') //piesa e nebun negru
                {
                    pe_diagonala(i,j,table,mutari, &cate);
                }
            }
            else if (tomove=='w') //ne uitam la R,B,Q
            {
                if (table[i][j]=='Q')  //piesa e regina alba
                {
                    pe_linii(i,j,table,mutari, &cate);
                    pe_diagonala(i,j,table,mutari, &cate);
                }
                else if (table[i][j]=='R')   //piesa e turn alb
                {
                    pe_linii(i,j,table,mutari, &cate);
                }
                else if (table[i][j]=='B') //piesa e nebun alb
                {
                    pe_diagonala(i,j,table,mutari, &cate);
                }
            }
            else
            {
                printf("Eroare: nu ai introdus tomove corect");
            }
        }
    }
    mutari[cate].c1='0';
    mutari[cate].c2='0';
    mutari[cate].r1=0;
    mutari[cate].r2=0;
    return mutari;
}


