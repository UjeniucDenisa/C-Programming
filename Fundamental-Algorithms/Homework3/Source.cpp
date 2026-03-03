/*
* Am implementat metoda de sortare QuickSort care este o metoda recursiva . 
* Observam din grafice ca:
* Complexitatea in cazul average, cand sirul este nesortat, ca complexitatea este O(n*log n)
* Complexitatea in cazul worst , cand sirul e sortat este O(n^2)
* Cazul best este atunci cand pozitia returnata de "partition" va fi mereu mediana sirului curent.
* 
* Am comparat metoda Quicksort cu metoda heapsort in cazul average. Din grafice putem observa ca:
* -metoda heapsort este mai rapida ca nr. de atriburi, respectiv si ca nr de comparatii, deci e mai rapida ca numar total de operatii.
* 
* Am implementat metoda de sortare cu Quicksort hibridizat, folosind insertion sort de la un anumit prag.
* Atunci cand pragul folosit e de 30, Metoda hibridizata e mai rapida ca numar de atriburi, dar mai inceata ca numar de comparatii fata de metoda initiala a Quicksortului.
* Ca numar total de operatii totusi, metodsa hibridizata este mai rapida atunci cand pragul este 30.
* Ca timp de executie, graficele sunt identice.
* 
* Am implementat metoda QuickSelect, care returneaza al i-lea element dintr-un sir nesortat
 */

#include <stdio.h>
#include "Profiler.h"


Profiler p("heap");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 50

#define MAX_SIZE2 1000
#define STEP_SIZE2 10
#define NR_TESTS2 50

#define MAX_SIZE3 100
#define STEP_SIZE3 10


typedef struct {
    int dim;
    int capacitate;
    int* arr;
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


void Heap_vector(heap* A, int* v, int n)
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

        heapify(A, maxx, opComp, opAttr);
    }
}
void build_heap_bu(heap* h, Operation* opComp, Operation* opAttr)
{

    for (int i = h->dim / 2 - 1; i >= 0; i--)
    {
        heapify(h, i, opComp, opAttr);
    }
}

void heapsort(heap* A)
{
    Operation opComp = p.createOperation("heapsort-comp", A->dim);
    Operation opAttr = p.createOperation("heapsort-attr", A->dim);
    build_heap_bu(A, &opComp, &opAttr);


    int lung = A->dim;
    for (int i = A->dim - 1; i >= 1; i--)
    {

        opAttr.count();
        opAttr.count();
        opAttr.count();

        int aux = A->arr[0];
        A->arr[0] = A->arr[i];
        A->arr[i] = aux;

        A->dim = A->dim - 1;
        heapify(A, 0, &opComp, &opAttr);

    }
    A->dim = lung;
}


void demo_heap()
{
    heap* h = createHeap(10);
    int v[7] = { 3,6,1,4,2,8,9 };
    int n = sizeof(v) / sizeof(v[0]);
    Heap_vector(h, v, n);
    printf("Heap-urile nesortate:\n");
    afisare_heap(h);
    printf("Heap dupa heapsort:\n");
    heapsort(h);
    afisare_heap(h);
}


//QuickSort

int partition(int* A, int p, int r, Operation* opComp, Operation* opAttr)
{
    int i = p - 1;
    for (int j = p; j < r; j++)
    {
        (*opComp).count();
        if (A[j] <= A[r])
        {
            i++;

            (*opAttr).count();
            (*opAttr).count();
            (*opAttr).count();

            int aux = A[i];
            A[i] = A[j];
            A[j] = aux;
        }
    }

    (*opAttr).count();
    (*opAttr).count();
    (*opAttr).count();

    int aux = A[i + 1];
    A[i + 1] = A[r];
    A[r] = aux;
    return i + 1;
}

void Quicksort(int* A, int p, int r, Operation* opComp, Operation* opAttr)
{
    if (p < r)
    {
        int  q = partition(A, p, r, opComp, opAttr);
        Quicksort(A, p, q - 1, opComp, opAttr);
        Quicksort(A, q + 1, r, opComp, opAttr);
    }
}

void afisare_vect(int* A, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", A[i]);
    }
    printf("\n");
}

void perf_average_Quick(int order)
{
    int v[MAX_SIZE], v1[MAX_SIZE], v2[MAX_SIZE];
    int n;
    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {
        for (int test = 0; test < NR_TESTS; test++)
        {
            FillRandomArray(v, n, 10, 50000, false, order);
            Operation opComp = p.createOperation("QuickSort-comp", n);
            Operation opAttr = p.createOperation("QuickSort-attr", n);
            Quicksort(v, 0, n - 1, &opComp, &opAttr);

        }
    }
    p.divideValues("QuickSort-comp", NR_TESTS);
    p.divideValues("QuickSort-attr", NR_TESTS);
    p.addSeries("QuickSort", "QuickSort-comp", "QuickSort-attr");

    

}

void perf_Quick(int order)
{
    int v[MAX_SIZE2];
    int n;
    for (n = STEP_SIZE2; n <= MAX_SIZE2; n += STEP_SIZE2)
    {
        for (int test = 0; test < NR_TESTS; test++)
        {
            FillRandomArray(v, n, 10, 50000, false, order);
            Operation opComp = p.createOperation("QuickSort-comp", n);
            Operation opAttr = p.createOperation("QuickSort-attr", n);
            Quicksort(v, 0, n - 1, &opComp, &opAttr);
        }

    }
    p.addSeries("QuickSort", "QuickSort-comp", "QuickSort-attr");
    p.showReport();

}



void perf_Quicksort()
{
    perf_Quick(ASCENDING);
    p.reset("Quicksort average case");
    perf_average_Quick(UNSORTED);
    p.reset("Quicksort best case");
    perf_Quick(DESCENDING);
    p.showReport();

}

//HEAPSORT VS QUICKSORT

void perf_Heapsort_vs_Quicksort(int order)
{
    int v[MAX_SIZE], v1[MAX_SIZE];
    int n;
    heap* h = createHeap(MAX_SIZE);
    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {
        for (int test = 0; test < NR_TESTS; test++)
        {
            FillRandomArray(v, n, 10, 50000, false, order);
            Heap_vector(h, v, n);
            Operation opComp = p.createOperation("QuickSort-comp", n);
            Operation opAttr = p.createOperation("QuickSort-attr", n);
            Quicksort(v, 0, n - 1, &opComp, &opAttr);
            heapsort(h);
        }
    }
    p.divideValues("QuickSort-comp", NR_TESTS);
    p.divideValues("QuickSort-attr", NR_TESTS);
    p.addSeries("QuickSort", "QuickSort-comp", "QuickSort-attr");

    p.divideValues("heapsort-comp", NR_TESTS);
    p.divideValues("heapsort-attr", NR_TESTS);
    p.addSeries("heapsort", "heapsort-comp", "heapsort-attr");

    p.createGroup("comp", "heapsort-comp", "QuickSort-comp");
    p.createGroup("attr", "heapsort-attr", "QuickSort-attr");
    p.createGroup("total", "heapsort", "QuickSort");

    p.showReport();

}

//HEAPSORT HYBRIDIZATION


void insertionSort(int* A, int n, Operation* opComp, Operation* opAttr)
{

    for (int i = 1; i < n; i++)
    {
        (*opAttr).count();
        int x = A[i];
        int j = i - 1;
        if (j > 0)
            (*opComp).count();//doar daca j>0 ajunge sa faca comparatia A[j]>x
        while (j >= 0 && A[j] > x)
        {
            A[j + 1] = A[j];
            j--;

            if (j > 0)
                (*opComp).count();//doar daca j>0 ajunge sa faca comparatia A[j]>x
        }
        A[j + 1] = x;
        (*opAttr).count();
    }
}

void hybrid_Quicksort(int* A, int p, int r, int threshold, Operation* opComp, Operation* opAttr)
{
    if (r - p > threshold)
    {
        int  q = partition(A, p, r, opComp, opAttr);
        hybrid_Quicksort(A, p, q - 1, threshold, opComp, opAttr);
        hybrid_Quicksort(A, q + 1, r, threshold, opComp, opAttr);
    }
    else
    {
        insertionSort(A + p, r - p + 1, opComp, opAttr);
    }
}


//  QUICKSORT VS HYBRID QUICKSORT
void perf_Hybrid_QuickSort_vs_Quicksort()
{
    int v[MAX_SIZE], v1[MAX_SIZE];
    int n;
    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {
        for (int test = 0; test < NR_TESTS; test++)
        {
            FillRandomArray(v, n, 10, 50000, false);
            for (int i = 0; i < n; i++)
            {
                v1[i] = v[i];
            }
            Operation opComp = p.createOperation("QuickSort-comp", n);
            Operation opAttr = p.createOperation("QuickSort-attr", n);
            Quicksort(v, 0, n - 1, &opComp, &opAttr);
            Operation opComp_hybrid = p.createOperation("Hybrid_QuickSort-comp", n);
            Operation opAttr_hybrid = p.createOperation("Hybrid_QuickSort-attr", n);
            hybrid_Quicksort(v1, 0, n - 1, 30, &opComp_hybrid, &opAttr_hybrid);
           
        }

    }
    p.divideValues("QuickSort-comp", NR_TESTS);
    p.divideValues("QuickSort-attr", NR_TESTS);
    p.addSeries("QuickSort", "QuickSort-comp", "QuickSort-attr");

    p.divideValues("Hybrid_QuickSort-comp", NR_TESTS);
    p.divideValues("Hybrid_QuickSort-attr", NR_TESTS);
    p.addSeries("Hybrid_QuickSort", "Hybrid_QuickSort-comp", "Hybrid_QuickSort-attr");

    p.createGroup("comp", "Hybrid_QuickSort-comp", "QuickSort-comp");
    p.createGroup("attr", "Hybrid_QuickSort-attr", "QuickSort-attr");
    p.createGroup("total", "Hybrid_QuickSort", "QuickSort");

    p.showReport();

}
/*void perf_Hybrid_QuickSort_vs_Quicksort_time(int order)
{
    int v[MAX_SIZE2], v1[MAX_SIZE2];
    int n;
    for (n = STEP_SIZE2; n <= MAX_SIZE2; n += STEP_SIZE2)
    {
        

        

            p.startTimer("Quicksort", n);
            for (int test = 0; test < 100; ++test) {
                Operation opComp = p.createOperation("QuickSort-comp", n);
                Operation opAttr = p.createOperation("QuickSort-attr", n);
                Quicksort(v, 0, n - 1, &opComp, &opAttr);
            }
            p.stopTimer("Quicksort", n);

            p.startTimer("hybrid_Quicksort", n);
            for (int test = 0; test < 100; ++test) {
                Operation opComp_hybrid = p.createOperation("Hybrid_QuickSort-comp", n);
                Operation opAttr_hybrid = p.createOperation("Hybrid_QuickSort-attr", n);
                hybrid_Quicksort(v1, 0, n - 1, 30, &opComp_hybrid, &opAttr_hybrid);
            }
            p.stopTimer("hybrid_Quicksort", n);


    }

}*/
void PERF_Hybrid_QuickSort_vs_Quicksort()
{
    perf_Hybrid_QuickSort_vs_Quicksort();
   // p.reset("Execution time:\n");
   // perf_Hybrid_QuickSort_vs_Quicksort_time(UNSORTED); 
    // p.showReport();
}


//Determination of an optimal threshold used in hybridization

void perf_Hybrid_QuickSort_threshold()
{
    int v[MAX_SIZE2],v1[MAX_SIZE2];
    int n;
        for (n = STEP_SIZE2; n <= MAX_SIZE2; n += STEP_SIZE2)
        {
            for (int test = 0; test < NR_TESTS; test++)
            {

                FillRandomArray(v, n, 10, 50000, false);

                for (int threshold = 5; threshold <= 50; threshold = threshold + 5)
                {

                    for (int i = 0; i < n; i++)
                    {
                        v1[i] = v[i];
                    }
                    Operation opComp_hybrid = p.createOperation("Hybrid_QuickSort-comp", n);
                    Operation opAttr_hybrid = p.createOperation("Hybrid_QuickSort-attr", n);

                    hybrid_Quicksort(v1, 0, n - 1, threshold, &opComp_hybrid, &opAttr_hybrid);

                     /*p.startTimer("hybrid_Quicksort", n);
                     for (int test = 0; test < 100; ++test)
                     {
                         hybrid_Quicksort(v1, 0, n - 1, threshold, &opComp_hybrid, &opAttr_hybrid);
                     }
                     p.stopTimer("hybrid_Quicksort", n);
                     */
                    
                    p.addSeries("Hybrid_QuickSort", "Hybrid_QuickSort-comp", "Hybrid_QuickSort-attr");

                   
                }
                
            }
            p.divideValues("Hybrid_QuickSort", 5);
            p.showReport();

         
        }

        p.showReport();
         
}


//Quick Select

int hoare_partition(int* A, int p, int r)
{
    int x = A[(p + r) / 2];
    int i = p;
    int j = r;
    do
    {
        while (A[i] < x)
            i++;
        while (A[j] > x)
            j--;
        if (i <= j)
        {
            int aux = A[i];
            A[i] = A[j];
            A[j] = aux;
            i++;
            j--;
        }
    } while (i < j);
    return j;
}

int QuickSelect(int* A, int p, int r, int i)
{
    if (p == r)
        return A[p];
    int q = hoare_partition(A, p, r);
    int k = q - p + 1;
    if (i <= k)
        return QuickSelect(A, p, q, i);
    else return QuickSelect(A, q + 1, r, i - k);

}


//demo

void demo_Quickselect()
{
    printf("Demo pentru QuickSelect:\n");
    int A[] = { -1,0,1,2,7,9,2,3,1,4,5,0 };
    int n = sizeof(A) / sizeof(A[0]);
    afisare_vect(A, n);
    int x = QuickSelect(A, 0, n - 1, 6);
    printf("%d \n", x);
    //afisare_vect(A, n);
}

void demo_Quicksort()
{
    printf("Demo pentru QuickSort:\n");
    int A[] = { -1,0,1,2,7,9,2,3,1,4,5,0 };
    int n = sizeof(A) / sizeof(A[0]);
    Operation opComp = p.createOperation("QuickSort-comp", n);
    Operation opAttr = p.createOperation("QuickSort-attr", n);
    afisare_vect(A, n);
    Quicksort(A, 0, n - 1, &opComp, &opAttr);
    afisare_vect(A, n);
}
void demo_HybridQuicksort()
{
    printf("Demo pentru Hybrid QuickSort:\n");
    int A[] = { 4,5,2,7,6,5,4,3,8,9,0,8,9,6,7,5,-1 };
    int n = sizeof(A) / sizeof(A[0]);
    Operation opComp = p.createOperation("QuickSort-comp", n);
    Operation opAttr = p.createOperation("QuickSort-attr", n);
    afisare_vect(A, n);
    hybrid_Quicksort(A, 0, n - 1, 3, &opComp, &opAttr);
    afisare_vect(A, n);
}



int main()
{
    //demo_heap();
   // demo_Quicksort();
   // demo_HybridQuicksort();
     //demo_Quickselect();
    //perf_Heapsort_vs_Quicksort(UNSORTED);
    //perf_Quicksort();
    //perf_Quick(DESCENDING); 
   // PERF_Hybrid_QuickSort_vs_Quicksort();
   perf_Hybrid_QuickSort_threshold();
   
     //perf_Quicksort();
    //perf_all();
    return 0;
}