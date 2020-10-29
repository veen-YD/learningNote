#include<stdio.h>
#include<stdlib.h>
#define INF 2147483647;

typedef struct ArcNode {			//
	int index;				//�û�ָ�򶥵��λ��
	struct ArcNode* nextarc;				//ָ����һ������ָ��
	int weight;				//����Ȩֵָ��
}ArcNode;

typedef struct VNode {
	char data[20];				//������
	ArcNode* firstarc;				//ָ���һ������ĵ�ַ
}VNode,AdjList[100];

typedef struct ALGraph{
	AdjList vertices;				//���������
	int vexnum, arcnum;				//ͼ�ĵ�ǰ�������ͻ���
}ALGraph;

void createALGraph(ALGraph* G, FILE* fp) {
	int i, j, k, weight;
	ArcNode* e;

	if (!feof(fp)) {
		fscanf_s(fp, "%d %d", &G->vexnum, &G->arcnum);			//���붥����������
		for (i = 0; i < G->vexnum; i++) {
			fscanf_s(fp, "%s", G->vertices[i].data, 20);					//����ÿ�����������
			G->vertices[i].firstarc = NULL;
		}
		for (k = 0; k < G->arcnum; k++) {
			fscanf_s(fp, "%d %d %d", &i, &j, &weight);					//���붥���±��Լ�֮���Ȩֵ

			e = (ArcNode*)malloc(sizeof(ArcNode));				//Ϊ�߱������洢�ռ�
			e->index = j;				//�ڽ����Ϊj
			e->weight = weight;
			e->nextarc = G->vertices[i].firstarc;				//��e��ָ��ָ��ǰ������ָ��Ľ��
			G->vertices[i].firstarc = e;			//����ǰ�����ָ��ָ��e

			e = (ArcNode*)malloc(sizeof(ArcNode));
			e->index = i;
			e->weight = weight;
			e->nextarc = G->vertices[j].firstarc;
			G->vertices[j].firstarc = e;
		}
	}
}

void insertVex(ALGraph* G) {				//��ӵ�
	int index = G->vexnum;
	G->vexnum++;
	printf("��������붥�����Ϣ:");
	scanf_s("%s", G->vertices[index].data, 20);
	G->vertices[index].firstarc = NULL;
	
	printf("\n");
}

void insertArc(ALGraph* G) {				//��ӱ�
	int i, j, weight;
	ArcNode* e;
	printf("������Ҫ��ӵı�������������±꼰Ȩֵ:");
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

void delVex(ALGraph* G) {				//ɾ����
	ArcNode *node1, *node2;
	int index;
	printf("������Ҫɾ������±�:");
	scanf_s("%d", &index);
	if (index > G->vexnum - 1)
		printf("����������˳�");

	else {
		node1 = G->vertices[index].firstarc;
		while (node1 != NULL) {				//����ѭ�����ý��������ÿһ�������Ͽ�����
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

void delArc(ALGraph* G) {					//ɾ����
	int i, j;
	ArcNode *node;
	printf("������Ҫɾ���ıߵ�������±�:");
	scanf_s("%d %d", &i, &j);
	
	if (i > G->vexnum - 1 || j > G->vexnum - 1 || i < 0 || j < 0 || i == j)
		printf("����������˳�");

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

void changeVex(ALGraph* G) {				//�ı�����Ϣ
	int i;
	printf("������Ҫ�޸ĵ���±�:");
	scanf_s("%d", &i);
	if (i > G->vexnum - 1 || i < 0)
		printf("����������˳�");
	else {
		printf("������Ҫ�޸ĵ�ֵΪ:");
		scanf_s("%s", G->vertices[i].data, 20);
	}

	printf("\n");
}

void changeArc(ALGraph* G) {				//�ı�ߵ���Ϣ
	int i, j, weight;
	ArcNode *node;
	printf("������Ҫ�޸ĵıߵ�������±��Լ�Ҫ�޸ĵ�Ȩֵ:");
	scanf_s("%d %d %d", &i, &j, &weight);

	if (i > G->vexnum - 1 || j > G->vexnum - 1 || i < 0 || j < 0)
		printf("����������˳�");

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

int lowestWeightSum(ALGraph* G) {			//��������·����dijkstra�㷨��
	int i, j, k, min, tempIndex, tempWeight;
	int sorted[100], waited[100], flag[100];
	ArcNode *node;
	printf("������������С����������±�:");
	scanf_s("%d %d", &i, &j);
	if (i > G->vexnum - 1 || j > G->vexnum - 1 || i < 0 || j < 0) {
		printf("����������˳�");
		return -1;
	}
		
	else {
		for (k = 0; k < G->vexnum; k++) {				//�����������븳��ֵΪINF
			waited[k] = INF;
			sorted[k] = INF;
		}
		for (k = 0; k < G->vexnum; k++) {				//����flag���飬����Ϊ�õ��Ƿ����ҳ��������
			flag[k] = 0;
		}

		sorted[i] = 0;
		node = G->vertices[i].firstarc;
		min = INF;
		while (node != NULL) {					//������±�Ϊi�Ľ�������ľ�������Ľ��
			if (node->weight < min) {
				tempIndex = node->index;
				min = node->weight;
			}
			waited[node->index] = node->weight;
			node = node->nextarc;
		}
		sorted[tempIndex] = min;
		flag[tempIndex] = 1;

		for (k = 1; k < G->vexnum; k++) {			//ÿ��ѭ���õ�����ĳ������������
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

void output(ALGraph* G) {					//���
	ArcNode* p1;
	for (int i = 0; i < G->vexnum; i++) {
		p1 = G->vertices[i].firstarc;
		printf("��%s�����ĵ�ֱ�Ϊ:   ",G->vertices[i].data);
		while (p1 != NULL) {
			printf("%s->(%d)%s   ", G->vertices[i].data, p1->weight, G->vertices[p1->index].data);
			p1 = p1->nextarc;
		}
		printf("\n\n");
	}
}

void saveFile(ALGraph* G) {				//�������ļ���
	int visited[100][100], i;
	ArcNode* node;
	FILE* fp;
	errno_t err = 0;
	err = fopen_s(&fp, "����.txt", "w");
	if (err != 0) {
		printf("���ܴ��ļ���\n");
		exit(0);
	}
	for (i = 0; i < G->vexnum; i++) {
		for (int j = 0; j < G->vexnum; j++) {
			visited[i][j] = 0;
		}
	}

	fprintf(fp, "%d   %d\n", G->vexnum, G->arcnum);				//�����µĶ��������������ļ���
	for (i = 1; i <= G->vexnum; i++) {
		fprintf(fp, "%s     ", G->vertices[i - 1].data);					//���涥���������ļ���
		if (i % 5 == 0)fprintf(fp, "\n");
	}
	fprintf(fp, "\n");
	for (i = 0; i < G->vexnum; i++) {
		node = G->vertices[i].firstarc;
		while (node != NULL) {
			if (visited[i][node->index] == 0&&visited[node->index][i]==0) {
				fprintf(fp, "%d  %d  %d\n", i, node->index, node->weight);					//���������±��Լ�Ȩֵ���ļ���
				visited[i][node->index] = 1;
				visited[node->index][i] = 1;
			}
			node = node->nextarc;
		}
	}
	if (fclose(fp)) {
		printf("���������ر��ļ���\n");
		exit(0);
	}
}

int main() {
	ALGraph* G = (ALGraph*)malloc(sizeof(ALGraph));
	int in;
	int distance;
	printf("��ȡ����\n\n");
	FILE* fp;
	errno_t err = 0;
	err = fopen_s(&fp, "����.txt", "r");
	if (err != 0) {
		printf("���ܴ��ļ���\n");
		exit(0);
	}
	createALGraph(G, fp);
	printf("��ȡ�ɹ�\n\n");
	if (fclose(fp)) {
		printf("���������ر��ļ���\n");
		exit(0);
	}

	while (1) {				//����ѭ��
		printf("\n");
		printf("|******************************************************************************************|\n");
		printf("      1.��ӵ�              2.��ӱ�                    3.ɾ����                \n");
		printf("      4.ɾ����              5.�޸ĵ�                    6.�޸ı�                  \n");
		printf("      7.��̾���	    8.���                      0.���沢�˳�                   \n");
		printf("|******************************************************************************************|\n");

		printf("������Ҫ���еĲ���:");
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
			printf("��̾���Ϊ%d", distance);
			break;
		case(8):
			output(G);
		default:
			break;
		}
	}
	return 0;
}