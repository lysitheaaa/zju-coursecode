#include <stdio.h>
#include <stdlib.h>

#define Max 20

typedef struct NodeType *Node;   //graph node
struct NodeType{
    int adj[1000];   //the adjacent node stored in input order
    int distance[1000];    //the distance from current node to index node
    int Nadj;   //number of adjacent nodes
};

typedef struct GraphType* Graph;   //graph struct
struct GraphType{   
    int Nv;   //number of vertices
    int Ne;   //nember of edges
    Node ver;   //node list
};

int short1 = 0, short2 = 0;    //the length of shortest and 2nd-shortest path
int nshort1=0, nshort2 = 0;    //the number of shortest and 2nd-shortest path
static int path2[Max][2000], path1[Max][2000];   //the path of shortest and 2nd-shortest path
static int pass[1001];  //the pass time of index node

void readG(Graph G);   //read the input and create the graph
void Findpath(Graph G, int v, int *path, int step);    //find the shortest and 2nd-shortest way and print out

int main(void){

    static int path[2000];   //the current path

    Graph G;
    G = (Graph)malloc(sizeof(struct GraphType));    //malloc G
    scanf("%d%d",&G->Nv, &G->Ne);   //read in Nv and Ne
    G->ver = (Node)malloc(sizeof(struct NodeType)*(G->Nv+1));
    
    readG(G);    //read input and fill the graph
    
    Findpath(G, 1, path, 0);   //find shortest and 2nd-shortest way and print out

    int i, j;
    if(short2){   //find the 2nd-shortest
        for(j=0; j<nshort2; j++){
            printf("%d", short2);
            i=0;
            while(path2[j][i]){
                printf(" %d", path2[j][i]);
                i++;
            }
            printf("\n");
        }
        
    }else printf("There is no 2nd-shortest path!\n");  //do not find 2nd-shortest path
    
    system("pause");
    return 0;
}

void readG(Graph G){      //read input and fill the graph
    int i, vi, vj, dis;
    Node N;
    for(i=0; i<G->Nv; i++)G->ver[i+1].Nadj = 0;   //clear the information
    for(i=0; i<G->Ne; i++){   //loop for Ne times

        scanf("%d%d%d", &vi, &vj, &dis);    //read in input
        G->ver[vi].adj[G->ver[vi].Nadj++] = vj;    //build adjacent list
        G->ver[vi].distance[vj] = dis;  //fill the distance
    }
    return;
}

void Findpath(Graph G, int v, int *path, int step){     //find shortest and 2nd-shortest way and print out
    int i, j;
    
    path[step] = v;   //add new path
    if(v==G->Nv&&v!=1){   //if reach the end node
        path[step+1] = 0;    //end the path
        int length = 0;     //current path's length
        for(i=0; i<step; i++){    
            length+=G->ver[path[i]].distance[path[i+1]];   //go through the path and get length
        }

        if(short1==0||short1==length&&nshort1<Max){    //if the first path or same short as current shortest path
            short1 = length;
            i=0;
            while(path[i]){             //store current path into shortest path
                path1[nshort1][i] = path[i];
                i++;
            }
            path1[nshort1][i] = 0;
            nshort1++;

        }else if(short1>length){    //if current path shorter than the shortest
            short2 = short1;
            i=0;
            nshort2 = 0;
            while(path1[i][0]&&i<Max){    //store shortest paths into 2nd-shortest path
                j=0;
     
                while(path1[i][j]){
                    path2[nshort2][j] = path1[i][j];
                    j++;
                }

                path2[nshort2][j]=0;
                nshort2++;
                i++;
            }

            short1 = length;
            i=0;
            nshort1 = 0;
            while(i<=step){    //store current path into shortest path
                path1[0][i] = path[i];
                i++;
            }
            path1[0][i] = 0;
            nshort1++;

        }else if(length<short2||short2==0){     //new 2nd-shortest
            short2 = length;
            i=0;
            while(path[i]){        //store current path into 2nd-shortest path
                path2[0][i] = path[i];
                i++;
            }
            path2[0][i] = 0;
            nshort2 = 1;
        }else if(length==short2&&nshort2<Max){      //same length with 2nd-shortest path
            i=0;
            while(path[i]){      //store current path into 2nd-shortest path
                path2[nshort2][i] = path[i];
                i++;
            }
            path2[nshort2][i] = 0;
            nshort2++;

        }
        if(G->ver[v].Nadj==0)return;
    
    }

    if(pass[v]>1)return;    //go through a node no more than 2 times
    pass[v]++;

    for(i = 0; i<G->ver[v].Nadj; i++){     //loop all the adjacent vertices
        Findpath(G, G->ver[v].adj[i], path, step+1);   //find next vertice
    }

    pass[v]--;  //v are not in current path
    return;

}
