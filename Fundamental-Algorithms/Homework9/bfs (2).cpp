/*Ujeniuc Denisa-Andreea, grupa 10
Am completat functia get_neighbors() parcurgand pe rand cei 4 vecinii nodului respectiv si punandu-i in vectorul neighb
Am verificat ca fiecare vecin sa nu defpaseasca marginile matricei si sa nu fie intr-un perete

Functia bfs creeaza o coada pentru a stoca nodurile pe care urmeaza sa le vizitam.
prima data adaugam nodul de start in coada si il marcam ca vizitat(culoarea gri)
apoi repetam urmatoarele pana cand stiva devine goala(nu mai avem noduri de vizitat):
    -scoatem un nod din coada
    -ii vizitam toti vecinii(care nu au fost vizitati) si ii marcam ca vizitati si ii adaugam in coada
    -dupa ce terminam cu un nod in marcam ca negru
O(V+E)

La bfs_tree am folosit vectorul repr care contine coordonatelor tututor nodurilor
Am utilizat functia pretty_print_R1 care este recursiva si pentru fiecare apel avem un parametru "nivel" care decide numarul de spatii care
vor fi afisate.
Incepem cu radacina si ii afisam coordonatele si apoi pentru fiecare din copiii sai apelam functia recursiv

*/
#include <stdlib.h>
#include <string.h>
#include "bfs.h"
#include <queue>

int get_neighbors(const Grid *grid, Point p, Point neighb[])
{
    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4

    //neighb[0]=vecinul din stanga
    //neighb[1]=vecinum de sus
    //neighb[2]=vecinul din dreapta
    //neighb[3]=vecinul de jos

   
    int nr = 0;

    //sus
    if (p.row - 1 >= 0 && grid->mat[p.row - 1][p.col] == 0)
    {
        neighb[nr].row = p.row - 1;
        neighb[nr].col = p.col;
        nr++;
    }

    //dr
    if (p.col + 1 < grid->cols && grid->mat[p.row][p.col + 1] == 0)
    {
        neighb[nr].row = p.row;
        neighb[nr].col = p.col + 1;
        nr++;
    }

    //jos
    if (p.row + 1 < grid->rows && grid->mat[p.row + 1][p.col] == 0)
    {
        neighb[nr].row = p.row + 1;
        neighb[nr].col = p.col;
        nr++;
    }

    //stanga
    if (p.col - 1>=0 && grid->mat[p.row ][p.col - 1]==0)
    {
        neighb[nr].row = p.row;
        neighb[nr].col = p.col - 1;
        nr++;
    }


    return nr;
}

void grid_to_graph(const Grid *grid, Graph *graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(grid->mat[i][j] == 0){
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }else{
                nodes[i][j] = NULL;
            }
        }
    }

    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(nodes[i][j] != NULL){
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for(i=0; i<graph->nrNodes; ++i){
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if(graph->v[i]->adjSize != 0){
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for(j=0; j<graph->v[i]->adjSize; ++j){
                if( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0){
                        graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if(k < graph->v[i]->adjSize){
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph *graph)
{
    if(graph->v != NULL){
        for(int i=0; i<graph->nrNodes; ++i){
            if(graph->v[i] != NULL){
                if(graph->v[i]->adj != NULL){
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

int findIndex(Graph* graph, Node* node)
{
    for (int i = 0; i < graph->nrNodes; i++)
    {
        if (graph->v[i] == node)
        {
            return i;
        }
    }
    return -1;//daca nu gasim nodul
}


/*typedef struct sn
{
    //int key;
    Node* ndd;
    struct sn* next;
} node;

typedef struct
{
    node* head, * tail;
    int capacity, size;
} Que;

Que* createQue()
{
    Que* Q = (Que*)malloc(sizeof(Que));
    Q->head = NULL;
    Q->tail = NULL;
    return Q;
}
void enqueue(Que *q, Node *k)//k e nodul din graf pe care vrem sa l adaugam(in loc de key)
{
    node* p = (node*)malloc(sizeof(node));
    p->next = NULL;
    p->ndd = k;
    if (q->head == NULL)
    {
        q->head = p;
        q->tail = p;
    }
    else
    {
        q->tail->next = p;
        q->tail = p;
    }
}
Node* dequeue(Que* q)
{
    if (q->head == NULL) return NULL;  

    node* temp = q->head;  
    Node* retNode = temp->ndd;  

    q->head=q->head->next; 

    if (q->head == NULL)  
    {
        q->tail = NULL;
    }

    free(temp);  
    return retNode;  
}

int emptyQue(Que *q)
{
    return (q->head == NULL);
}

void freeQue(Que* q)
{
    while (emptyQue(q) != 1)
    {
        dequeue(q);
    }
}*/

void bfs(Graph *graph, Node *s, Operation *op)
{
    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();


    int queue[MAX_ROWS * MAX_COLS];
    int q_tail = 0;
    int q_head = 0;
    Node* current;

    //initializam culoarea tututot nodurilor ca alb
    for (int i = 0; i < graph->nrNodes; i++)
    {
        graph->v[i]->color = COLOR_WHITE;
        graph->v[i]->dist = -2;
        graph->v[i]->parent = NULL;
    }

    s->color = COLOR_GRAY;
    s->dist = 0;
    s->parent = NULL;

   int s_index= findIndex(graph, s);
   if (s_index != -1)
       queue[q_tail++] = s_index;

    while (q_head !=q_tail)
    {
        current = graph->v[queue[q_head++]];
        //(*op).count();

        //adaugam vecinii
        for (int i = 0; i < current->adjSize; i++)
        {
            Node* vecin = current->adj[i];
            if (vecin!=NULL&&vecin->color == COLOR_WHITE)//vrem sa l luam in cosiderare doar daca e alb
            {
                vecin->color = COLOR_GRAY;
                vecin->parent = current;
                vecin-> dist = current->dist + 1;
         

               //adaugam vecinul in coada
                int vecinIndex = findIndex(graph, vecin);
                if (vecinIndex != -1) {
                    queue[q_tail++] = vecinIndex;
                }
            }
        }
        current->color = COLOR_BLACK;
    }
    

    //Varianta 2 coada
    /*Que* Q = createQue();
    Node* current;
    //initializam culoarea tututot nodurilor ca alb
    for (int i = 0; i < graph->nrNodes; i++)
    {
        graph->v[i]->color = COLOR_WHITE;
        graph->v[i]->dist = -2;
        graph->v[i]->parent = NULL;
    }

    s->color = COLOR_GRAY;
    s->dist = 0;
    s->parent = NULL;//?
   if(s!=NULL)
    enqueue(Q, s);
   while (emptyQue(Q)!=1)
   {
       current = dequeue(Q);
       //op
       //(*op).count();

       //adaugam vecinii
       for (int i = 0; i < current->adjSize; i++)
       {
           Node* vecin = current->adj[i];
           if (vecin != NULL && vecin->color == COLOR_WHITE)//vrem sa l luam in cosiderare doar daca e alb
           {
               vecin->color = COLOR_GRAY;
               vecin->parent = current;
               vecin->dist = current->dist + 1;


               //adaugam vecinul in coada
               enqueue(Q, vecin);
           }
       }
       current->color = COLOR_BLACK;
   }
   freeQue(Q);
   */
}


void pretty_print_R1(int* v, int n, int nivel, int nod_crrnt, Point *repr)
{
   // if(repr[nod_crrnt].row!=NULL&& repr[nod_crrnt].col!=NULL)
    printf("%*c(%d, %d)\n", nivel, ' ', repr[nod_crrnt].row, repr[nod_crrnt].col);
    for (int i = 0; i < n; i++)
    {
        if (v[i] == nod_crrnt)
        {
            pretty_print_R1(v, n, nivel + 1, i, repr);
        }
    }
}
void R1(int* v, int n, Point *repr)
{
    printf("Pretty print pentru R1:\n");
    int r = 0;
    int i = 0;
    for (i = 0; i < n; i++)
    {
        if (v[i] == -1)
        {
           // r = i + 1;
            r = i;
            break;
        }
    }
    if (i == n)
    {
        printf("Nu am gasit radacina, vector incorect\n");
        return;
    }
    pretty_print_R1(v, n, 1, r,repr);
}

void print_bfs_tree(Graph *graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for(int i=0; i<graph->nrNodes; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            transf[i] = n;
            ++n;
        }else{
            transf[i] = -1;
        }
    }
    if(n == 0){
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for(int i=0; i<graph->nrNodes && !err; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            if(transf[i] < 0 || transf[i] >= n){
                err = 1;
            }else{
                repr[transf[i]] = graph->v[i]->position;
                if(graph->v[i]->parent == NULL){
                    p[transf[i]] = -1;
                }else{
                    err = 1;
                    for(int j=0; j<graph->nrNodes; ++j){
                        if(graph->v[i]->parent == graph->v[j]){
                            if(transf[j] >= 0 && transf[j] < n){
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if(!err){
        
        R1(p, n,repr);

    }

   /* for (int i = 0; i < n; i++) {
        printf("p[%d] = %d, repr[%d] = (%d, %d)\n", i, p[i], i, repr[i].row, repr[i].col);
    }*/
    

    if(p != NULL){
        free(p);
        p = NULL;
    }
    if(repr != NULL){
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[])
{
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000
    return -1;
}

void performance()
{
    int n, i;
    Profiler p("bfs");
    
    // vary the number of edges
    for(n=1000; n<=4500; n+=100){
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected
        //graph.nrNodes<n
        for (int i = 1; i <graph.nrNodes; i++)
        {
            graph.v[i-1]->adj[0] = graph.v[i];
            graph.v[i - 1]->adjSize++;
        }
        graph.v[graph.nrNodes - 1]->adj[0] = graph.v[0];
        graph.v[graph.nrNodes - 1]->adjSize++;
        
        int muchii_ramase = n - graph.nrNodes + 1;
        while (muchii_ramase != 0)
        {
            for (int i = 0; i < graph.nrNodes; i++)
            {
                for (int j = 1; j < graph.nrNodes; j++)
                {
                    graph.v[i]->adj[j] = graph.v[j];
                    muchii_ramase--;
                }
            }
            
        }

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for(n=100; n<=200; n+=10){
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}
