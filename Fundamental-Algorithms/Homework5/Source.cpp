/*Ujeniuc Denisa-Andreea, grupa 10
* Tema 5
* Observam din tabelul de la punctul 2 ca cu cat factorul de umplere este mai mare efortul pentru gasirea unui element creste, 
*chiar daca acesta se afla sau nu in tabel;
* 
Pentru inserare si cautare, complexitatea in cele 3 cazuri:
* Best:O(1)
* Worst:O(N)
* Average: cu cate factorul de umplere creste, cu atat creste si complexitatea.
* 
* Pentru stergerea unui element am cautat elemntul si daca l am gasit l-am marcat ca sters intr un vector de dimensiunea tabelei
* Cand am cautat din nou in tabel si am accesat o casuta care e marcata ca DELETED am continuat cautarea pana la gasirea unei valori nulle
* 
* Inserarea in cel mai bun caz e atunci cand casuta accesata prima data e goala, iar in cazul worst e atunci cand tabela e aproape plina
* La cautare cel mai bun caz e atunci cand gasim elementul cautat pe prima casuta accesata, iar cel mai rau caz e atunci cand o gasim printre ultimele pozitii sau chiar deloc.
*/

#include <stdio.h>
#include "Profiler.h"

Profiler p("Hashing");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 50
//#define M 11
#define M 10007

typedef struct 
{
    int id;
    char name[30]; 
} Entry;


int second_hash_function(int id)
{
    return (id % 7);
   // printf("Second: %d\n",id % 7);
}
int hash_function(int id,int i)
{
    int c1 = 5,c2 = 3;
    return ((second_hash_function(id) + c1 * i + c2 * i * i) % M);
}

void hash_insert(Entry* T[M], int k, char *s)
{
    int i = 0, j;
    do
    {
        j = hash_function(k, i);
       // printf("H(%d,%d)=%d\n", k, i, j);
        if (T[j] == NULL)
         {
            T[j] = (Entry*)malloc(1*sizeof(Entry));
            T[j]->id = k;
            strncpy_s(T[j]->name, s, 30);
            return;
         }
        else
        {
            i++;
        }
        
    } while (i != M);
    printf("Eroare, hash table overflow!!!\n");
}


Entry* hash_search(Entry* T[M], int k,int deleted[], int *operatii)
{
    int i = 0,j;
    do
    {
        (*operatii)++;
        j = hash_function(k, i);
        if (T[j] != NULL )
        {
            if (T[j]->id == k)
            {
                printf("Am gasit serialul cu id-ul %d pe pozitia %d: %s \n", k, j, T[j]->name);
                return T[j];
            }
        }
            i++;
        
    } while ((T[j]!= NULL || deleted[j] == 1) && i < M-1);
    printf("Nu am gasit serialul cu id-ul %d  \n",k);
    return NULL;
}

void hash_delete(Entry *T[M], int id,int deleted[])
{
    for (int i = 0; i < M-1; i++)
    {
        int h_val = hash_function(id,i);
        int op=0;
        Entry* nd = hash_search(T, id,deleted,&op);
       // if(nd!=NULL)
       // printf("Am gasit nodul cu id-ul %d pe pozitia %d\n", id, h_val);
        if (nd == T[h_val])
        {
          //  if (nd->id == id)
           
                T[h_val] = NULL;
                deleted[h_val] = 1;
                return;
           
        }
    }
}

void printHashTable(Entry* T[M])
{
    for (int i = 0; i < M; i++)
    {
        //Entry* iterator = T[i];
        printf("%d : ", i);

        if (T[i]!= NULL)
        {
            printf("[ %s ] ", T[i]->name);
           
        }
        else  printf(" [ NULL ]");
        printf("\n");
    }
}

void perf_average_heap()
{
    int v[MAX_SIZE],pozitii[MAX_SIZE],v1[MAX_SIZE];
    float valori_alfa[5] = { 0.8,0.85,0.9,0.95,0.99 };
    FILE* f;// = fopen_s("Tabel.csv", "w");
    errno_t err = fopen_s(&f, "Tabel.csv", "w");
    fprintf(f, "factor de umplere,efortMediuGasite,efortMaximGasite,efortMediuNegasite,efortMaximNegasite\n");
    int n;
    for ( int i=0; i < 5; i++)
    {
        n = (int)(valori_alfa[i] * M);
        printf("n este :%d pentru factorul de umplere %f:\n", n, valori_alfa[i]);//numarul de elemente pe care vrem sa le inseram in tabela
        Entry* T[M];
        for (int m = 0; m < M; m++)
        {
            T[m] = NULL;
        }
        FillRandomArray(v, n, 10, 10000, true, UNSORTED);
        FillRandomArray(pozitii, n, 0, n-1, true, UNSORTED);
     
        char s[] = "Serial";
        for (int j = 0; j < n - 1; j++)
        {
            hash_insert(T, v[j], s);
        }
        for (int oo = 0; oo <= 4; oo++)
        {
            printf("%d ;", T[oo]->id);
        }
        
        long long int opGasiteTotal = 0,opNegasiteTotal=0;
        int efortMaximGasite = 0;
        int efortMaximNegasite = 0;
        
        for (int test = 0; test < 5; test++)
        {
            int deleted[M] = { 0 };
            //int nr_elem_gasite = 0;
            int opGasite=0;
            int opNegasite=0;
            for (int g = 0; g < 1500 && g<n; g++)
            {
                Entry* gs = hash_search(T, v[pozitii[g]], deleted,&opGasite);//caut elemente care sunt in tabel
                
            }
            FillRandomArray(v1, 1500, 10000, 20000, true, UNSORTED);
            for (int ng = 0; ng < 1500 &&ng<n; ng++)
            {
                Entry* ngs = hash_search(T, v1[ng], deleted,&opNegasite);//caut elemente care nu sunt in tabel
            }
            opGasite = opGasite / 1500;
            opNegasite = opNegasite / 1500;
            if (opGasite > efortMaximGasite) efortMaximGasite = opGasite;
            if (opNegasite > efortMaximNegasite) efortMaximNegasite = opNegasite;
            opGasiteTotal = opGasiteTotal + opGasite;
            opNegasiteTotal = opNegasiteTotal + opNegasite;

        }
        opGasiteTotal = opGasiteTotal / 5;
        opNegasiteTotal = opNegasiteTotal / 5;
        int EfortMediuGasite = opGasiteTotal;// 1500;
        int EfortMediuNegasite = opNegasiteTotal; // 1500;
        fprintf(f, "%f,%d,%d,%d,%d\n", valori_alfa[i], EfortMediuGasite, efortMaximGasite, EfortMediuNegasite, efortMaximNegasite);
        //printf("Nr operatii gasite : %d\nNumar operatii negasite: %d\n", opGasiteTotal, opNegasiteTotal);
    }
   

}



void demo_open_adressing()
{
   // Entry *T=(Entry*)calloc(M,sizeof(Entry));
    Entry* T[M];
    for (int i = 0; i < M; i++)
    {
        T[i] = NULL;
    }
    int deleted[M] = { 0 };
    char name1[30] = "Gossip Girl";
    char name2[30] = "Outerbanks";
    char name3[30] = "Velvet";
    char name4[30] = "TVD";
    char name5[30] = "Teen wolf";
    char name6[30] = "Breaking Bad";
    char name7[30] = "Stranger Things";
    char name8[30] = "Black Mirrror";
    char name9[30] = "Grey's Anatomy";
    char name10[30] = "Criminal Minds";
    char name11[30] = "Game of Thrones";
    char name12[30] = "Sherlock";
    hash_insert(T, 5, name1);
    hash_insert(T, 4,name2);
    hash_insert(T, 13, name3);
    hash_insert(T, 9, name4);
    hash_insert(T, 7, name5);
    hash_insert(T, 1, name6);
    hash_insert(T, 6, name7);
   // hash_insert(T, 2, name8);
  //  hash_insert(T, 15, name9);
  //  hash_insert(T, 20, name10);
  //  hash_insert(T, 21, name11);
  //  hash_insert(T, 3, name12);
    printHashTable(T);
    int op = 0;
    Entry* n1 = hash_search(T, 4,deleted,&op);
    Entry* n2 = hash_search(T, 8,deleted, &op);
    hash_delete(T, 13,deleted);
    Entry* n3 = hash_search(T, 6, deleted, &op);
    hash_delete(T, 6, deleted);
   // hash_delete(T, 6, deleted);
    printHashTable(T);
   // hash_search(T, 6, deleted);


    

}

int main()
{
   // demo_open_adressing();
    perf_average_heap();

    return 0;
}