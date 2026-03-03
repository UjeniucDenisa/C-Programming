/*Ujeniuc Denisa-Andreea, gr.10
* Tema 7
* Am implementat metoda BUILD_OS (create_TREE) care face un arbore binar de cautare echilibrat 
* Foloseste divide et impera ca sa creeze un arbore echilibrat
* Ia mijlocul si creeaza radacina curenta cu valoarea sa, apoi apeleaza pentru root->right si root->left aceeasi functie
* Compl:O(n)
* 
* Select OS cauta intr un arbore de cautare binar al i-lea cel mai mic nod, folosindu se de rangul nodului curent, care e dimens subarboreului stang+1
*Complex:O(h)=O(logn)
* 
* Delete OS sterge al i-lea cel mai mic nod din arbore, reface legaturile si scade size-ul tuturor nodurilor de deasupra aceluia pe care vrem sa il stergem
* Complex:O(h)=O(logn)
* 
* Observam din grafice :
* ca si nr total de operatii, observaam ca Build OS nu face comparatii, iar Select OS nu face atribuiri
* 
* Ca si nr total de operatii, delete e cel mai lent , iar build e cel mai rapid
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
    int size;
    struct _Node* left, * right, *parent;
} NodeT; 

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

NodeT* creeaza_nod(int key)
{
    NodeT* nd = (NodeT*)malloc(sizeof(NodeT));
    if (nd == NULL) {
        printf("Eroare la alocare");
        return NULL;
    }
    nd->key = key;
    nd->size = 1;
    nd->left = NULL;
    nd->right = NULL;
    nd->parent = NULL;
    return nd;
}
int size(NodeT* nd)
{
    if (nd == NULL)
    {
        return 0;
    }
    else
    {
        return size(nd->left) + size(nd->right) + 1;
    }
}
NodeT* create_TREE(int left, int right, Operation* opComp, Operation* opAttr)
{
    if (left > right)
    {
        return NULL;
    }
    int mij = (left + right) / 2;
    (*opAttr).count();
    (*opAttr).count();
    (*opAttr).count();
    NodeT* root = creeaza_nod(mij);
    if (root == NULL)
    {
        printf("eroare alocare nod\n");
        return NULL;
    }
    (*opAttr).count();
    root->left = create_TREE(left, mij-1,opComp,opAttr);
   // if (root->left != NULL)
    //    root->left->parent = root;
  
    (*opAttr).count();
    root->right = create_TREE(mij+1, right,opComp, opAttr);
   // if (root->right != NULL)
     //   root->right->parent = root;

    /*printf("Copilul stang a lui %d este:", root->key);
    if (root->left != NULL)
        printf("%d\n", root->left->key);
    else printf("NULL\n");

    printf("Copilul drept a lui %d este:", root->key);
    if (root->right != NULL)
        printf("%d\n", root->right->key);
    else printf("NULL\n");
    */
    root->size=size(root);
    return root;
}

void pretty_print1(NodeT* root, int nivel)
{
    if (root ==NULL)
    {
        return;
    }
    printf("%*c%d\n", nivel, ' ', root->key);//afisam "nivel" spatii si apoi cheia nodului curent
    

    if (root->left != NULL)
    {
        pretty_print1(root->left, nivel + 1);
    }

    if (root->right != NULL)
    {
        pretty_print1(root->right, nivel + 1);
    }

}

NodeT* OS_select(NodeT* root,int  i, Operation* opComp, Operation* opAttr)
{
    (*opComp).count();
    if (root == NULL) return NULL;
    int r;//rangul nodului curent
    (*opComp).count();
    if (root->left != NULL)//daca avem arbore stang
        r = root->left->size + 1;
    else r = 1;//altfel o sa fie rangul 1
    if (i == r)
    {
        return root;
    }
    else if (i < r)
    {
        return OS_select(root->left, i,opComp,opAttr);
    }
    else return OS_select(root->right, i - r, opComp, opAttr);
}


NodeT* OS_delete(NodeT* root, int i,Operation* opComp, Operation* opAttr)
{
    (*opComp).count();
    if (root == NULL) return NULL ;
    int r;//rangul nodului curent
    (*opComp).count();
    if (root->left != NULL)//daca avem arbore stang
        r = root->left->size + 1;
    else r = 1;//altfel o sa fie rangul 1
    root->size--;
    if (i < r)
    {
        (*opAttr).count();
        root->left=OS_delete(root->left, i,opComp,opAttr);
    }
    else if (i > r)
    {
        (*opAttr).count();
        root->right=OS_delete(root->right, i - r, opComp, opAttr);
    }
    else if (i == r)//am gasit nodul pe care vrem sa l stergem
    {
        (*opComp).count();
        (*opComp).count();
      
        if (root->left == NULL && root->right == NULL)//nu are copii
        {
            free(root);
            return NULL;
        }


        else 
        if (root->right != NULL && root->left==NULL)//are doar copilul drept
        {
            (*opComp).count();
            (*opComp).count();
            (*opAttr).count();
            NodeT* current = root->right;
            free(root);
            return current;
            
        }
        else if (root->left != NULL && root->right == NULL)//are doar copilul stang
        {
            (*opComp).count();
            (*opComp).count();
            (*opAttr).count();
            NodeT* current = root->left;
            free(root);
            return current;
        }
        else //are ambii copii, atunci ii cautam succesorul(cel mai mic din subarborele drept)
        {
            NodeT* it = root->right;
            (*opAttr).count();
            while (it->left != NULL)//cautam succesorul
            {
                (*opAttr).count();
                it = it->left;
                (*opAttr).count();
            }
            root->key = it->key;//ii ilocuim cheia cu cea a succesorului

            (*opAttr).count();
            root->right=OS_delete(root->right, root->size-1, opComp, opAttr);
            //return root;

        }
       
    }
    return root;
}



void perf(int order)
{
    int v[MAX_SIZE], v1[MAX_SIZE], v2[MAX_SIZE];
    int n;
    for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
    {
        for (int test = 0; test < NR_TESTS; test++)
        {
            NodeT* root = (NodeT*)malloc(sizeof(NodeT));
            Operation opComp1 = p.createOperation("build-comp", n);
            Operation opAttr1 = p.createOperation("build-attr", n);
            root = create_TREE(1, n, &opComp1, &opAttr1);
            if (root == NULL)
            {
                printf("Eroare creare arbore binar de cautare perfect echilibrat");
            }
            
            Operation opComp2 = p.createOperation("select-comp", n);
            Operation opAttr2 = p.createOperation("select-attr", n);
            Operation opComp3 = p.createOperation("delete-comp", n);
            Operation opAttr3 = p.createOperation("delete-attr", n);
            FillRandomArray(v, n, 1, n, true, UNSORTED);
            for (int i = 1; i <= n; i++)
            {
                OS_select(root, v[i], &opComp2, &opAttr2);
                OS_delete(root, v[i], &opComp3, &opAttr3);
            }

        }
    }
    p.divideValues("build-comp", NR_TESTS);
    p.divideValues("build-attr", NR_TESTS);
    p.addSeries("Build_OS", "build-comp", "build-attr");

    p.divideValues("select-comp", NR_TESTS);
    p.divideValues("select-attr", NR_TESTS);
    p.addSeries("Select_OS", "select-comp", "select-attr");

    p.divideValues("delete-comp", NR_TESTS);
    p.divideValues("delete-attr", NR_TESTS);
    p.addSeries("Delete_OS", "delete-comp", "delete-attr");

    
    p.createGroup("comp", "build-comp", "select-comp","delete-comp");
    p.createGroup("attr", "build-attr", "select-attr","delete-attr");
    p.createGroup("Total", "Build_OS", "Select_OS", "Delete_OS");

    p.createGroup("OS_Select-separat", "select-comp", "select-attr", "Select_OS");
    p.createGroup("OS_Select-comp-separat", "select-comp");
    p.createGroup("OS_Select-attr-separat", "select-attr");
    p.createGroup("OS_Select-total-separat", "Select_OS");

    p.createGroup("OS_Build-separat", "build-comp", "build-attr");
    p.showReport();

}

void demo()
{
    int n = 11;
    Operation opComp1 = p.createOperation("build-comp", n);
    Operation opAttr1 = p.createOperation("build-attr", n);
    NodeT* root = (NodeT*)malloc(sizeof(NodeT));
    root = create_TREE(1, n, &opComp1, &opAttr1);
    if (root == NULL)
    {
        printf("Eroare creare arbore binar de cautare perfect echilibrat");
    }
    //print_inordine(root);
    pretty_print1(root, 1);
    NodeT* nd1 = (NodeT*)malloc(sizeof(NodeT));
    int i = 3;
    nd1 = OS_select(root, i, &opComp1, &opAttr1);
    printf("\nAl %d-lea cel mai mic nod este %d\n", i, nd1->key);
    printf("\nDupa stergere\n");
    root = OS_delete(root, 5, &opComp1, &opAttr1);
    pretty_print1(root, 1);
}

int main()
{
  //  demo();
   perf(UNSORTED);
}