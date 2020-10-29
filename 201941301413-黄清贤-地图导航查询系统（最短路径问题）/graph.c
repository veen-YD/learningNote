#include<stdio.h>
#include<stdlib.h>
#define INF 2147483647;

typedef struct ArcNode {			//
	int index;				//该弧指向顶点的位置
	struct ArcNode* nextarc;				//指向下一条弧的指针
	int weight;				//网的权值指针
}ArcNode;

typedef struct VNode {
	char data[20];				//数据域
	ArcNode* firstarc;				//指向第一个表结点的地址
}VNode,AdjList[100];

typedef struct ALGraph{
	AdjList vertices;				//顶点表数组
	int vexnum, arcnum;				//图的当前顶点数和弧数
}ALGraph;

void createALGraph(ALGraph* G, FILE* fp) {
	int i, j, k, weight;
	ArcNode* e;

	if (!feof(fp)) {
		fscanf_s(fp, "%d %d", &G->vexnum, &G->arcnum);			//输入顶点数，边数
		for (i = 0; i < G->vexnum; i++) {
			fscanf_s(fp, "%s", G->vertices[i].data, 20);					//输入每个顶点的数据
			G->vertices[i].firstarc = NULL;
		}
		for (k = 0; k < G->arcnum; k++) {
			fscanf_s(fp, "%d %d %d", &i, &j, &weight);					//输入顶点下标以及之间的权值

			e = (ArcNode*)malloc(sizeof(ArcNode));				//为边表结点分配存储空间
			e->index = j;				//邻接序号为j
			e->weight = weight;
			e->nextarc = G->vertices[i].firstarc;				//将e的指针指向当前顶点上指向的结点
			G->vertices[i].firstarc = e;			//将当前顶点的指针指向e

			e = (ArcNode*)malloc(sizeof(ArcNode));
			e->index = i;
			e->weight = weight;
			e->nextarc = G->vertices[j].firstarc;
			G->vertices[j].firstarc = e;
		}
	}
}

void insertVex(ALGraph* G) {				//添加点
	int index = G->vexnum;
	G->vexnum++;
	printf("请输入插入顶点的信息:");
	scanf_s("%s", G->vertices[index].data, 20);
	G->vertices[index].firstarc = NULL;
	
	printf("\n");
}

void insertArc(ALGraph* G) {				//添加边
	int i, j, weight;
	ArcNode* e;
	printf("请输入要添加的边所在两个点的下标及权值:");
	scanf_s("%d %d %d", &i, &j, &weight);
	G->arcnum++;
	
	e = (ArcNode*)malloc(sizeof(ArcNode));
	e->index = j;
	e->weight = weight;
	e->nextarc = G->vertices[i].firstarc;
	G->vertices[i].firstarc = e;

	e = (ArcNode*)malloc(sizeof(ArcNode));
	e->index = i;
	e->weight = weight;
	e->nextarc = G->vertices[j].firstarc;
	G->vertices[j].firstarc = e;

	printf("\n");
}

void delVex(ALGraph* G) {				//删除点
	ArcNode *node1, *node2;
	int index;
	printf("请输入要删除点的下标:");
	scanf_s("%d", &index);
	if (index > G->vexnum - 1)
		printf("输入错误，已退出");

	else {
		node1 = G->vertices[index].firstarc;
		while (node1 != NULL) {				//采用循环将该结点所连的每一结点与其断开连接
			node2 = G->vertices[node1->index].firstarc;
			while (node2->index != index) {
				node2 = node2->nextarc;
			}
			if (node2 == G->vertices[node1->index].firstarc)
				G->vertices[node1->index].firstarc = NULL;
			node2 = node2->nextarc;
			node1 = node1->nextarc;
			G->arcnum--;
		}

		for (int i = 0; i < G->vexnum; i++) {
			node1 = G->vertices[i].firstarc;
			while (node1 != NULL) {
				if (node1->index > index)
					node1->index--;
				node1 = node1->nextarc;
			}
		}

		for (int i = index; i < G->vexnum - 1; i++) {
			G->vertices[i] = G->vertices[i + 1];
		}
		G->vexnum--;
	}
	printf("\n");
}

void delArc(ALGraph* G) {					//删除边
	int i, j;
	ArcNode *node;
	printf("请输入要删除的边的两点的下标:");
	scanf_s("%d %d", &i, &j);
	
	if (i > G->vexnum - 1 || j > G->vexnum - 1 || i < 0 || j < 0 || i == j)
		printf("输出错误，已退出");

	else {
		node = G->vertices[i].firstarc;
		if (node->index == j)
			G->vertices[i].firstarc = node->nextarc;
		else {
			while (node->nextarc->index != j) {
				node = node->nextarc;
			}
			node->nextarc = node->nextarc->nextarc;
		}

		node = G->vertices[j].firstarc;
		if (node->index == i)
			G->vertices[j].firstarc = node->nextarc;
		else {
			while (node->nextarc->index != i) {
				node = node->nextarc;
			}
			node->nextarc = node->nextarc->nextarc;
		}
		G->arcnum--;
	}

	printf("\n");
}

void changeVex(ALGraph* G) {				//改变点的信息
	int i;
	printf("请输入要修改点的下标:");
	scanf_s("%d", &i);
	if (i > G->vexnum - 1 || i < 0)
		printf("输入错误，已退出");
	else {
		printf("请输入要修改的值为:");
		scanf_s("%s", G->vertices[i].data, 20);
	}

	printf("\n");
}

void changeArc(ALGraph* G) {				//改变边的信息
	int i, j, weight;
	ArcNode *node;
	printf("请输入要修改的边的两点的下标以及要修改的权值:");
	scanf_s("%d %d %d", &i, &j, &weight);

	if (i > G->vexnum - 1 || j > G->vexnum - 1 || i < 0 || j < 0)
		printf("输入错误，已退出");

	else {
		node = G->vertices[i].firstarc;
		while (node->index != j) {
			node = node->nextarc;
		}
		node->weight = weight;

		node = G->vertices[j].firstarc;
		while (node->index != i) {
			node = node->nextarc;
		}
		node->weight = weight;
	}

	printf("\n");
}

int lowestWeightSum(ALGraph* G) {			//两点间最短路径（dijkstra算法）
	int i, j, k, min, tempIndex, tempWeight;
	int sorted[100], waited[100], flag[100];
	ArcNode *node;
	printf("请输入所求最小距离的两点下标:");
	scanf_s("%d %d", &i, &j);
	if (i > G->vexnum - 1 || j > G->vexnum - 1 || i < 0 || j < 0) {
		printf("输入错误，已退出");
		return -1;
	}
		
	else {
		for (k = 0; k < G->vexnum; k++) {				//给到达各点距离赋初值为INF
			waited[k] = INF;
			sorted[k] = INF;
		}
		for (k = 0; k < G->vexnum; k++) {				//定义flag数组，含义为该点是否已找出最近距离
			flag[k] = 0;
		}

		sorted[i] = 0;
		node = G->vertices[i].firstarc;
		min = INF;
		while (node != NULL) {					//计算从下标为i的结点所连的距离最近的结点
			if (node->weight < min) {
				tempIndex = node->index;
				min = node->weight;
			}
			waited[node->index] = node->weight;
			node = node->nextarc;
		}
		sorted[tempIndex] = min;
		flag[tempIndex] = 1;

		for (k = 1; k < G->vexnum; k++) {			//每次循环得到到达某个点的最近距离
			tempWeight = sorted[tempIndex];
			node = G->vertices[tempIndex].firstarc;
			min = INF;
			while (node != NULL) {
				if (node->weight + tempWeight < waited[node->index]) {
					waited[node->index] = node->weight + tempWeight;
				}
				node = node->nextarc;
			}
			for (i = 0; i < G->vexnum; i++) {
				if (waited[i] < min && flag[i]==0) {
					tempIndex = i;
					tempWeight = waited[i];
					min = waited[i];
				}
			}
			sorted[tempIndex] = tempWeight;
			flag[tempIndex] = 1;
		}
		return sorted[j];
	}

	printf("\n");
}

void output(ALGraph* G) {					//输出
	ArcNode* p1;
	for (int i = 0; i < G->vexnum; i++) {
		p1 = G->vertices[i].firstarc;
		printf("与%s相连的点分别为:   ",G->vertices[i].data);
		while (p1 != NULL) {
			printf("%s->(%d)%s   ", G->vertices[i].data, p1->weight, G->vertices[p1->index].data);
			p1 = p1->nextarc;
		}
		printf("\n\n");
	}
}

void saveFile(ALGraph* G) {				//保存至文件中
	int visited[100][100], i;
	ArcNode* node;
	FILE* fp;
	errno_t err = 0;
	err = fopen_s(&fp, "数据.txt", "w");
	if (err != 0) {
		printf("不能打开文件！\n");
		exit(0);
	}
	for (i = 0; i < G->vexnum; i++) {
		for (int j = 0; j < G->vexnum; j++) {
			visited[i][j] = 0;
		}
	}

	fprintf(fp, "%d   %d\n", G->vexnum, G->arcnum);				//保存新的顶点数，边数至文件中
	for (i = 1; i <= G->vexnum; i++) {
		fprintf(fp, "%s     ", G->vertices[i - 1].data);					//保存顶点数据至文件中
		if (i % 5 == 0)fprintf(fp, "\n");
	}
	fprintf(fp, "\n");
	for (i = 0; i < G->vexnum; i++) {
		node = G->vertices[i].firstarc;
		while (node != NULL) {
			if (visited[i][node->index] == 0&&visited[node->index][i]==0) {
				fprintf(fp, "%d  %d  %d\n", i, node->index, node->weight);					//保存两点下标以及权值至文件中
				visited[i][node->index] = 1;
				visited[node->index][i] = 1;
			}
			node = node->nextarc;
		}
	}
	if (fclose(fp)) {
		printf("不能正常关闭文件！\n");
		exit(0);
	}
}

int main() {
	ALGraph* G = (ALGraph*)malloc(sizeof(ALGraph));
	int in;
	int distance;
	printf("读取数据\n\n");
	FILE* fp;
	errno_t err = 0;
	err = fopen_s(&fp, "数据.txt", "r");
	if (err != 0) {
		printf("不能打开文件！\n");
		exit(0);
	}
	createALGraph(G, fp);
	printf("读取成功\n\n");
	if (fclose(fp)) {
		printf("不能正常关闭文件！\n");
		exit(0);
	}

	while (1) {				//进入循环
		printf("\n");
		printf("|******************************************************************************************|\n");
		printf("      1.添加点              2.添加边                    3.删除点                \n");
		printf("      4.删除边              5.修改点                    6.修改边                  \n");
		printf("      7.最短距离	    8.输出                      0.保存并退出                   \n");
		printf("|******************************************************************************************|\n");

		printf("请输入要进行的操作:");
		scanf_s("%d", &in);
		if(in == 0) {
			saveFile(G);
			break;
		}
		switch (in){
		case(1):
			insertVex(G);
			break;
		case(2):
			insertArc(G);
			break;
		case(3):
			delVex(G);
			break;
		case(4):
			delArc(G);
			break;
		case(5):
			changeVex(G);
			break;
		case(6):
			changeArc(G);
			break;
		case(7):
			distance=lowestWeightSum(G);
			printf("最短距离为%d", distance);
			break;
		case(8):
			output(G);
		default:
			break;
		}
	}
	return 0;
}