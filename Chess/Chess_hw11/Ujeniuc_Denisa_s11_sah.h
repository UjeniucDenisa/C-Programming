#ifndef SAH_H_INCLUDED
#define SAH_H_INCLUDED

typedef struct
    {
        char c1, c2;
        int r1, r2;
    } move;
void pe_linii(int i, int j, char table[][8], move *mutari, int *cate);
void pe_diagonala(int i, int j, char table[][8], move *mutari, int *cate);
move* all_moves(char table[][8], char tomove);

#endif // SAH_H_INCLUDED
