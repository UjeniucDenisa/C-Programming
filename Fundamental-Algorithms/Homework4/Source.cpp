#include <stdio.h>
#include "Profiler.h"

/*
* Am realizat generarea a k liste sortate crescator pe care le-am retinut intr-un vector de tip "list".
* In total ceke k liste au n elemente. Pentru a le distribui uniform am folossit un vector in care am retinut lungimile fiecarei liste.
* Pentru a le face egale, am pus n/k  pentru lungimea fiecarei liste, iar cele n%k care au ramas le-am distribuit uniform incepand cu ultima lista.
* 
* Am realizat interclasarea a k liste utilizand un priority min-heap. Am repetat operatia "extract-min-heap" pentru a extrage minimul. Apoi am rearanjat heap-ul(heapify) 
* ca sa fie din nou un min-heap si am repetat procesul.
* Cand o lista ramanea fara elemente, scadeam dimensiunea heap-ului cu un element pentru a o elimina si puneam ultima lista in locul listei eliminate.
* 
* 
* Extract-min-O(logk);
* Complexitatea interclasarii a k liste e O(nlgk)
* 
* Observam din grafice ca: 
* 1.atunci cand variem n-ul si alegem pentru k 3 valori merge k lists este la fel de rapida d.p.d.v a atribuirilor,respectiv a comparatiilor.
* 2.atunci cand variem k-u si n ramane acelasi, merge k lists este la fel de rapida d.p.d.v a atribuirilor,respectiv a comparatiilor.
*/

Profiler p("heap");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 50
typedef struct nod
{
    int key;
    struct nod* next;
} nd;

typedef struct lista
{
    nd* first;
    nd* last;
}list;

void create_list_with_vector(list *l,int*v,int n)
{

    l->first = NULL;
    l->last = NULL;
    for (int i = 0; i < n; i++)
    {
        nd* p = (nd*)malloc(sizeof(nd));
        p->key = v[i];
        p->next = NULL;
        if (l->first == NULL)
        {
            l->first = p;
            l->last = p;
        }
        else
        {
            l->last->next = p;
            l->last = p;
        }
    }
}
void insert_last(list*l,int key)
{  nd* p = (nd*)malloc(sizeof(nd));
    p->next = NULL;
    p->key = key;
    if (l->first == NULL)
    {
      
        l->first = p;
        l->last = p;
    }
    else
    {
       
        l->last->next = p;
        l->last = p;
    }
}


void print_list(list* l)
{
    if (l->first == NULL) {
        printf("Lista e goala.\n");
    }
    else {
        
        nd* current = l->first; // Pointer temporar
        while (current != NULL) { // Continua atata timp cat current nu e NULL
            printf("%d ", current->key);
            current = current->next; // Muta la urmatorul nod
        }
    }
    printf("\n");
}

int* generare_lungimi(int k, int n)
{
    int* lungimi = (int*)malloc(k * sizeof(int));
    for (int i = 0; i < k; i++)
    {
        lungimi[i] = n / k;
    }
    int rest = n % k;
    if (rest != 0)
    {
        for (int j = k - 1; j >= 0; j--)
        {
            lungimi[j] = lungimi[j] + 1;
            rest--;
            if (rest <= 0) break;
        }

    }
    return lungimi;
}

list *generare_liste(int k, int n)
{

    int* lungimi = generare_lungimi(k, n);
    int** m_liste = (int**)malloc(k * sizeof(int*));

    list* liste = (list*)calloc(k, sizeof(list));

    for (int i = 0; i < k; i++)
    {
        *(m_liste + i) = (int*)malloc(lungimi[i] * sizeof(int));
        FillRandomArray(*(m_liste + i), lungimi[i], 10, 1000, false, ASCENDING);
        create_list_with_vector(&liste[i], m_liste[i], lungimi[i]);

    }
    return liste;
}

list* interclasare(list* l1, list* l2)
{
    list *l3=(list*)malloc(sizeof(list));
    l3->first = NULL;
    l3->last = NULL;
    //int lung3 = 0;
    nd* curr_l1 = l1->first;
    nd* curr_l2 = l2->first;
    while (curr_l1 != NULL && curr_l2 != NULL)
    {
        if (curr_l1->key < curr_l2->key)
        {
            insert_last(l3, curr_l1->key);
            curr_l1 = curr_l1->next;
        }
        else
        {
            insert_last(l3, curr_l2->key);
            curr_l2 = curr_l2->next;
        }
    }
    while (curr_l1 != NULL)
    {
        insert_last(l3, curr_l1->key);
        curr_l1 = curr_l1->next;
    }
    while (curr_l2 != NULL)
    {
        insert_last(l3, curr_l2->key);
        curr_l2 = curr_l2->next;
    }
    return l3;
}


typedef struct {
    int dim;
    int capacitate;
    list *arr;
} heap;

int parent(int i)
{
    return (i - 1) / 2;

}

int left(int i)
{

    return 2 * i + 1;
}

int right(int i)
{
    return 2 * i + 2;
}

heap* createHeap(int capac)
{
    heap* h = (heap*)malloc(sizeof(heap));
    h->dim = 0;
    h->capacitate = capac;
    h->arr = (list*)malloc(capac * sizeof(list));
    return h;
}

void Heap_lista(heap* A, list* l, int n)
{
    A->dim = n;
    for (int i = 0; i < n; i++)
    {
        A->arr[i] = l[i];
    }
}



void afisare_heap(heap* A)
{
    for (int i = 0; i < A->dim; i++)
    {
        printf("%d ", A->arr[i].first->key);
    }
    printf("\n");
}

void heapify(heap* A, int i, Operation* opComp, Operation* opAttr)
{
    int l = left(i);
    int r = right(i);
    int minn;
   
    if (l < A->dim)
        (*opComp).count();

    if (l < A->dim && A->arr[l].first->key < A->arr[i].first->key)
        minn = l;
    else minn = i;

    if (r < A->dim)
        (*opComp).count();

    if (r < A->dim && A->arr[r].first->key < A->arr[minn].first->key)
        minn = r;


    if (minn != i)
    {
        (*opAttr).count();
        (*opAttr).count();
        (*opAttr).count();

        list aux = A->arr[i];
        A->arr[i] = A->arr[minn];
        A->arr[minn] = aux;

        heapify(A, minn, opComp, opAttr);
    }
   
}

void build_heap_bu(heap* h,Operation *opComp,Operation *opAttr)
{
    //Operation opComp = p.createOperation("bottomup-comp", h->dim);
    //Operation opAttr = p.createOperation("bottomup-attr", h->dim);
    for (int i = h->dim / 2 - 1; i >= 0; i--)
    {
        heapify(h, i, opComp, opAttr);
    }
}

int extract_min_heap(heap* h,Operation *opComp,Operation *opAttr)
{
    
    if (h->dim < 1)
    {
        printf("Underflow");
        return -1;
    }

    (*opAttr).count();
    int minn = h->arr[0].first->key;
   
    (*opComp).count();
    if (h->arr[0].first->next != NULL)//daca urmatorul nod nu e null
    {
        (*opAttr).count();
        h->arr[0].first = h->arr[0].first->next;//punem pe prima pozitie a vectorului de liste urmatorul element din prima lista
    }
    
   //daca urmatorul nod e null
    else {
        if (h->dim > 1) {
            (*opAttr).count();
            h->arr[0] = h->arr[h->dim-1];//punem ultima lista pe prima poziite si dupa aplicam hepify
        }
        h->dim--; 
    }
    heapify(h,0,opComp,opAttr);
    return minn;
}


void demo_generare_liste_si_interclasare()
{
    int n, k;
    printf("k=");
    scanf_s("%d", &k);
    printf("\nn=");
    scanf_s("%d", &n);
    printf("%d %d\n", k, n);
    int* lungimi = generare_lungimi(k, n);
    list* liste = generare_liste(k, n);
    for (int i = 0; i < k; i++)
    {
        //printf("Lista %d este: ",i);
        print_list(&liste[i]);
    }
    printf("Lista dupa interclasarea listelor:");
    list* l3=interclasare(&liste[0],&liste[1]);
    for (int i = 2; i < k; i++)
    {
        l3 = interclasare(l3, &liste[i]);
    }

    print_list(l3);

}

list* interclasare_k_liste(heap* h,int n)//n=nr elem total
{
    Operation opComp = p.createOperation("interclasare-comp",n);
    Operation opAttr = p.createOperation("interclasare-attr",n);
    list* lf = (list*)malloc(sizeof(list));
    lf->first = NULL;
    lf->last = NULL;
    build_heap_bu(h,&opComp,&opAttr);
    //afisare_heap(h);
    while (h->dim > 0)
    {
    int minn = extract_min_heap(h, &opComp, &opAttr);
      //printf("Minimul este:%d \n", minn);

        opComp.count();
        opAttr.count();
        opAttr.count();
        insert_last(lf, minn);
    }
  
    return lf;
}

void demo_interclasare_k_liste()
{
    heap* H;
    int n, k;
    printf("k=");
    scanf_s("%d", &k);
    printf("\nn=");
    scanf_s("%d", &n);
    printf("%d %d\n", k, n);
    int* lungimi = generare_lungimi(k, n);
    list* liste = generare_liste(k, n);
    for (int i = 0; i < k; i++)
    {
        //printf("Lista %d este: ",i);
        print_list(&liste[i]);
    }
    printf("\nElementele interclasate:\n");
    H = createHeap(k);
    Heap_lista(H, liste, k);
    list* lf = interclasare_k_liste(H, n);
    print_list(lf);
}


void perf_average()
{
   
    int kv[] = { 5,10,100 };
    int n;
    for (int i = 0; i < 3; i++)
    {
        int k = kv[i];
        for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
        {
            for (int test = 0; test < NR_TESTS; test++)
            {

                int* lungimi = generare_lungimi(k, n);
                list* liste = generare_liste(k, n);
                heap* H = createHeap(k);
                Heap_lista(H, liste, k);
                list* lf = interclasare_k_liste(H, n);

  

            }

        }
    }

    p.divideValues("interclasare-comp", NR_TESTS);
    p.divideValues("interclasare-attr", NR_TESTS);
    p.addSeries("Interclasare", "interclasare-comp", "interclasare-attr");

    p.showReport();

}
void perf_average2()
{
   // int v[MAX_SIZE], v1[MAX_SIZE], v2[MAX_SIZE];
   // int kv[] = { 5,10,100 };
    int n=MAX_SIZE;
    
        for (int k = 10; k <= 500; k += 10)
        {
            for (int test = 0; test < NR_TESTS; test++)
            {
               
                int* lungimi = generare_lungimi(k, n);
                list* liste = generare_liste(k, n);
                heap* H = createHeap(k);
                Heap_lista(H, liste, k);
                list* lf = interclasare_k_liste(H, k);
                n = MAX_SIZE;

            }

        }

    p.divideValues("interclasare-comp", NR_TESTS);
    p.divideValues("interclasare-attr", NR_TESTS);
    p.addSeries("Interclasare", "interclasare-comp", "interclasare-attr");

    p.showReport();

}

int main()
{
    demo_generare_liste_si_interclasare();
    //demo_interclasare_k_liste();
    //perf_average(UNSORTED);
    //perf_average2();

    return 0;
}