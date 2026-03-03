/*Ujeniuc Denisa-Andreea, grupa 10
* Am realizat parcurgerea in adancime DFS . Initial, am marcat toate nodurile ca WHITE(neparcurse)
* Incepand de la un nod, il marcam ca vizitat(GRAY) si ii exploram toti vecinii care nu au fost inca vizitati(WHITE)
* Repetam procesul pentru fiecare vecin
* Dupa ce toti vecinii unui nod sunt explorati marcam nodul ca BLACk si ii pastram timpul de finalizare in vectorul timp_finalizare[]
* 
* Sortare topologica: se face odata cu dfs-ul
* avem o variabila int(flag ) care ne arata daca in graful nostru avem cicluri
* daca parcurgem un nod gri din nou(avem un vecin gri al unui nod ) in timpul dfs-ului avem un ciclu si facem flag-ul "aciclic" 1
* in timpul dfs-ului dupa ce marcam un nod ca black il punem in vectorul de sortare topologica
* nodurile vor fi adaugate in lista de sortare topologica in oridne inversa a timpilor de finalizare, de aceea o afisam de la coada la inceput
* 
* 
* Tarjan
* Alg lui Tarjan foloseste DFS pt a explora graful si determina componentele in care orice nod al componentei poate ajunge in orice alt nod al aceleiasi componente
* adica componentele tare conexe
* fiecarui nod i se atribuie un index si un lowlink
* se parcurg toti vecinii unui nod:
*    -daca un vecin nu a fost inca vizitat, il vizitam si se actualizeaza lowlink-ul
*    -daca un vecin a fost deja vizitat si e in stiva de recursivitate se actualizeaza lowlink-ul
* daca lowlink-ul unui nod e egal cu indexul sau atunci acesta impreuna cu vecinii sai formeaza o componenta tare conexa
* se scoate componenta din stiva si se proceseaza
* 
* la dfs am folosit 2 vectori in care am retinut timpii de descoperire si finalizare
* 
* 

*/

#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

Profiler p("Hashing");
typedef struct nd {
    int val;
    struct nd* next;
}nod;

typedef struct gr{
    int n;
    nod** t;
    int* pi;
    int* d;
    int* f;
    int* color;
   int* topologic; //pt ordinea topologica
    int index_t; // indexul pt ord topologica
    int aciclic=0;// daca e 0 e aciclic, daca e 1 nu e
    int* index; //indexul de vizitare(tarjan)
    int* lowlink;//vectorul low-link urilor(tarjan pt scc)
    int* onStack;//verif daca un nod e in stiva tarjan
    nod* stack;//stiva pt implementare iterativa tarjan
    int* comp;//componentele
    int nrComponents;//nr de componente
    int tarjan_index;
    int* timp_finalizare;
    int* timp_descoperire;
}graf;

enum {
    WHITE, GRAY, BLACK
};

void push(nod** stiva, int val)
{
    nod* el = (nod*)malloc(sizeof(nod));
    el->val = val;
    el->next = (*stiva);
    *stiva = el;
}

int pop(nod** stiva)
{
    int n = -1;
    if (*stiva != NULL)
    {
        nod* first = (*stiva);
        n = (*stiva)->val;
        *stiva = (*stiva)->next;
        free(first);
    }
    return n;
}
int empty(nod* stiva)
{
    return stiva == NULL;
}
void init(nod** stiva)
{
    *stiva = NULL;
}
void printErr() {
    printf("Memorie insuficienta!\n");
    exit(1);
}

void print_ordine_topologica(graf* G)
{
    printf("Ord topologica: ");
    for (int i = G->index_t - 1; i >= 0; i--)
    {
        printf("%d ", G->topologic[i]);
    }
    printf("\n");
}

void dfs_visit_iterativ(graf* G, int start,int *aciclic)
{
    nod* stack = NULL;
    push(&stack, start);
    int time = 0;

    while (empty(stack) != 1)
    {
        start = pop(&stack);

        //  printf("Culoare %d=%d ",start, G->color[start]);
          //daca nodul nu a fost procesat
        if (G->color[start] == 0)//WHITE
        {
            G->color[start] = 1; //in proces(GRAY)
            G->timp_descoperire[start] = ++time;
            printf("%d ", start);

           nod* node = G->t[start];
            while (node != NULL)
            {
                if (G->color[node->val] == 0)//WHITE
                {
                    G->pi[node->val] = start;
                    push(&stack, node->val);
                }
                else if (G->color[node->val] == 1)//GRAY
                {
                    printf("Ciclu detelctat in graf\n");
                    *(aciclic) = 1;//are cel putin un ciclu

                }
                node = node->next;
            }
        }
    //daca nodul a fost vizitat(GRAY) avem un ciclu
       
        G->color[start] = -1;//BLACK
        G->timp_finalizare[start] = ++time;

        G->topologic[G->index_t++] = start; //adaugam in vect de ordine topologica si incrementam dupa indexul
    }
}

void dfs_iterativ(graf* G, int *aciclic)
{
    *aciclic = 0;
    int i;
    for (int i = 0; i < G->n; i++)
    {
        if (G->color[i] == 0)//WHITE
            dfs_visit_iterativ(G, i, aciclic);
    }
}

void dfs_visit_recursiv(graf* G, int start, int* aciclic, int *time, Operation *opComp, Operation *opAttr)
{
    G->color[start] = GRAY;
    G->timp_descoperire[start] = ++*(time);
    printf("%d ", start);

    nod* node = G->t[start];
    while (node != NULL)
    {
        if (G->color[node->val] == WHITE)//vecinul nu a fost vizitat

        {
            G->pi[node->val] = start;
            dfs_visit_recursiv(G, node->val, aciclic, time,opComp,opAttr);
        }
        else if (G->color[node->val] == GRAY)//vecinul a mai fost vizitat, avem ciclu
        {
            printf("\nCilclu detectat\n");
            *aciclic = 1;//graful nu e aciclic
        }
        node = node->next;
    }
    //dupa ce am terminam cu nodul il marcam ca BLACK
    G->color[start] = BLACK;
    G->timp_finalizare[start] = ++*(time);

    //adaugam nodul la ordinea topologica
    G->topologic[G->index_t++] = start;
}

void dfs_recursiv(graf* G, int* aciclic, Operation *opComp, Operation *opAttr)
{
    *aciclic = 0;
    int time = 0;
    for (int i = 0; i < G->n; i++)
    {
        if (G->color[i] == WHITE)
        {
            dfs_visit_recursiv(G, i, aciclic, &time,opComp,opAttr);
        }
    }
}

void citeste_graf(FILE* f, graf* g)
{
    fscanf(f, "%d", &g->n);
    g->t=(nod**)calloc(g->n, sizeof(nod));
    if (g->t == NULL) printErr();
    g->pi = (int*)calloc(g->n, sizeof(int));
    g->d = (int*)calloc(g->n, sizeof(int));
    g->f = (int*)calloc(g->n, sizeof(int));
    g->color = (int*)calloc(g->n, sizeof(int));
    g->topologic = (int*)calloc(g->n, sizeof(int));
    g->index_t = 0;
    g->index = (int*)calloc(g->n, sizeof(int));
    g->lowlink = (int*)calloc(g->n, sizeof(int));
    g->onStack = (int*)calloc(g->n, sizeof(int));
    g->stack = NULL;
    g->nrComponents = 0;
    g->tarjan_index = 0;
    g->comp = (int*)calloc(g->n, sizeof(int));
    g->tarjan_index = 0;
    g->timp_descoperire= (int*)calloc(g->n, sizeof(int));
    g->timp_finalizare= (int*)calloc(g->n, sizeof(int));
    for (int i = 0; i < g->n; i++)
    {
        g->t[i] = NULL;
        g->d[i] = -1;
        g->f[i] = -1;
        g->pi[i] = -1;
        g->color[i] = 0;
        g->index[i] = -1;
        g->lowlink[i] = -1;
        g->onStack[i] = 0;
        g->comp[i] = 0;
        g->timp_descoperire[i] = -1;
        g->timp_finalizare[i] = -1;
    }
    int v, w;
    while (fscanf(f, "%d%d", &v, &w) == 2)
    {
        push(&g->t[v], w);
       //push(&g->t[w], v);
    }

}

//tarjan iterativ
void strongConnect(graf* G, int u)
{
    G->index[u] = G->lowlink[u] = G->tarjan_index++;
    push(&G->stack, u);
    G->onStack[u] = 1; //marcam ca nodul u e in stiva

    nod* node = G->t[u];
    while (node != NULL)
    {
        int v = node->val;
        if (G->index[v] == -1)
        {
            strongConnect(G, v);
            G->lowlink[u] = (G->lowlink[u] < G->lowlink[v]) ? G->lowlink[u] : G->lowlink[v];

        }
        else if(G->onStack[v]==1)
        {
            G->lowlink[u] = (G->lowlink[u] < G->lowlink[v]) ? G->lowlink[u] : G->index[v];

        }
        node = node->next;
    }

    if (G->lowlink[u] == G->index[u])
    {
        G->nrComponents++;
        int v;
        do {
            v = pop(&G->stack);
            G->onStack[v] = 0;
            G->comp[v] = G->nrComponents;
            printf("%d ", v);

        } while (v != u);
        printf("\n");

    }

}


void tarjan(graf* G)
{
    for (int u = 0; u < G->n; u++)
    {
        if (G->index[u] == -1)//daca nodul n a fost vizitat, apelam strongConect
        {
            strongConnect(G, u);
        }
    }

}


void print(int* arr, int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void genereaza_graf(graf* g, int V, int E)
{
    g->n = V;
    g->t = (nod**)calloc(g->n, sizeof(nod));
    if (g->t == NULL) printErr();
    g->pi = (int*)calloc(g->n, sizeof(int));
    g->d = (int*)calloc(g->n, sizeof(int));
    g->f = (int*)calloc(g->n, sizeof(int));
    g->color = (int*)calloc(g->n, sizeof(int));
    g->topologic = (int*)calloc(g->n, sizeof(int));
    g->index_t = 0;
   g->index = (int*)calloc(g->n, sizeof(int));
g->lowlink = (int*)calloc(g->n, sizeof(int));
    g->onStack = (int*)calloc(g->n, sizeof(int));
   g->stack = NULL;
    g->nrComponents = 0;
    g->tarjan_index = 0;
    g->comp = (int*)calloc(g->n, sizeof(int));
   g->tarjan_index = 0;
g->timp_descoperire = (int*)calloc(g->n, sizeof(int));
    g->timp_finalizare = (int*)calloc(g->n, sizeof(int));
    for (int i = 0; i < g->n; i++)
    {
        g->t[i] = NULL;
        g->d[i] = -1;
        g->f[i] = -1;
        g->pi[i] = -1;
        g->color[i] = WHITE;
        g->index[i] = -1;
        g->lowlink[i] = -1;
        g->onStack[i] = 0;
       g->comp[i] = 0;
       g->timp_descoperire[i] = -1;
        g->timp_finalizare[i] = -1;
    }
    int v, w;
    int e = 0;
    while (e < E)
    {
        v = rand() % g->n;
        w = rand() % g->n;
        if (v == w)
        {
            w = rand() % v - 3;
        }
        push(&g->t[v], w);
        e++;
      
    }
}
void perf()
{
    for (int e = 1000; e < 4500; e = e + 100)
    {
        Operation opComp = p.createOperation("dfs-comp(variem_muchiile)", 100);
        Operation opAttr = p.createOperation("dfs-attr(variem_muchiile)", 100);
        int aciclic = 0;
        graf* G = (graf*)malloc(sizeof(graf));
        genereaza_graf(G, 100, e);
        dfs_recursiv(G, &aciclic,&opComp,&opAttr);
        free(G);
    }

    for (int v = 100; v <= 200; v = v + 10)
    {
        Operation opComp = p.createOperation("dfs-comp(variem_varfurile)",v);
        Operation opAttr = p.createOperation("dfs-attr(variem_varfurile)", v);
        int aciclic = 0;
        graf* G = (graf*)malloc(sizeof(graf));
        genereaza_graf(G, v, 4500);
        dfs_recursiv(G, &aciclic, &opComp, &opAttr);
        free(G);
    }
    p.addSeries("dfs_muchii", "dfs-comp(variem_muchiile)", "dfs-attr(variem_muchiile)");
    p.addSeries("dfs_varfuri", "dfs-comp(variem_varfurile)", "dfs-attr(variem_varfurile)");
    p.showReport();
}
void demo()
{
    FILE* f = fopen("graf.txt", "r");
    if (f == NULL) {
        perror("graf.txt");
        exit(1);
    }
    graf* G = (graf*)malloc(sizeof(graf));
    citeste_graf(f, G);
    fclose(f);

    //afisare vecini;
    for (int i = 0; i < G->n; i++)
    {
        printf("%d: ", i);
        nod* n = G->t[i];
        while (n != NULL)
        {
            printf("%d ", n->val);
            n = n->next;
        }
        printf("\n");

    }
    Operation opComp = p.createOperation("dfs-comp(variem_muchiile)", G->n);
    Operation opAttr = p.createOperation("dfs-attr(variem_muchiile)", G->n);
    //varianta iterativa
    printf("\nDFS (varianta iterativa): ");
    int aciclic = 0;
    // dfs_iterativ(G, &aciclic);
    dfs_recursiv(G, &aciclic, &opComp, &opAttr);
    printf("\nVectorul de parinti:");
    print(G->pi, G->n);


    //afisare timp descoperire/ finalizare
    printf("\nTimp descoperire/finalizare:\n");
    for (int i = 0; i < G->n; i++)
    {
        printf(" timp_descoperire[%d] = %d; timp_finalizare[%d] = %d\n", i, G->timp_descoperire[i], i, G->timp_finalizare[i]);
    }


    //afisare sortare topologica
    if (aciclic == 0)
        print_ordine_topologica(G);
    else
    {
        printf("In graf sunt cilcuri deci nu se poate face sortarea topologica!\n");
    }

    printf("Componente tare conexe(SCC):\n");
    tarjan(G);
}
int main()
{
    demo();
   // perf();

    return 0;
}
