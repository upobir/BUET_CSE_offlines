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
	rear = (rear + 1) % queueMaxSize ;   // circular queue implementation
	length-- ;
	return item ;
}


Queue::~Queue()
{
    if(data) delete[] data; //deallocate memory
    data = 0; //set to NULL
}

//****************Queue class ends here************************

//****************Dynamic ArrayList class based************************
class ArrayList
{
	int * list;
	int length ;
	int listMaxSize ;
	int listInitSize ;
public:
	ArrayList() ;
	~ArrayList() ;
	int searchItem(int item) ;
    void insertItem(int item) ;
	void removeItem(int item) ;
	void removeItemAt(int item);
	int getItem(int position) ;
	int getLength();
	bool empty();
	void printList();
} ;


ArrayList::ArrayList()
{
	listInitSize = 2 ;
	listMaxSize = listInitSize ;
	list = new int[listMaxSize] ;
	length = 0 ;
}

void ArrayList::insertItem(int newitem)
{
	int * tempList ;
	if (length == listMaxSize)
	{
		//allocate new memory space for tempList
		listMaxSize = 2 * listMaxSize ;  // doubling memory when array is full
		tempList = new int[listMaxSize] ;
		int i;
        for( i = 0; i < length ; i++ )
        {
            tempList[i] = list[i] ; //copy all items from list to tempList
        }
        delete[] list ; //free the memory allocated before
        list = tempList ; //make list to point to new memory
	};

	list[length] = newitem ; //store new item
	length++ ;
}

int ArrayList::searchItem(int item)
{
	int i = 0;
	for (i = 0; i < length; i++)
	{
		if( list[i] == item ) return i;
	}
	return NULL_VALUE;
}

void ArrayList::removeItemAt(int position) //do not preserve order of items
{
	if ( position < 0 || position >= length ) return ; //nothing to remove
	list[position] = list[length-1] ;
	length-- ;
}


void ArrayList::removeItem(int item)
{
	int position;
	position = searchItem(item) ;
	if ( position == NULL_VALUE ) return ; //nothing to remove
	removeItemAt(position) ;
}


int ArrayList::getItem(int position)
{
	if(position < 0 || position >= length) return NULL_VALUE ;
	return list[position] ;
}

int ArrayList::getLength()
{
	return length ;
}

bool ArrayList::empty()
{
    if(length==0)return true;
    else return false;
}

void ArrayList::printList()
{
    int i;
    for(i=0;i<length;i++)
        printf("%d ", list[i]);
    printf("Current size: %d, current length: %d\n", listMaxSize, length);
}

ArrayList::~ArrayList()
{
    if(list) delete [] list;
    list = 0 ;
}

//******************ArrayList class ends here*************************

//******************Graph class starts here**************************
class Graph
{
	int nVertices, nEdges ;
	bool directed ;
	ArrayList  * adjList ;
	//define other variables required for bfs such as color, parent, and dist
	//you must use pointers and dynamic allocation
	int *color;
	int *dist;
	int *parent;

public:
	Graph(bool dir = false);                            ///primarily finished
	~Graph();
	void setnVertices(int n);                           ///primarily finished
	void addEdge(int u, int v);                         ///primarily finished
	void removeEdge(int u, int v);                      ///primarily finished
	bool isEdge(int u, int v);                          ///primarily finished
    int getDegree(int u, bool out);                     ///primarily finished
    void printAdjVertices(int u);                       ///primarily finished
    bool hasCommonAdjacent(int u, int v);               ///primarily finished
    int getDist(int u, int v);                          ///primarily finished
    void printGraph();
	void bfs(int source); ///will run bfs in the graph  ///primarily finished

};


Graph::Graph(bool dir)
{
	nVertices = 0 ;
	nEdges = 0 ;
	adjList = 0 ;
	directed = dir ; //set direction of the graph
	//define other variables to be initialized
}

void Graph::setnVertices(int n)
{
	nVertices = n ;
	parent = new int[nVertices];
    color = new int[nVertices];
    dist = new int[nVertices];
	if(adjList!=0) delete[] adjList ; //delete previous list
	adjList = new ArrayList[nVertices] ;
}

void Graph::addEdge(int u, int v)
{
    if(u<0 || v<0 || u>=nVertices || v>=nVertices) return; //vertex out of range
    this->nEdges++ ;
	adjList[u].insertItem(v);
	if(!directed) adjList[v].insertItem(u) ;
}

void Graph::removeEdge(int u, int v)
{
    //write this function
    if(u<0 || v<0 || u>=nVertices || v>=nVertices) return; //vertex out of range
    nEdges--;
    adjList[u].removeItem(v);
    if(!directed) adjList[v].removeItem(u);
}

bool Graph::isEdge(int u, int v)
{
    //returns true if (u,v) is an edge, otherwise should return false
    if(u<0 || v<0 || u>=nVertices || v>=nVertices) return false;
    return (adjList[u].searchItem(v) != NULL_VALUE);
}

int Graph::getDegree(int u, bool out = true)
{
    //returns the degree of vertex u
    if(u<0 || u>=nVertices) return NULL_VALUE;
    int ret = 0;
    if(out){
        ret = adjList[u].getLength();
    }
    else{
        for(int v = 0; v<nVertices; v++)
            for(int i = 0; i<adjList[v].getLength(); i++)
                if(adjList[v].getItem(i) == u) ret++;
    }
    return ret;
}

void Graph::printAdjVertices(int u)
{
    //prints all adjacent vertices of a vertex u
    if(u<0 || u>=nVertices) return;
    for(int i = 0; i<adjList[u].getLength(); i++)
        printf("%d ", adjList[u].getItem(i));
}

bool Graph::hasCommonAdjacent(int u, int v)
{
    //returns true if vertices u and v have common adjacent vertices
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return false;
    for(int i = 0; i<adjList[u].getLength(); i++){
        if(adjList[v].searchItem(adjList[u].getItem(i)) != NULL_VALUE) return true;
    }
    return false;
}

void Graph::bfs(int source)
{
    //complete this function
    //initialize BFS variables for all n vertices first
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
        for (int i = 0; i<adjList[u].getLength(); i++){
            int v = adjList[u].getItem(i);
            if(color[v] == WHITE){
                color[v] = GREY;
                dist[v] = dist[u] + 1;
                parent[v] = u;
                q.enqueue(v);
            }
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
        for(int j=0; j<adjList[i].getLength();j++)
        {
            printf(" %d", adjList[i].getItem(j));
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
    if(adjList) delete[] adjList;
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
