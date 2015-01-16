#include<iostream>
#include<cmath>
using namespace std;
#define MAXVEX 101
#define FALSE 0
#define TRUE  1
#define Yes   1
#define No    0
int visited[MAXVEX];

typedef int VertexType;

/*Edge struct*/
typedef struct EdgeNode
{
	int adjvex;
	struct EdgeNode *next;
} EdgeNode;

/*Vertex struct*/
typedef struct VertexNode
{
	int adjvex;
	int dis;
	VertexType x;
	VertexType y;
	EdgeNode * firstedge;
} VertexNode, AdjList[MAXVEX];

/*Graph struct*/
typedef struct
{
	AdjList adjList;
	int numVertexes, numEdges;
}GraphList;

bool IsJump(VertexNode *a, VertexNode *b, VertexType distance)
{
	return (distance >= sqrt((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y)));
}

bool JumpOut(VertexNode *a, VertexType distance)
{
	return ((a->x + distance >= 50) || (a->x - distance <= -50) || (a->y + distance >= 50) || (a->y - distance <= -50));
}

bool JumpFirstOut(VertexNode *a, VertexType distance)
{
	return ((a->x + distance + 7.5 >= 50) || (a->x - distance - 7.5 <= -50) || (a->y + distance + 7.5 >= 50) || (a->y - distance - 7.5 <= -50));
}

bool JumpFirst(VertexNode *a, VertexNode *b, VertexType distance)
{
	return ((distance + 7.5) >= sqrt((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y)));
}

/*创建图*/
void createGraph(GraphList *g, VertexType ** array, VertexType distance, int num)
{
	EdgeNode *e;
	EdgeNode *f;
	g->numVertexes = num;
	/*init vertex*/
	for (int i = 0; i < g->numVertexes; i++)
	{
		g->adjList[i].adjvex = i;//顶点索引
		g->adjList[i].dis = 0;
		g->adjList[i].x = array[i][0];
		g->adjList[i].y = array[i][1];
		g->adjList[i].firstedge = NULL;
		//if (JumpOut(&g->adjList[i], distance))
		//{
		//	cout << g->adjList[i].adjvex << endl;
		//}
	}

	for (int i = 0; i < g->numVertexes; i++)
	{
		for (int j = i + 1; j < g->numVertexes; j++)
		{
			if (i == 0 && JumpFirst(&g->adjList[i], &g->adjList[j], distance))
			{
				e = (EdgeNode *)malloc(sizeof(EdgeNode));
				e->adjvex = j;
				e->next = g->adjList[i].firstedge;
				g->adjList[i].firstedge = e;

				f = (EdgeNode *)malloc(sizeof(EdgeNode));
				f->adjvex = i;
				f->next = g->adjList[j].firstedge;
				g->adjList[j].firstedge = f;
			}
			else if (IsJump(&g->adjList[i], &g->adjList[j], distance))
			{
				e = (EdgeNode *)malloc(sizeof(EdgeNode));
				e->adjvex = j;
				e->next = g->adjList[i].firstedge;
				g->adjList[i].firstedge = e;

				f = (EdgeNode *)malloc(sizeof(EdgeNode));
				f->adjvex = i;
				f->next = g->adjList[j].firstedge;
				g->adjList[j].firstedge = f;
			}
		}
	}
}

/*打印图（邻接表形式）*/
void printGraph(GraphList *g)
{
	int i = 0;
	while (i < g->numVertexes)
	{
		cout << "index:" << g->adjList[i].adjvex << " x-y:" << g->adjList[i].x << " " << g->adjList[i].y << "->";
		EdgeNode *e = NULL;
		e = g->adjList[i].firstedge;
		while (e != NULL)
		{
			cout << e->adjvex << " ";
			e = e->next;
		}
		cout << endl;
		i++;
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

void getShortPath(GraphList *g, int **index, int count)
{
	EdgeNode *p;
	if (count > 0)
	{
		int postion = 0;
		for (int i = 0; i <g->numVertexes; i++)
		{

			if (index[count + 1][i] > 0)
			{
				int m = index[count + 1][i];
				p = g->adjList[m].firstedge;
				while (p)
				{
					if (g->adjList[p->adjvex].dis == count)
					{
						index[count][postion++] = p->adjvex;
					}
					p = p->next;
				}
			}
		}
		getShortPath(g, index, count - 1);
	}
}

int getSmallVertex(GraphList *g,int i,int j)
{
	if (g->adjList[i].x * g->adjList[i].x + g->adjList[i].y * g->adjList[i].y > g->adjList[j].x * g->adjList[j].x + g->adjList[j].y * g->adjList[j].y)
		return j;
	return i;
}

/*广度优先搜索*/
void BFSTraverse(GraphList *g, int distance)
{
	EdgeNode *p;
	Quence *q = createQuence();
	//保存的路径索引
	int postion = 0;
	int **index = new int*[g->numVertexes];
	for (int i = 0; i <=g->numVertexes; i++)
	{
		index[i] = new int[g->numVertexes];
	}
	for (int i = 0; i < g->numVertexes; i++)
	{
		for (int j = 0; j < g->numVertexes; j++)
			index[i][j] = -1;
	}
	index[0][0] = 0;
	for (int i = 0; i < g->numVertexes; i++)
	{
		visited[i] = FALSE;
	}
	//for (int i = 0; i < g->numVertexes; i++)
	for (int i = 0; i < 1; i++)
	{
		int count = 0;
		if (!visited[i])
		{
			g->adjList[i].dis = 0;
			bool flag = false;
			//index[postion++] = i;
			visited[i] = TRUE;
			if (JumpFirstOut(&g->adjList[i], distance))
			{
				cout << "1" << endl;
				return;
			}
			//cout << "{ " << g->adjList[i].adjvex << ">" << g->adjList[i].x << "*" << g->adjList[i].y << "dis:" << g->adjList[i].dis << " ";
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
						g->adjList[p->adjvex].dis = g->adjList[m].dis + 1;
						visited[p->adjvex] = TRUE;
						//cout << g->adjList[p->adjvex].adjvex << ">" << g->adjList[p->adjvex].x << "*" << g->adjList[p->adjvex].y <<"dis:"<< g->adjList[p->adjvex].dis<<" ";
						addQuence(q, p->adjvex);
						if (JumpOut(&g->adjList[p->adjvex], distance) && !flag)
						{
							flag = true;
							count = g->adjList[p->adjvex].dis + 1;
						}
						if (JumpOut(&g->adjList[p->adjvex], distance) && (g->adjList[p->adjvex].dis + 1) == count)
							index[count-1][postion++] = p->adjvex;
					}
					p = p->next;
				}
			}
			//cout << "}"<<endl;
			//cout << count << endl;
		}
		cout << count << endl;
		if (count > 0)
		{
			getShortPath(g, index, count - 2);

			//for (int i = 0; i <= count; i++)
			//{
			//	for (int j = 0; j < g->numVertexes; j++)
			//	{
			//		if (index[i][j] >= 0)
			//		{
			//			int m = index[i][j];
			//			cout << g->adjList[m].x << " " << g->adjList[m].y << endl;
			//		}
			//	}
			//	cout << endl;
			//}

			int *po = new int[count];
			int poi = 0;
			//查找第一最小的点
			int k = index[1][0];
			for (int j = 1; j < g->numVertexes; j++)
			{
				if (index[1][j] >= 0)
				{
					k = getSmallVertex(g, k, index[1][j]);
				}
			}
			po[poi++] = k;
			EdgeNode *mx;
			for (int i = 2; i <= count; i++)
			{
				for (int j = 0; j < g->numVertexes; j++)
				{
					if (index[i][j] >= 0)
					{
						mx = g->adjList[po[poi - 1]].firstedge;
						while (mx)
						{
							if (g->adjList[mx->adjvex].adjvex == index[i][j])
								po[poi++] = index[i][j];
							mx = mx->next;
						}
					}
				}
			}
			for (int i = 0; i < poi && count != 0; i++)
			{
				cout << g->adjList[po[i]].x << " " << g->adjList[po[i]].y << endl;
			}
		}
	}
}


int main()
{
	int num, distance;
	cin >> num;
	cin >> distance;
	GraphList g;
	int **array = new int*[num + 1];
	for (int i = 0; i <= num; i++)
	{
		array[i] = new int[2];
	}
	array[0][0] = 0;
	array[0][1] = 0;
	for (int i = 1; i <= num; i++)
	{
		int x, y;
		cin >> x;
		cin >> y;
		array[i][0] = x;
		array[i][1] = y;
	}
	createGraph(&g, array, distance, num + 1);
	//printGraph(&g);
	BFSTraverse(&g,distance);

	// TestCase
	//Sample Input:
	//	17 15
	//	10 - 21
	//	10 21
	//	- 40 10
	//	30 - 50
	//	20 40
	//	35 10
	//	0 - 10
	//	- 25 22
	//	40 - 40
	//	- 30 30
	//	- 10 22
	//	0 11
	//	25 21
	//	25 10
	//	10 10
	//	10 35
	//	- 30 10
	//Sample Output:
	//	4
	//	0 11
	//	10 21
	//	10 35
	
	return 0;
}