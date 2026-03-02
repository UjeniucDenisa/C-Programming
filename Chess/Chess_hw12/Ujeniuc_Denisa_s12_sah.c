#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char c1, c2;
    int r1, r2;
}move;

typedef struct{
    char table[8][8];
    move mv;
}config;


int capturare(int i, int j, int pi, int pj, char table[][8], int r2, char c2){
    if (pi==r2 && pj==c2){
        if (isupper(table[i][j])!=0){
            if (strchr("rbqp",table[r2][c2])!=NULL)
                return 1;
        }else if (strchr("RBQP",table[r2][c2])!=NULL)
            return 1;
    }
    return 0;
}


int pe_linii(int i, int j, char table[][8], int r2, char c2){
    int p;

    if (i==r2){
        for(p=j+1; p<8 && table[i][p]==' '; ++p) //aceasi linie, meregem pe coloane in dreapta
            if(p==c2)
                return 1;

        if (capturare(i,j,i,p,table,r2,c2)==1)
            return 1;

//////////
        for(p=j-1; p>=0 && table[i][p]==' '; --p) //aceasi linie, meregem pe coloane in stanga
            if(p==c2)
                return 1;

        if (capturare(i,j,i,p,table,r2,c2)==1)
            return 1;
    }

/////////
    if (j==c2){
        for(p=i+1; p<8 && table[p][j]==' '; ++p) //aceasi coloana, meregem pe linii in jos
            if (p==r2)
                return 1;

        if (capturare(i,j,p,j,table,r2,c2)==1)
            return 1;

//////////
        for(p=i-1; p>=0 && table[p][j]==' '; --p) //aceasi coloana, meregem pe linii in sus
            if (p==r2)
                return 1;

        if (capturare(i,j,p,j,table,r2,c2)==1)
            return 1;
    }
    return 0;
}

int pe_diagonala(int i, int j, char table[][8], int r2, char c2){

    int pi, pj;

    for (pi=i-1, pj=j+1; pi>=0 && pj<8 && table[pi][pj]==' '; --pi,++pj)  //pe diagonala in dreapta sus
        if (pi==r2 && pj==c2)
            return 1;

    if (capturare(i,j,pi,pj,table,r2,c2)==1)
        return 1;

///////
    for(pi=i+1,pj=j+1; pi<8 && pj<8 && table[pi][pj]==' '; ++pi,++pj) //pe diagonala in dreapta jos
        if (pi==r2 && pj==c2)
            return 1;

    if (capturare(i,j,pi,pj,table,r2,c2)==1)
        return 1;

///////
    for(pi=i-1,pj=j-1; pi>=0 && pj>=0 && table[pi][pj]==' '; --pi,--pj) //pe diagonala in stanga sus
        if (pi==r2 && pj==c2)
            return 1;

    if (capturare(i,j,pi,pj,table,r2,c2)==1)
        return 1;

//////
    for(pi=i+1,pj=j-1; pi<8 && pj>=0 && table[pi][pj]==' '; ++pi,--pj) //pe diagonala in stanga jos
        if (pi==r2 && pj==c2)
            return 1;

    if (capturare(i,j,pi,pj,table,r2,c2)==1)
        return 1;

    return 0;
}

void run_tests(){
    int n;
    FILE *ptr, *scrie;

    ptr = fopen("chess_in.bin","rb");  // r for read, b for binary
    scrie = fopen("chess_out.txt", "w");

    fread(&n,sizeof(int),1,ptr);
    //printf("%d\n", n);

    config c;
    for (int o=0; o<n; ++o){
        unsigned char buff[65], spatiu;
        fread(buff, sizeof(char),8*8,ptr);

        fread(&c.mv.c1, sizeof(char),1,ptr);
        fread(&c.mv.c2, sizeof(char),1,ptr);
        fread(&spatiu, sizeof(char),1,ptr);//spatiu
        fread(&spatiu, sizeof(char),1,ptr);//spatiu
        fread(&c.mv.r1, sizeof(int),1,ptr);
        fread(&c.mv.r2, sizeof(int),1,ptr);

        for (int a=0; a<8; ++a){
            //printf("%d",8-a);
            for (int b=0; b<8; ++b) {
                c.table[a][b] = buff[a*8+b];
                //if (c.table[a][b]==' ')
                //printf(" _ ");
                //else printf(" %c ", c.table[a][b]);
            }
            //printf("\n");
        }
        //printf("  a  b  c  d  e  f  g  h\n");
        //printf("%c%d %c%d\n", c.mv.c1,c.mv.r1, c.mv.c2, c.mv.r2);

        int ok=0;
        int r1=8-c.mv.r1, r2=8-c.mv.r2;
        char c1=c.mv.c1-'a', c2=c.mv.c2-'a';

        //verificam daca este pozitie buna
        if (r1<0 || r1 >7 || r2 <0 || r2>7 || c1<0 || c1>7 || c2<0 || c2>7 || c.table[r1][c1]==' '){
            //printf("0000\n");
            fprintf(scrie, "0\n");
            continue;
        }
        //daca avem pion negru
        if (c.table[r1][c1]=='p'){
            if (r1+1==r2){
                if (c1==c2 && c.table[r2][c2]==' ') //mergem in jos
                    ok=1;
                else if ((c1+1==c2 || c1-1==c2) && strchr("RBQP",c.table[r2][c2])!=NULL) //capturam o piese la diag dr
                    ok=1;

            }else if (r1==1 && r1+2==r2 && c1==c2 && c.table[r2][c2]==' ')
                ok=1;

        }else if (c.table[r1][c1]=='P'){
            if (r1-1==r2){
                if (c1==c2 && c.table[r2][c2]==' ') //mergem in sus
                    ok=1;
                else if ((c1+1==c2 || c1-1==c2) && strchr("rbqp",c.table[r2][c2])!=NULL) //capturam o piese la diag dr
                    ok=1;

            }else if (r1==6 && r1-2==r2 && c1==c2 && c.table[r2][c2]==' ')
                ok=1;

        }else if (c.table[r1][c1]=='b' || c.table[r1][c1]=='B'){
            ok+=pe_diagonala(r1,c1,c.table,r2,c2);

        }else if (c.table[r1][c1]=='r' || c.table[r1][c1]=='R'){
            ok+=pe_linii(r1,c1,c.table,r2,c2);

        }else if (c.table[r1][c1]=='q' || c.table[r1][c1]=='Q'){
            ok+=pe_linii(r1,c1,c.table,r2,c2);
            ok+=pe_diagonala(r1,c1,c.table,r2,c2);
        }
        //printf("%d\n\n\n",ok);
        fprintf(scrie, "%d\n",ok);
    }
}
