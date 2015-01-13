#include<iostream>
typedef int Boolean;
using namespace std;

#define MAXVEX 11
#define TRUE 1
#define FALSE 0
Boolean visited[MAXVEX];

typedef int VertexType;
typedef int EdgeType;

/*Edge struct*/
typedef struct EdgeNode
{
	int adjvex;
	struct EdgeNode *next;
} EdgeNode;

/*Vertex struct*/
typedef struct VertexNode
{
	VertexType data;
	EdgeNode * firstedge;
} VertexNode,AdjList[MAXVEX];

/*Graph struct*/
typedef struct
{
	AdjList adjList;
	int numVertexes, numEdges;
}GraphList;

/*quickSort*/
void quickSort(int * a, int low, int high)
{
	if (low >= high) return;
	int first = low;
	int last = high;
	int key = a[first];
	while (first < last)
	{
		while (first < last && a[last] >= key)
			last--;
		a[first] = a[last];
		while (first < last && a[first] <= key)
			first++;
		a[last] = a[first];
	}
	a[first] = key;
	quickSort(a, low, first - 1);
	quickSort(a, first + 1, high);
}

/*插入调整后的数据*/
void insertIndex(GraphList *g,int m,int index)
{
	EdgeNode *e;
	e = (EdgeNode *)malloc(sizeof(EdgeNode));
	e->adjvex = index;
	e->next = g->adjList[m].firstedge;
	g->adjList[m].firstedge = e;
}

/*调整图的连接顺序（从小到大）*/
void adjustGraph(GraphList *g, int m)
{
	EdgeNode *e = g->adjList[m].firstedge;
	int p = 0;
	int array[MAXVEX];
	while (e)
	{
		array[p++] = e -> adjvex;
		e = e->next;
	}
	quickSort(array, 0, p - 1);
	g->adjList[m].firstedge = NULL;
	for (int i = p-1; i >= 0; i--)
	{
		insertIndex(g, m, array[i]);
	}
}

/*创建图*/
void createGraph(GraphList *g)
{
	EdgeNode *e;
	EdgeNode *f;
	cin >> g->numVertexes >> g->numEdges;
	/*init vertex*/
	for (int i = 0; i < g->numVertexes; i++)
	{
		g->adjList[i].data = i;
		g->adjList[i].firstedge = NULL;
	}
	for (int i = 0; i < g->numEdges; i++)
	{
		int m, n;
		cin >> m >> n;
		e = (EdgeNode *)malloc(sizeof(EdgeNode));
		e->adjvex = n;
		e->next = g->adjList[m].firstedge;
		g->adjList[m].firstedge = e;
		adjustGraph(g,m);

		f = (EdgeNode *)malloc(sizeof(EdgeNode));
		f->adjvex = m;
		f->next = g->adjList[n].firstedge;
		g->adjList[n].firstedge = f;
		adjustGraph(g, n);
	}
}

/*打印图（邻接表形式）*/
void printGraph(GraphList *g)
{
	int i = 0;

	while (i < g->numVertexes)
	{
		cout << "index:" << i << " data:" <<g->adjList[i].data <<"->";
		EdgeNode *e = NULL;
		e = g->adjList[i].firstedge;
		while (e != NULL)
		{
			cout << e->adjvex << " ";
			e = e->next;
		}
		i++;
		cout << endl;
	}
}

/*深度优先搜索*/
void DFS(GraphList *g, int i)
{
	EdgeNode *p;
	visited[i] = TRUE;
	cout << g->adjList[i].data<<" ";
	p = g->adjList[i].firstedge;
	while (p)
	{
		if (!visited[p->adjvex])
		{
			DFS(g, p->adjvex);
		}
		p = p->next;
	}
}

void DFSTraverse(GraphList* g)
{
	int i;
	for (i = 0; i < g->numVertexes; i++)
	{
		visited[i] = FALSE;
	}
	for ( i = 0; i < g->numVertexes; i++)
	{
		if (!visited[i])
		{
			cout << "{ ";
			DFS(g, i);
			cout << "}" << endl;
		}
	}
}

typedef struct {
	int a[MAXVEX];
	int f;
	int r;
} Quence;

Quence * createQuence()
{
	Quence * q = (Quence *)malloc(sizeof(Quence));
	q->f = -1;
	q->r = -1;
	return q;
}

void addQuence(Quence *q, int np)
{
	if ((q->r + 1) % MAXVEX == q->f)
		return;
	q->r = (q->r + 1) % MAXVEX;
	q->a[q->r] = np;
}

int deleteQuence(Quence *q)
{
	if (q->r == q->f)
	{
		return NULL;
	}
	else
	{
		q->f = (q->f + 1) % MAXVEX;
		return q->a[q->f];
	}
}

bool empty(Quence *q)
{
	return q->r == q->f;
}

/*广度优先搜索*/
void BFSTraverse(GraphList *g)
{
	EdgeNode *p;
	Quence *q = createQuence();
	for (int i = 0; i < g->numVertexes; i++)
	{
		visited[i] = FALSE;
	}
	for (int i = 0; i < g->numVertexes; i++)
	{
		if (!visited[i])
		{
			visited[i] = TRUE;
			cout <<"{ "<<g->adjList[i].data << " ";
			addQuence(q, i);
			while (!empty(q))
			{
				int m;
				m = deleteQuence(q);
				p = g->adjList[m].firstedge;
				while (p)
				{
					if (!visited[p->adjvex])
					{
						visited[p->adjvex] = TRUE;
						cout << g->adjList[p->adjvex].data << " ";
						addQuence(q, p->adjvex);
					}
					p = p->next;
				}
			}
			cout << "}"<<endl;
		}
	}
}

int main()
{
	GraphList g;
	createGraph(&g);
	//printGraph(&g);
	DFSTraverse(&g);
	BFSTraverse(&g);
	return 0;
}