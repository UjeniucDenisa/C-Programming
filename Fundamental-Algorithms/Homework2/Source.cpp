/*
* Ujeniuc Denisa-Andreea
* grupa 10
* Am ales sa implementez varianta recursiva pentru algoritmul de sortare Selection sort. Din graficele rezultate pentru cazul average, observam ca cele doua metode au o complexitate identica deoarece graficele sunt identice. O(n^2) 
* Timpul de executie este de asemenea acelasi.
* Am facut doi algoritmi de constuire a unui heap, respectiv bottom-up si top-down;
* Pentru metoda bottom-up complexitatea e O(nlgn):  build_heap_bu->O(n) apeluri ale heapify->O(lgn);
* Pentru metoda top-down complexitatea e tot O(nlgn);
* Pentru heapsort complexitatea e tot O(nlgn);
* Observam din grafice ca in cazul average, metoda bottom-up este mai rapida d.p.d vedere al numarului de atribuiri si comparatii, deci per total.
* In cazul worst, de asemenea bottom-up este o metoda mai rapida decat top-down, ca si atribuiri si comparatii, deci si per total ca nr de operatii.
* Pentru heapsort, din grafic reiese ca nr. de comparatii este mai redus ca cel al atribuirilor.
*/

#include <stdio.h>
#include "Profiler.h"


Profiler p("heap");

#define MAX_SIZE_SEL 1000
#define STEP_SIZE_SEL 10
#define NR_TESTS_SEL 1000

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 50


/*typedef struct _Node
{
    int key;
    struct _Node* left, * right;
} nod;*/

typedef struct {
    int dim;
    int capacitate;
    int* arr ;
} heap;

int parent(int i)
{
    return (i-1)/ 2;
   
}

int left(int i)
{
    
    return 2 * i+1;
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
    h->arr = (int*)malloc(capac * sizeof(int));
    return h;
}

void afisare_heap(heap* A)
{
    for (int i = 0; i < A->dim; i++)
    {
        printf("%d ", A->arr[i]);
    }
    printf("\n");
}

void heap_increase_key(heap* A, int i, int key, Operation* opComp, Operation* opAttr)
{
    (*opComp).count();
    if (key < A->arr[i])
    {
        //printf("Eroare:Noua cheie e mai mica decat cheia curenta");
        return;
    }
    (*opAttr).count();
    A->arr[i] = key;
    (*opComp).count();
    while (i > 0 && A->arr[parent(i)] < A->arr[i])
    {
        (*opAttr).count();
        (*opAttr).count();
        (*opAttr).count();

        int aux = A->arr[i];
        A->arr[i] = A->arr[parent(i)];
        A->arr[parent(i)] = aux;

        i = parent(i);
    }
}

void heap_insert(heap* A, int key, Operation *opComp, Operation *opAttr)
{
    if (A->dim > A->capacitate)
    {
        printf("Heap overflow\n");
        return;
    }
    A->dim = A->dim + 1;
    A->arr[A->dim - 1] = -10000000;
    heap_increase_key(A, A->dim - 1, key,opComp,opAttr);
}


void Heap_vector(heap* A, int* v,int n)
{
    if (n > A->capacitate)
    {
        printf("Vector de dimens prea mare");
        return;
    }
    A->dim = n;
    for (int i = 0; i < n; i++)
    {
        A->arr[i] = v[i];
    }
 }

void heapify(heap* A, int i, Operation* opComp, Operation* opAttr)
{
    int l = left(i);
    int r = right(i);
    int maxx;

    if (l < A->dim)
        (*opComp).count();

    if (l < A->dim && A->arr[l] > A->arr[i])
        maxx = l;
    else maxx = i;

    if (r < A->dim)
        (*opComp).count();

    if (r < A->dim && A->arr[r] > A->arr[maxx])
        maxx = r;


    if (maxx != i)
    {
        (*opAttr).count();
        (*opAttr).count();
        (*opAttr).count();

        int aux = A->arr[i];
        A->arr[i] = A->arr[maxx];
        A->arr[maxx] = aux;

        heapify(A, maxx,opComp, opAttr);
    }
}

void heapsort(heap* A)
{
    Operation opComp = p.createOperation("heapsort-comp", A->dim);
    Operation opAttr = p.createOperation("heapsort-attr", A->dim);
    int lung = A->dim;

    for (int i = A->dim-1; i >= 1; i--)
    {

        opAttr.count();
        opAttr.count();
        opAttr.count();

        int aux = A->arr[0];
        A->arr[0] = A->arr[i];
        A->arr[i] = aux;

        A->dim = A->dim - 1;
        heapify(A, 0, &opComp,&opAttr);

    }
    A->dim = lung;
}

void build_heap_bu(heap* h)
{
    Operation opComp = p.createOperation("bottomup-comp", h->dim);
    Operation opAttr = p.createOperation("bottomup-attr", h->dim);
    for (int i = h->dim / 2 - 1; i >= 0; i--)
    {
        heapify(h, i,&opComp,&opAttr);
    }
}

void build_heap_td(heap* h)
{
    Operation opComp = p.createOperation("topdown-comp", h->dim);
    Operation opAttr = p.createOperation("topdown-attr", h->dim);
    int lung_arr = h->dim;
    h->dim = 1;
    for (int i = 1; i < lung_arr; i++)
    {
        heap_insert(h, h->arr[i], &opComp, &opAttr);
    }
}

void perf_average_heap(int order)
{
    int v[MAX_SIZE], v1[MAX_SIZE], v2[MAX_SIZE];
    int n;
    heap* h = createHeap(MAX_SIZE);
    heap* h1 = createHeap(MAX_SIZE);
    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {
        for (int test = 0; test < NR_TESTS; test++)
        {
            FillRandomArray(v, n, 10, 50000, false, order);
    

            Heap_vector(h, v, n);
            Heap_vector(h1, v, n);
            build_heap_bu(h);
            build_heap_td(h1);

        }
    }
    p.divideValues("topdown-comp", NR_TESTS);
    p.divideValues("topdown-attr", NR_TESTS);
    p.addSeries("topdown", "topdown-comp", "topdown-attr");

    p.divideValues("bottomup-comp", NR_TESTS);
    p.divideValues("bottomup-attr", NR_TESTS);
    p.addSeries("bottomup", "bottomup-comp", "bottomup-attr");


    p.createGroup("attr", "bottomup-attr", "topdown-attr");
    p.createGroup("comp", "bottomup-comp", "topdown-comp");
    p.createGroup("total", "bottomup", "topdown");
   // p.showReport();

}

void perf_worst_heap(int order)
{
    int v[MAX_SIZE];
    int n;
    heap* h = createHeap(MAX_SIZE);
    heap* h1 = createHeap(MAX_SIZE);
    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {
       
        
            FillRandomArray(v, n, 10, 50000, false, order);

            Heap_vector(h, v, n);
            Heap_vector(h1, v, n);
            build_heap_bu(h);
            build_heap_td(h1);

        
    }
   
    p.addSeries("topdown", "topdown-comp", "topdown-attr");

    p.addSeries("bottomup", "bottomup-comp", "bottomup-attr");


    p.createGroup("attr", "bottomup-attr", "topdown-attr");
    p.createGroup("comp", "bottomup-comp", "topdown-comp");
    p.createGroup("total", "bottomup", "topdown");
    //p.showReport();

}

void perf_heap_bu()
{
    int v[MAX_SIZE];
    int n;
    heap* h = createHeap(MAX_SIZE);
    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {


        FillRandomArray(v, n, 10, 50000, false);

        Heap_vector(h, v, n);
        build_heap_bu(h);

    }

    p.addSeries("bottomup", "bottomup-comp", "bottomup-attr");

    //p.showReport();

}

void perf_heap_td()
{
    int v[MAX_SIZE];
    int n;
    heap* h = createHeap(MAX_SIZE);
    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {


        FillRandomArray(v, n, 10, 50000, false);

        Heap_vector(h, v, n);
        build_heap_td(h);

    }

    p.addSeries("topdown", "topdown-comp", "topdown-attr");

    //p.showReport();

}
void perf_heapsort()
{
    int v[MAX_SIZE];
    int n;
    heap* h = createHeap(MAX_SIZE);
    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {

        FillRandomArray(v, n, 10, 50000, false);
        Heap_vector(h, v, n);
        build_heap_bu(h);
        heapsort(h);

    }

    p.addSeries("heapsort", "heapsort-comp", "heapsort-attr");
    p.createGroup("heapsortt", "heapsort", "heapsort-comp", "heapsort-attr");
   // p.showReport();

}

void demo_heap()
{
    heap* h = createHeap(10);
    heap* h1 = createHeap(10);
    heap* h2 = createHeap(10);
    int v[7] = { 3,6,1,4,2,8,9 };
    int n = sizeof(v) / sizeof(v[0]);
    Heap_vector(h, v, n);
    Heap_vector(h1, v, n);
    Heap_vector(h2, v, n);
    printf("Heap-urile nesortate:\n");
    afisare_heap(h);
    afisare_heap(h1);
    afisare_heap(h2);
    printf("Heap dupa bottom-up:\n");
    build_heap_bu(h);
    afisare_heap(h);
    printf("Heap dupa top-down:\n");
    build_heap_bu(h1);
    afisare_heap(h1);
    printf("Heap dupa heapsort:\n");
    heapsort(h2);
    afisare_heap(h2);
}





//sortare
int min_elem_index(int* A, int i, int f, Operation* opComp, Operation* opAttr)
{
    int  minn =i,p;
    for ( p = i; p <= f; p++)
    {
        (*opComp).count();
        if (A[p] < A[minn]) minn = p;
    }
    return minn;
}

void selectionSortRec(int* A,int i, int n, Operation *opComp,Operation *opAttr)
{
    if (i == n-1) return;
    int p = min_elem_index(A, i , n-1, opComp, opAttr);
    if (p != i)
    {
        (*opAttr).count();
        int aux = A[p];
        (*opAttr).count();
        A[p] = A[i];
        (*opAttr).count();
        A[i] = aux;
    }
    
    selectionSortRec(A, i + 1, n,opComp,opAttr);
}

void selectionSort(int* A, int n)
{
    Operation opComp = p.createOperation("s-comp", n);
    Operation opAttr = p.createOperation("s-attr", n);
    for (int i = 0; i <= n - 1; i++)
    {
        int min = i;
        int j;
        for (j = i; j < n; j++)
        {
            opComp.count();
            if (A[j] < A[min])
            {
                min = j;
            }
        }
        if (i != min)
        {
            opAttr.count();
            int aux = A[min];

            opAttr.count();
            A[min] = A[i];

            opAttr.count();
            A[i] = aux;
        }
    }
}




void perf_selection()
{
    
    int v[MAX_SIZE_SEL], v1[MAX_SIZE_SEL];
    int n;
    
    for (n = STEP_SIZE_SEL; n <= MAX_SIZE_SEL; n += STEP_SIZE_SEL)
    {
        FillRandomArray(v, n, 10, 50000, false);
        for (int i = 0; i < n; i++)
        {
            v1[i] = v[i];
        }

        Operation ropComp = p.createOperation("sr-comp", n);
        Operation ropAttr = p.createOperation("sr-attr", n);

        selectionSort(v, n);
        selectionSortRec(v1,0, n,&ropComp,&ropAttr);


        p.startTimer("selectionSort", n);
        for (int test = 0; test < NR_TESTS_SEL; ++test) {
            selectionSort(v,n);
        }
        p.stopTimer("selectionSort",n);


        p.startTimer("selectionSortRec", n);
        for (int test = 0; test < NR_TESTS_SEL; ++test) {
            selectionSortRec(v,0,n,&ropComp,&ropAttr);
        }
        p.stopTimer("selectionSortRec", n);

    }

  /* p.divideValues("s-comp", NR_TESTS_SEL);
    p.divideValues("s-attr", NR_TESTS_SEL);*/
    p.addSeries("selection", "s-comp", "s-attr");

    /*p.divideValues("sr-comp", NR_TESTS_SEL);
    p.divideValues("sr-attr", NR_TESTS_SEL);*/
    p.addSeries("selection recursiv", "sr-comp", "sr-attr");

    p.createGroup("sel comp", "s-comp");
    p.createGroup("sel attr", "s-attr");
    p.createGroup("sel rec comp", "sr-comp");
    p.createGroup("sel rec attr", "sr-attr");
    //am facut separat ca sa observam ca graficele sunt identice pentru selection recursiv si iterativ.

    p.createGroup("attr", "sr-attr", "s-attr");
    p.createGroup("comp", "sr-comp", "s-comp");
    p.createGroup("total", "selection", "selection recursiv");
    
  
    //p.showReport();
}

void demo_selection()
{
    int n = 6;
    Operation opComp = p.createOperation("sr-comp", n);
    Operation opAttr = p.createOperation("sr-attr", n);
    int v[] = { 2,5,0,3,5,7 };
    int dim = 6;
    int v1[6];
    for (int i = 0; i < dim; i++)
    {
        v1[i] = v[i];

    }
   
    selectionSort(v, dim);
    selectionSortRec(v1,0,dim,&opComp,&opAttr);
    printf("Selection Sort:\n");
    for (int i = 0; i < dim; i++)
    {
        printf("%d ", v[i]);

    }
    printf("\nSelection sort recursiv\n");
    for (int i = 0; i < dim; i++)
    {
        printf("%d ", v1[i]);

    }
    
}

void perf_all()
{
    perf_selection();
    p.reset("Bottom up heap build");
    perf_heap_bu();
    p.reset("Top down heap build");
    perf_heap_td();
    p.reset("Average heap build");
    perf_average_heap(UNSORTED);
    p.reset("Worst heap build");
    perf_worst_heap(DESCENDING);
    p.reset("Heapsort");
    perf_heapsort();
    p.showReport();
}


int main()
{
    //demo_selection();
     demo_heap();
    //perf_all();
    return 0;
}