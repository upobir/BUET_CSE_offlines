#include<stdio.h>
#include<stdlib.h>
#define NULL_VALUE -999999
#define INFINITY 999999
#define WHITE 1
#define GREY 2
#define BLACK 3

class Queue
{
    int queueInitSize ;
    int queueMaxSize;
    int * data;
    int length;
    int front;
    int rear;
public:
    Queue();
    ~Queue();
    void enqueue(int item); //insert item in the queue
    int dequeue(); //returns the item according to FIFO
    bool empty(); //return true if Queue is empty
};

Queue::Queue()
{
    queueInitSize = 2 ;
    queueMaxSize = queueInitSize;
    data = new int[queueMaxSize] ; //allocate initial memory
    length = 0 ;
    front = 0;
    rear = 0;
}


void Queue::enqueue(int item)
{
	if (length == queueMaxSize)
	{
		int * tempData ;
		//allocate new memory space for tempList
		queueMaxSize = 2 * queueMaxSize ;
		tempData = new int[queueMaxSize] ;
		int i, j;
		j = 0;
		for( i = rear; i < length ; i++ )
		{
			tempData[j++] = data[i] ; //copy items from rear
		}
		for( i = 0; i < rear ; i++ )
		{
			tempData[j++] = data[i] ; //copy items before rear
		}
		rear = 0 ;
		front = length ;
		delete[] data ; //free the memory allocated before
		data = tempData ; //make list to point to new memory
	}

	data[front] = item ; //store new item
	front = (front + 1) % queueMaxSize ;
	length++ ;
}


bool Queue::empty()
{
	if(length == 0) return true ;
	else return false ;
}


int Queue::dequeue()
{
	if(length == 0) return NULL_VALUE ;
	int item = data[rear] ;
	rear = (rear + 1) % queueMaxSize ;  // circular queue implementation
	length-- ;
	return item ;
}


Queue::~Queue()
{
    if(data) delete[] data; //deallocate memory
    data = 0; //set to NULL
}

//****************Queue class ends here************************


//******************Graph class starts here**************************
class Graph
{
	int nVertices, nEdges ;
	bool directed ;
	int ** matrix ; //adjacency matrix to store the graph
	//define other variables required for bfs such as color, parent, and dist
	//you must use pointers and dynamic allocation
	int *color;
	int *dist;
	int *parent;


public:
	Graph(bool dir = false);                            ///primarily done
	~Graph();                                           ///primarily done
	void setnVertices(int n);                           ///primarily done
	void addEdge(int u, int v);                         ///primarily done
	void removeEdge(int u, int v);                      ///primarily done
	bool isEdge(int u, int v);                          ///primarily done
    int getDegree(int u, bool out);                     ///primarily done
    void printAdjVertices(int u);                       ///primarily done
    bool hasCommonAdjacent(int u, int v);               ///primarily done
    int getDist(int u, int v);                          ///primarily done
    void printGraph();
	void bfs(int source); ///will run bfs in the graph  ///primarily done
};


Graph::Graph(bool dir)
{
	nVertices = 0 ;
	nEdges = 0 ;
	matrix = 0 ;
	directed = dir ; //set direction of the graph
	//define other variables to be initialized
}

void Graph::setnVertices(int n)
{
	this->nVertices = n ;

	//allocate space for the matrix
    matrix = new int*[nVertices];
    parent = new int[nVertices];
    color = new int[nVertices];
    dist = new int[nVertices];
    for(int i=0;i<nVertices;i++)
    {
        parent[i] = NULL_VALUE;
        color[i] = NULL_VALUE;
        dist[i] = INFINITY;
        matrix[i] = new int[nVertices];
        for(int j=0;j<nVertices;j++)
            matrix[i][j] = 0; //initialize the matrix cells to 0
    }

}

void Graph::addEdge(int u, int v)
{
    //write your code here
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return;
    if(matrix[u][v] == 0) nEdges++;
    matrix[u][v] = 1;
    if(!directed) matrix[v][u] = 1;

}

void Graph::removeEdge(int u, int v)
{
    //write this function
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return;
    if(matrix[u][v] == 1) nEdges--;
    matrix[u][v] = 0;
    if(!directed) matrix[v][u] = 0;
}

bool Graph::isEdge(int u, int v)
{
    //returns true if (u,v) is an edge, otherwise should return false
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return false;
    return (matrix[u][v] == 1);
}

int Graph::getDegree(int u, bool out = true)
{
    //returns the degree of vertex u
    if(u<0 || u>=nVertices) return NULL_VALUE;
    int ret = 0;
    if(out){
        for(int v = 0; v<nVertices; v++)
            if(isEdge(u, v)) ret++;
    }
    else{
        for(int v = 0; v<nVertices; v++)
            if(isEdge(v, u)) ret++;
    }
    return ret;
}

void Graph::printAdjVertices(int u)
{
    //prints all adjacent vertices of a vertex u
    if(u<0 || u>=nVertices) return;
    for(int v = 0; v<nVertices; v++)
        if(isEdge(u, v)) printf("%d ", v);
}

bool Graph::hasCommonAdjacent(int u, int v)
{
    //returns true if vertices u and v have common adjacent vertices
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return false;
    for(int w = 0; w<nVertices; w++)
        if(isEdge(u, w) && isEdge(v, w)) return true;
    return false;
}

void Graph::bfs(int source)
{
    //write this function
    if(source<0 || source>=nVertices) return;
    for(int u = 0; u<nVertices; u++){
        color[u] = WHITE;
        dist[u] = INFINITY;
        parent[u] = NULL_VALUE;
    }
    color[source] = GREY;
    dist[source] = 0;
    Queue q;
    q.enqueue(source);
    printf("BFS: ");
    while(!q.empty()){
        int u = q.dequeue();
        printf("%d ", u);
        for (int v = 0; v<nVertices; v++)
            if(matrix[u][v] == 1 && color[v] == WHITE){
                color[v] = GREY;
                dist[v] = dist[u] + 1;
                parent[v] = u;
                q.enqueue(v);
            }
        color[u] = BLACK;
    }
    printf("\n");
}

int Graph::getDist(int u, int v)
{
    //returns the shortest path distance from u to v
    //must call bfs using u as the source vertex, then use distance array to find the distance
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return NULL_VALUE;
    bfs(u);
    return dist[v];
}


void Graph::printGraph()
{
    printf("\nNumber of vertices: %d, Number of edges: %d\n", nVertices, nEdges);
    for(int i=0;i<nVertices;i++)
    {
        printf("%d:", i);
        for(int j=0; j<nVertices;j++)
        {
            if(matrix[i][j]==1)
                printf(" %d", j);
        }
        printf("\n");
    }
}

Graph::~Graph()
{
    //write your destructor here
    if(color) delete[] color;
    if(parent) delete[] parent;
    if(dist) delete[] dist;
    if(matrix){
        for(int i = 0; i<nVertices; i++)
            delete[] matrix[i];
        delete[] matrix;
    }
}


//**********************Graph class ends here******************************


//******main function to test your code*************************
int main(void)
{
    int n;
    int choice;
    bool dir;
    printf("Enter your choice:\n");
    printf("1. directed graph   2. undirected graph\n");
    scanf("%d",&choice);
    if(choice == 1)dir = true;
    else if(choice == 2)dir = false;

    Graph g(dir);
    printf("Enter number of vertices: ");
    scanf("%d", &n);
    g.setnVertices(n);

    while(1)
    {
        printf("1. Add edge. 2. Remove edge\n");
        printf("3. Check edge 4. Get degree\n");
        printf("5. Print adjacent vertices\n");
        printf("6. Check common adjacent\n");
        printf("7. BFS  8. Get distance\n");
        printf("9. Print Graph  10. Exit.\n");

        int ch;
        scanf("%d",&ch);
        if(ch==1)
        {
            int u, v;
            scanf("%d%d", &u, &v);
            g.addEdge(u, v);
        }
        else if(ch==2)
        {
            int u, v;
            scanf("%d%d", &u, &v);
            g.removeEdge(u, v);
        }
        else if(ch==3)
        {
            int u, v;
            scanf("%d%d", &u, &v);
            if(g.isEdge(u, v)) printf("Yes, edge exists\n");
            else printf("No, there's no edge\n");
        }
        else if(ch == 4)
        {
            int u, tmp;
            scanf("%d", &u);
            printf("1. outdegree  2. indegree\n");
            scanf("%d", &tmp);
            if(tmp == 1)
                printf("degree: %d\n", g.getDegree(u, true));
            else
                printf("degree: %d\n", g.getDegree(u, false));
        }
        else if(ch==5)
        {
            int u;
            scanf("%d", &u);
            printf("adjacent vertices: ");
            g.printAdjVertices(u);
            printf("\n");
        }
        else if(ch==6)
        {
            int u, v;
            scanf("%d%d", &u, &v);
            if(g.hasCommonAdjacent(u, v))
                printf("Yes, there are common adjacents\n");
            else
                printf("No, there are no common adjacents\n");
        }
        else if(ch == 7)
        {
            int u;
            scanf("%d", &u);
            g.bfs(u);
        }
        else if(ch == 8)
        {
            int u, v;
            scanf("%d%d", &u, &v);
            int d = g.getDist(u, v);
            if(d == INFINITY)
                printf("Not reachable\n");
            else
                printf("Distance: %d\n", d);
        }
        else if(ch == 9)
        {
            g.printGraph();
        }
        else if(ch == 10)
        {
            break;
        }
    }
    return 0;
}
