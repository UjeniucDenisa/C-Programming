#include <stdlib.h>
#include <stdio.h>
#include "sah.h"

void encode(char table[][8], char* fen)
{
    int nr=0;
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(table[i][j]!=' ')
            {
                fen[nr]=table[i][j];
                nr++;
            }
            else //este spatiu
            {
                int nrspatii=0;
                while(table[i][j]==' '&&j<8)
                {
                    nrspatii++;
                    j++;
                }
                fen[nr]=nrspatii+'0';
                nr++;
                j--;
            }
        }
        fen[nr]='/';
        nr++;
    }
    fen[nr-1]=NULL;
    fen = (char*) realloc(fen,sizeof(char)*(nr-1));
}
void decode(char table[][8], char* fen)
{
    int i=0,j=0;
      for(int k=0;fen[k]!=NULL;k++)
      {
          if(fen[k]>='1'&&fen[k]<='8')
          {
              while(fen[k]>='1'&&fen[k]<='8')
              {
                  table[i][j]=' ';
                  j++;
                  fen[k]--;
              }
          }
          else if(fen[k]!='/')//avem litera
          {
              table[i][j]=fen[k];
              j++;
          }
          else
          {
              i++; //crestem linia
              j=0; //resetam coloana
          }
      }
}
