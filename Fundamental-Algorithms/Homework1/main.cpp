/*
* Ujeniuc Denisa-Andreea 
* grupa 10
* Am implementat 3 metode de sortare a unui sir(in ordine crescatoare):Bubble sort, Selection sort, Insetion sort.
* Bubble sort si Insertion sort sunt doi algoritmi stabili, iar Selection sort nu este stabil.
* In cazul best, complexitatea Bubble sort este O(n) , Insetion sort O(n)  (dar daca am fi folosit cautarea binara complexitatea ar fi devenit O(log n)), Selection sort O(n^2);
* In cazul worst, complexitatea este aceeasi pentru toate cele 3 metode de sortare : O(n^2);
* Observam din grafice ca in cazul average , din p.d.v al atribuirilor ca metoda Insertion este mai rapida decat celelalte 2 care sunt egale ca rapiditate, iar d.p.d.v al comparatiilor, Selection sort este cea mai putin rapida, pe cand Bubble sort este cea mai rapida. In total, bubble sort ul este cea mai rapida metoda, iar Selection cea mai inceata.
* In cazul best, ca si nr de atribuiri insertion este cea mai lenta dintre metode, iar ca nr de comparatii selection este cea mai lenta. Ca nr. total de operatii, selection e cea mai lenta.
* In cazul worst, ca nr. de attr bubble e cea mai lenta, iar insertion e cea mai rapida. Ca numar de comp, selection e cea mai lenta. Iar ca nr. total de operatii ,slection e cea mai lenta.
*/

#include <stdio.h>
#include "Profiler.h"


Profiler p("sortare");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

void bubbleSort(int *A, int n)
{
    Operation opComp = p.createOperation("b-comp", n);
    Operation opAttr = p.createOperation("b-attr", n);
    int ok;
    do
    {
         ok = 1;
        for (int i = 0; i < n - 1; i++)
        {
            opComp.count();
            if (A[i + 1] < A[i])
            {
                opAttr.count();
                int aux = A[i];

                opAttr.count();
                A[i] = A[i + 1];

                opAttr.count();
                A[i + 1] = aux;

                ok = 0;
            }
            n--;
        }
    } while (ok == 0);
}

void selectionSort(int* A, int n)
{
    Operation opComp = p.createOperation("s-comp", n);
    Operation opAttr = p.createOperation("s-attr", n);
    for (int i = 0; i <= n - 1; i++)
    {
        int min = i;
        int j;
        for ( j = i; j < n; j++)
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

void insertionSort(int* A, int n)
{
    Operation opComp = p.createOperation("i-comp", n);
    Operation opAttr = p.createOperation("i-attr", n);

    for (int i = 1; i < n; i++)
    {
        opAttr.count();
        int x = A[i];
        int j = i - 1;
        if(j>0)
            opComp.count();//doar daca j>0 ajunge sa faca comparatia A[j]>x
        while (j >=0 && A[j] > x)
        {
            A[j + 1] = A[j];
            j--;

            if (j > 0)
                opComp.count();//doar daca j>0 ajunge sa faca comparatia A[j]>x
        }
        A[j+1] = x;
    }
}

void perf_average(int order)
{
    int v[MAX_SIZE], v1[MAX_SIZE], v2[MAX_SIZE];
    int n;
    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {
        for(int test=0;test<NR_TESTS;test++)
        {
            FillRandomArray(v, n, 10, 50000, false,order);
            for (int i = 0; i < n; i++)
            {
                v1[i] = v[i];
                v2[i] = v[i];
            }
            bubbleSort(v, n);
            selectionSort(v1, n);
            insertionSort(v2, n);

        }
    }
    p.divideValues("b-comp", NR_TESTS);
    p.divideValues("b-attr", NR_TESTS);
    p.addSeries("bubble", "b-comp", "b-attr");

    p.divideValues("s-comp", NR_TESTS);
    p.divideValues("s-attr", NR_TESTS);
    p.addSeries("selection", "s-comp", "s-attr");

    p.divideValues("i-comp", NR_TESTS);
    p.divideValues("i-attr", NR_TESTS);
    p.addSeries("insertion", "i-comp", "i-attr");

    p.createGroup("attr", "b-attr", "i-attr", "s-attr");
    p.createGroup("comp", "b-comp", "i-comp", "s-comp");
    p.createGroup("total", "bubble", "insertion", "selection");
    p.createGroup("Graf separat bubble sort", "b-comp", "b-attr", "bubble");
    p.createGroup("Graf separat selection sort", "s-comp", "s-attr", "selection");
    p.createGroup("Graf separat insertion sort", "i-comp", "i-attr", "insertion");


}
void perf(int order)
{
    int v[MAX_SIZE], v1[MAX_SIZE], v2[MAX_SIZE];
    int n;
    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {
       
        
            FillRandomArray(v, n, 10, 50000, false, order);
            for (int i = 0; i < n; i++)
            {
                v1[i] = v[i];
                v2[i] = v[i];
            }
            bubbleSort(v, n);
            selectionSort(v1, n);
            insertionSort(v2, n);

        
    }
    p.divideValues("b-comp", NR_TESTS);
    p.divideValues("b-attr", NR_TESTS);
    p.addSeries("bubble", "b-comp", "b-attr");

    p.divideValues("s-comp", NR_TESTS);
    p.divideValues("s-attr", NR_TESTS);
    p.addSeries("selection", "s-comp", "s-attr");

    p.divideValues("i-comp", NR_TESTS);
    p.divideValues("i-attr", NR_TESTS);
    p.addSeries("insertion", "i-comp", "i-attr");

    p.createGroup("attr", "b-attr", "i-attr", "s-attr");
    p.createGroup("comp", "b-comp", "i-comp", "s-comp");
    p.createGroup("total", "bubble", "insertion", "selection");
    p.createGroup("Graf separat bubble sort", "b-comp", "b-attr", "bubble");
    p.createGroup("Graf separat selection sort", "s-comp", "s-attr", "selection");
    p.createGroup("Graf separat insertion sort", "i-comp", "i-attr", "insertion");


}

void perf_all()
{
    perf_average(UNSORTED);
    p.reset("best");
    perf(ASCENDING);
    p.reset("worst");
    perf(DESCENDING);
    p.showReport();
}


void demo()
{
    int v[] = { 2,5,0,3,5,7 };
    int dim = sizeof(v) / sizeof(v[0]);
    int v1[6], v2[6];
    for (int i = 0; i < dim; i++)
    {
        v1[i] = v[i];
        v2[i] = v[i];
    }
    bubbleSort(v, dim);
    insertionSort(v1, dim);
    selectionSort(v2, dim);
    printf("Bublle:\n");
    for(int i=0;i<dim;i++)
    {
        printf("%d ", v[i]);

    }
    printf("\nInsertion\n");
    for (int i = 0; i < dim; i++)
    {
        printf("%d ", v1[i]);

    }
    printf("\nSelection\n");
    for (int i = 0; i < dim; i++)
    {
        printf("%d ", v2[i]);

    }
    printf("\n");
}

int main()
{
   // demo();
   perf_all();
    return 0;
}