/*Ujeniuc Denisa-Andreea
* grupa 10
* Tema 6
* Am realizat cele 3 parcurgeri ale unui arbore:preorder,inorder si postorder recursiv O(n)
* iar parcurgerea preorder am realizat-o si iterativ, folosind o stiva unde am pus prima data radacina. Am scos cate un nod de pe stiva cu pop si l am afisat ,
*iar apoi am pus copilul de pe dr si cel de pe stanga pe stiva si am repetat procesul cat timp stiva nu e goala.
* Complexitate: O(n)
* 
* Pentru pretty print la R1 am mers recursiv si pentru fiecare apel am avut un parametru "nivel" care numara pe ce nivel al arborelui suntem ca sa pot pune cate 
* spatui trebuie. 
* 
* Pentru T1 am parcurs recursiv vectorul de parinti si am alocat fiecarui nod un vector de copii de dimensiunea necesara, dupa care am pus copiii nodului in vector
* Pentru fiecare din copiii acestuia am apelat recursiv functia si astfel s-a creat arborele multicai.
* 
* Pentru pretty print la R2 am mers recursiv si  am parcurs vectorul de copii si am apelat functia recursiv pentru fiecare, ultilizand de asemenea un parametru "nivel"
* 
* Pentru  T2 am creat structul 
*    typedef struct nod_r3
    {
        int key;
        struct nod_r3* copil;
        struct nod_r3* f_dreapta;
    } nodb;
* Apoi am creat o functie recursiva care sa puna pe copilul fiecarui nodb primul nod al nodului de la R2, iar pe fratele dreapta fratele din partea dreapta.;
* Am utilizat un parametru de tip nodb care sa tina minte parintele nodului curent, ca sa ii putem accessa fratii din dreapta
* 
* 
*/
#include <stdio.h>
#include "Profiler.h"

Profiler p("Hashing");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 50
//#define M 11
#define M 10007
typedef struct _Node
{
    int key;
    struct _Node* left, * right;
} NodeT;

typedef struct nod
{
    NodeT* bn;
    struct nod* next;
} nd;
typedef struct
{
    nd* first;
} Stack;

NodeT* root = NULL;
NodeT* root1 = NULL;

typedef struct nod2
{
    int key;
    struct nod2* next;
} nd2;

typedef struct
{
    nd2* first;
} Stack2;



void insertRecursive(int k, NodeT** root)
{
    if (*root == NULL)
    {
        (*root) = (NodeT*)malloc(sizeof(NodeT));
        if (*root == NULL)
        {
            printf("Nu mai e spatiu");
            return;
        }
        (*root)->key = k;
        (*root)->right = NULL;
        (*root)->left = NULL;
        return;
    }
    else
    {
        if ((*root)->key == k) return;
        else
        {
            if ((*root)->key > k)
                insertRecursive(k, &(*root)->left);
            else
                insertRecursive(k, &(*root)->right);
        }
    }
    return;

}
//operatii stiva

int empty(Stack* stiva)
{
    return stiva->first == NULL;
}
int empty2(Stack2* stiva)
{
    return stiva->first == NULL;
}
void push(Stack* stack, nd* n) {

    if (stack->first == NULL)
        stack->first = n;

    else {
        n->next = stack->first;
        stack->first = n;

    }
}

nd* pop(Stack* stack)
{
    if (stack->first == NULL)
    {
        return NULL;//stiva goala
    }
    nd* n = stack->first;
    stack->first = stack->first->next;
    return n;
}

void push2(Stack2* stack, int k) {

    nd2* n = (nd2*)malloc(1 * sizeof(nd2));
    n->key = k;


    if (stack->first == NULL)
        stack->first = n;

    else {
        n->next = stack->first;
        stack->first = n;

    }

}

int pop2(Stack2* stack)
{
    nd2* n = stack->first;
    int key = n->key;
    stack->first = stack->first->next;
    free(n);
    return key;
}

void afiseaza_stiva2(Stack2* s)
{
    if (s->first == NULL) printf("Stiva e goala\n");
    else
    {
        printf("Lista este: ");
        while (s->first != NULL)
        {
            printf("%d ", s->first->key);
            s->first = s->first->next;
        }
    }
    printf("\n");

}


void afiseaza_stiva(Stack* s)
{
    if (s->first == NULL) printf("Stiva e goala\n");
    else
    {
        printf("Lista este: ");
        while (s->first != NULL)
        {
            printf("%d ", s->first->bn->key);
            s->first = s->first->next;
        }
    }
    printf("\n");

}
int lenght_stack(Stack* s)
{
    int nr = 0;
    if (s->first == NULL) return 0;
    else
    {
        nd* iterator = s->first;
        while (iterator != NULL)
        {
            nr++;
            iterator = iterator->next;

        }
    }
    return nr;
}



//parcurgeri arbore

void print_preordine_iterativ(NodeT* root, Operation* op)
{
    if (root == NULL)
        return;
    Stack* s = (Stack*)calloc(1, sizeof(Stack));
    // s->first = NULL;
    NodeT* iterator = root;
    nd* iterator_stiva = (nd*)malloc(sizeof(nd));
    iterator_stiva->bn = iterator;
    iterator_stiva->next = NULL;
    push(s, iterator_stiva);
    while (empty(s) != 1)
    {
        iterator_stiva = pop(s);
        (*op).count();
        printf("%d ", iterator_stiva->bn->key);
        if (iterator_stiva->bn->right != NULL)
        {

            nd* rChild = (nd*)malloc(sizeof(nd));
            rChild->bn = iterator_stiva->bn->right;
            rChild->next = NULL;
            push(s, rChild);

        }
        if (iterator_stiva->bn->left != NULL)
        {
          
            nd* leftChild = (nd*)malloc(sizeof(nd));
            leftChild->bn = iterator_stiva->bn->left;
            leftChild->next = NULL;
            push(s, leftChild);
        }
        free(iterator_stiva);
        // printf("%d ", lenght_stack(s));
    }
}
void print_preordine(NodeT* root, Operation* op)
{
    if (root == NULL)
    {
        return;
    }
    (*op).count();
    printf("%d ", root->key);
    print_preordine(root->left, op);
    print_preordine(root->right, op);
}

void print_inordine(NodeT* root)
{
    if (root == NULL)
    {
        return;
    }
    print_inordine(root->left);
    printf("%d ", root->key);
    print_inordine(root->right);
}

void print_postordine(NodeT* root)
{
    if (root == NULL)
    {
        return;
    }
    print_postordine(root->left);
    print_postordine(root->right);
    printf("%d ", root->key);

}

void perf_operatii_parcurgeri_pre()
{
    int v[MAX_SIZE];// v1[MAX_SIZE];
    int n;
    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {
        NodeT* root3=NULL;
        FillRandomArray(v, n, 10, 500000, false, UNSORTED);
        for (int i = 0; i < n; i++)
        {
            insertRecursive(v[i], &root3);
        }
        Operation opRec = p.createOperation("operatii-recursiv", n);
        Operation opIt = p.createOperation("operatii-iterativ", n);
        print_preordine(root3, &opRec);
        print_preordine_iterativ(root3, &opIt);


    }

    // p.divideValues("operatii-recursiv", 1);
    // p.divideValues("operatii-iterativ", 1);

    p.createGroup("Operatii", "operatii-recursiv", "operatii-iterativ");
    p.createGroup("Operatii-rec", "operatii-recursiv");
    p.createGroup("Operatii-it", "operatii-iterativ");


    p.showReport();

}

void print_R1(int* v, int n)
{
    Stack2* s = (Stack2*)calloc(1, sizeof(Stack2));
    int i;
    int r;
    int count = 0;
    for (i = 0; i < n; i++)
    {
        if (v[i] == -1)
        {
            push2(s, i + 1);
            break;
        }
    }
    int nr_sp = 0;
    int ok = 1;
    
    int it_niv = 0;
    while (empty2(s) != 1)
    {
        int crrnt = pop2(s);
        printf("%*c", it_niv, ' ');
        printf("%d\n", crrnt);
       
        ok = 0;
        for (int i = n - 1; i >= 0; i--)
        {

            if (v[i] == crrnt)
            {
                ok = 1;
                push2(s, i + 1);
                
            }

        }
       
    }

}


/*void print_R1(int* v, int n)
{
    Stack2* s = (Stack2*)calloc(1, sizeof(Stack2));
    int i;
    int r;
    int count = 0;
    for (i = 0; i < n; i++)
    {
        if (v[i] == -1)
        {
            push2(s, i + 1);
            break;
        }
    }
    int nr_sp = 0;
    int ok = 1;
    int* nivel = (int*)calloc(n, sizeof(int));
    nivel[0] = 1;
    int it_niv = 0;
    while (empty2(s) != 1)
    {
        int crrnt = pop2(s);
        printf("%*c", it_niv, ' ');
        printf("%d\n", crrnt);
         nivel[it_niv]--;
         it_niv++;
        ok = 0;
        for (int i = n - 1; i >= 0; i--)
        {

            if (v[i] == crrnt)
            {
                ok = 1;
                push2(s, i + 1);
                nivel[it_niv]++;
            }

        }
        if (nivel[it_niv] != 0)
        {
            if (ok == 0)
            {
                nivel[it_niv]--;
                it_niv = it_niv + 1;
            }
            else
            {
                it_niv -= 1;
            }
        }
        else
            if (ok == 0)
            {
                it_niv = it_niv - 1;
            }
            else it_niv++;

    }

}

void printR1(int* v, int n)
{
    int** nod = (int**)malloc(n * sizeof(int*));
    if (nod == NULL)
    {
        printf("Eroare alocare");
        return;
    }
    int r;
    for (int i = 0; i < n; i++)
    {
       // printf("copii lui %d sunt:", i + 1);
        int nr = 0;
        for (int j = 0; j < n; j++)
        {
            if (v[j] == (i + 1)) {
                nr++;
                //printf("(%d) ", nr);
                //printf("%d ", j + 1);
            }
            if (v[j] == -1)
                r = j+1;
        }
        nr++;
        nod[i] = (int*)malloc(nr * sizeof(int));
        int m = 0;
        for (int j = 0; j < n; j++)
        {
            if (v[j] == (i + 1))
            {
                nod[i][m] = j + 1;
                m++;
            }
        }
        nod[i][m] = -2;
        //printf("\n");
    }
     for (int i = 0; i < n; i++)
     {
         printf("%d:", i);
         int j=0;
         while (nod[i][j] !=-2)
         {
             printf("%d ", nod[i][j]);
             j++;
         }
         printf("\n");
     }
     //afisare
     printf("r:%d", r);
     int c = 0;

}*/
void pretty_print_R1(int* v, int n, int nivel, int nod_crrnt)
{
    printf("%*c%d\n", nivel, ' ',nod_crrnt);
    for (int i = 0; i < n; i++)
    {
        if (v[i] ==nod_crrnt)
        {
            pretty_print_R1(v, n, nivel+1, i+1);
        }
    }
}
void R1(int*v,int n)
{
    printf("Pretty print pentru R1:\n");
    int r=0;
    int i=0;
    for ( i = 0; i < n; i++)
    {
        if (v[i] == -1)
        {
            r = i + 1;
            break;
        }
    }
    if (i == n)
    {
        printf("Nu am gasit radacina, vector incorect\n");
        return;
    }
    pretty_print_R1(v, n,1,r);
}

typedef struct nodd
{
    int key;
    struct nodd** copii;//vector de copii
} node;


    void T1_1(int* v, int n, node* nod_crrnt,int key, int nivel)
    {
        
        nod_crrnt->key = key;
        //printf("%*c%d\n", nivel, ' ', key);
        int nr = 0;
        for (int i = 0; i < n; i++)
        {
            if (v[i] == nod_crrnt->key)
            {
                nr++;
            }
        }
        nr++;
            nod_crrnt->copii = (node**)malloc(nr * sizeof(node*));//alocam vectorul de copii
            nod_crrnt->copii[nr-1] = NULL; //punem NULL pe ultima pozitie ca sa stim cand se termina vectorul
        int m = 0;
        for (int i = 0; i < n; i++)
        {
            if (v[i] == nod_crrnt->key)//cand gasim cate un copil
            {
                nod_crrnt->copii[m] = (node*)malloc(sizeof(node));//alocam nodul
                T1_1(v, n, nod_crrnt->copii[m],i + 1,nivel+1);//apelam recursiv pentru fiecare copil in parte
                m++;
            }
        }

    }
    node* T1(int* v, int n)
    {
        node* rad = (node*)malloc(sizeof(node));
        if (rad == NULL)
        {
            printf("Eroare alocare");
            return NULL;
        }
        int r = 0;
        int i = 0;
        for (i = 0; i < n; i++)
        {
            if (v[i] == -1)
            {
                r = i + 1;
                break;
            }
        }
        if (i == n)
        {
            printf("Nu am gasit radacina, vector incorect\n");
            return NULL;
        }
        T1_1(v, n,rad, r,0);//trimitem radacina ca primul parametru
        return rad;
    }

    void pretty_print_R2(node* nod_crrnt, int nivel)
    {
        if (nod_crrnt == NULL)
            return;
        printf("%*c%d\n", nivel, ' ', nod_crrnt->key);//afisam "nivel" spatii si apoi cheia nodului curent
        int i=0;
        while (nod_crrnt->copii[i] !=NULL)
        {
            pretty_print_R2(nod_crrnt->copii[i] , nivel + 1);//vrem sa afisam toti copiii nodului curent
            i++;
        }
        
    }
    
    typedef struct nod_r3
    {
        int key;
        struct nod_r3* copil;
        struct nod_r3* f_dreapta;
    } nodb;

    void T2(node *rad,nodb* rad_noua, node* parinte)
    {
        if (rad == NULL)
        {
            rad_noua = NULL;
            return;
        }
       // printf("%*c%d\n", nivel, ' ', nod_crrnt->key);//afisam "nivel" spatii si apoi cheia nodului curent
       
        rad_noua->key = rad->key;
        rad_noua->copil = NULL;
        rad_noua->f_dreapta = NULL;

        printf("%d\n", rad_noua->key);
        if (rad->copii[0] != NULL)//avem cel putin un copil
        {
            rad_noua->copil = (nodb*)malloc(sizeof(nodb));
            if (rad_noua->copil == NULL)
            {
                printf("Eroare alocare copil\n");
                return;
            }
            T2(rad->copii[0], rad_noua->copil, rad);//apel recursiv pt primul copil
        }
        
        //inlantuim fratii din dreapta
        if (parinte != NULL)
        {
            int i = 1;
            nodb* iterator_rad_noua=rad_noua;
            while (parinte->copii[i] != NULL)
            {
                iterator_rad_noua->f_dreapta = (nodb*)malloc(sizeof(nodb));
                if (iterator_rad_noua->f_dreapta == NULL)
                {
                    printf("Eroare alocare frate dreapta\n");
                    return;
                }
                iterator_rad_noua = iterator_rad_noua->f_dreapta;
                T2(parinte->copii[i], iterator_rad_noua, parinte); //apel recursiv fratele drept
  
                i++;
            }
 
        }
      
   }

    void T2_v2(node* rad, nodb* rad_noua, node* parinte)
    {
        if (rad == NULL)
        {
            rad_noua = NULL;
            return;
        }
        // printf("%*c%d\n", nivel, ' ', nod_crrnt->key);//afisam "nivel" spatii si apoi cheia nodului curent

        rad_noua->key = rad->key;
        rad_noua->copil = NULL;
        rad_noua->f_dreapta = NULL;

        printf("%d\n", rad_noua->key);
        if (rad->copii[0] != NULL)//avem cel putin un copil
        {
            rad_noua->copil = (nodb*)malloc(sizeof(nodb));
            if (rad_noua->copil == NULL)
            {
                printf("Eroare alocare copil\n");
                return;
            }
            T2(rad->copii[0], rad_noua->copil, rad);//apel recursiv pt primul copil
        }

        //inlantuim fratii din dreapta
        if (parinte != NULL)
        {
            if (parinte->copii[1] != NULL)
            {
                rad_noua->f_dreapta = (nodb*)malloc(sizeof(nodb));
                if (rad_noua->f_dreapta == NULL)
                {
                    printf("Eroare alocare frate dreapta\n");
                    return;
                }
                T2(parinte->copii[1], rad_noua->f_dreapta , parinte); //apel recursiv fratele drept
            }

        }

    }

void demo_parcurgeri()
{
    Operation opRec = p.createOperation("operatii-recursiv", 1000);
    insertRecursive(10, &root);
    insertRecursive(5, &root);
    insertRecursive(7, &root);
    insertRecursive(12, &root);
    insertRecursive(11, &root);
    insertRecursive(8, &root);
    insertRecursive(2, &root);
    insertRecursive(15, &root);
    printf("Preordine:\n");
    print_preordine(root, &opRec);
    printf("\nInordine:\n");
    print_inordine(root);
    printf("\nPostordine:\n");
    print_postordine(root);
    printf("\nPreordine iterativ:\n");
    print_preordine_iterativ(root, &opRec);

}


int main()
{
    demo_parcurgeri();
    //perf_operatii_parcurgeri_pre();
    int n = 9;
    int  v[] = { 2,7,5,2,7,7,-1,5,2 };
    R1(v,n);
    
    node* rad = (node*)malloc(sizeof(node));
    if (rad == NULL)
    {
        printf("Eroare alocare");
        return -1;
    }
    rad=T1(v, n);
    printf("\nPretty print la R2:\n");
    pretty_print_R2(rad, 0);
    //nodb* rad_noua = (nodb*)malloc(sizeof(nodb));
    //if (rad_noua == NULL)
     //   return -1;
   // T2_v2(rad, rad_noua, NULL);
    return 0;
}