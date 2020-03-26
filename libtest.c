//
// Created by bugscan on 2020/3/25.
//

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

struct Node;
struct Edge;

typedef struct Edge {
    struct Node *caller;
    struct Node *callee;
    int callCount;
} Edge;

typedef struct Node {
    char *name;
    Edge *edges[20];
    int edgeCount;
} Node;

Node *nodes[100];
int nodeCount = 0;

Node *mainNode;

Node *createOrGetNode(char *functionName) {
    for (int i = 0; i < nodeCount; i++) {
        if (strcmp(nodes[i]->name, functionName) == 0) {
            return nodes[i];
        }
    }
    Node *node = malloc(sizeof(Node));
    node->name = functionName;
    node->edgeCount = 0;
    nodes[nodeCount++] = node;
    return node;
}

void addEdge(Node *callerNode, char *callee) {
    for (int i = 0; i < callerNode->edgeCount; i++) {
        if (strcmp(callerNode->edges[i]->callee->name, callee) == 0) {
            callerNode->edges[i]->callCount++;
            return;
        }
    }

    Edge *edge = malloc(sizeof(Edge));
    edge->caller = callerNode;
    edge->callee = createOrGetNode(callee);
    edge->callCount = 1;
    callerNode->edges[callerNode->edgeCount++] = edge;
}

void printNode(Node *node, int depth) {
    for (int i = depth; i > 1; i--) {
        printf(" ");
    }
    if (depth > 0) {
        printf("└─");
    }
    printf("%s\n", node->name);
    for (int i = 0; i < node->edgeCount; i++) {
        printf("(%d)", node->edges[i]->callCount);
        printNode(node->edges[i]->callee, depth + 1);
    }
}

void __record__(char *caller, char *callee) {
    // printf("__record__ called\n");
    // printf("caller is %s \n", caller);
    // printf("callee is %s \n", callee);
    Node *callerNode = createOrGetNode(caller);
    addEdge(callerNode, callee);

    if (strcmp(caller, "main") == 0) {
        mainNode = callerNode;
    }
}

void __print__() {
    // printf("__print__ called\n");
    printNode(mainNode, 0);
}
