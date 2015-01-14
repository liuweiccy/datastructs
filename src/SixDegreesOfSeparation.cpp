#include<iostream>
#include<iomanip>
typedef int Boolean;
using namespace std;

#define MAXVEX 10001
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
	int distance;
	EdgeNode * firstedge;
} VertexNode,AdjList[MAXVEX];

/*Graph struct*/
typedef struct
{
	AdjList adjList;
	int numVertexes, numEdges;
}GraphList;

/*创建图*/
void createGraph(GraphList *g)
{
	EdgeNode *e;
	EdgeNode *f;
	cin >> g->numVertexes >> g->numEdges;
	/*init vertex*/
	for (int i = 1; i <= g->numVertexes; i++)
	{
		g->adjList[i].data = i;
		g->adjList[i].distance = 100;
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

		f = (EdgeNode *)malloc(sizeof(EdgeNode));
		f->adjvex = m;
		f->next = g->adjList[n].firstedge;
		g->adjList[n].firstedge = f;
	}
}

void restVisited(GraphList *g)
{
	for (int i = 0; i <= g->numVertexes; i++)
	{
		visited[i] = FALSE;
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
	restVisited(g);
		for (int i = 1; i <= g->numVertexes; i++)
		{
			restVisited(g);
			if (!visited[i])
			{
				int count = 1;
				visited[i] = TRUE;
				g->adjList[i].distance = 0;
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
							g->adjList[p->adjvex].distance = g->adjList[m].distance + 1;
							visited[p->adjvex] = TRUE;
							if (g->adjList[p->adjvex].distance <= 6) count++;
							addQuence(q, p->adjvex);
						}
						p = p->next;
					}
				}
				cout << i << ": ";
				cout << setiosflags(ios::fixed) << setprecision(2) << ((double)count * 100 / g->numVertexes) << "%" << endl;
			}
		}
}

int main()
{
	GraphList g;
	createGraph(&g);
	BFSTraverse(&g);
	return 0;
}