/*
BOJ # 2533
URL: https://www.acmicpc.net/problem/2533
Date: September 10, 2024
Memo: tree, dynamic programming
Reference: Ries Naver Blog (https://blog.naver.com/kks227/220793134705)
*/
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define INF 2147483647

bool debug = false;

int32_t min2(int32_t a, int32_t b) { return (a > b) ? (b) : (a); }

typedef struct _node {
  int32_t x;
  struct _node *prev;
  struct _node *next;
} Node;
Node *initNode(int32_t x) {
  Node *n = (Node *)malloc(sizeof(Node));
  n->x = x;
  n->prev = NULL;
  n->next = NULL;
  return n;
}
void freeNode(Node *n) {
  if (n != NULL)
    free(n);
}

typedef struct _vector {
  Node *front;
  Node *back;
  int32_t length;
} Vector;
Vector *initVector(void) {
  Vector *v = (Vector *)malloc(sizeof(Vector));
  v->front = NULL;
  v->back = NULL;
  v->length = 0;
  return v;
}
void freeVector(Vector *v) {
  if (v == NULL)
    return;

  Node *itr1, *itr2;
  itr1 = v->front;
  while (itr1 != NULL) {
    itr2 = itr1->next;
    freeNode(itr1);
    itr1 = itr2;
  }
  free(v);
}
void pushFront(Vector *v, int32_t x) {
  Node *newnode = initNode(x);
  if (v->length == 0) {
    v->front = newnode;
    v->back = newnode;
  } else {
    v->front->prev = newnode;
    newnode->next = v->front;
    v->front = newnode;
  }
  v->length++;
}
void pushBack(Vector *v, int32_t x) {
  Node *newnode = initNode(x);
  if (v->length == 0) {
    v->front = newnode;
    v->back = newnode;
  } else {
    v->back->next = newnode;
    newnode->prev = v->back;
    v->back = newnode;
  }
  v->length++;
}
int32_t popFront(Vector *v) {
  if (v->length == 0)
    return -1;

  int32_t popval;

  if (v->length == 1) {
    popval = v->front->x;
    freeNode(v->front);
    v->front = NULL;
    v->back = NULL;
    v->length--;
    return popval;
  }

  // v->length  >= 2
  popval = v->front->x;
  v->front = v->front->next;
  freeNode(v->front->prev);
  v->front->prev = NULL;
  v->length--;
  return popval;
}
int32_t popBack(Vector *v) {
  if (v->length == 0)
    return -1;

  int32_t popval;

  if (v->length == 1) {
    popval = v->back->x;
    freeNode(v->back);
    v->front = NULL;
    v->back = NULL;
    v->length--;
    return popval;
  }

  // v->length  >= 2
  popval = v->back->x;
  v->back = v->back->prev;
  freeNode(v->back->next);
  v->back->next = NULL;
  v->length--;
  return popval;
}

typedef struct _tree {
  int32_t *parent;
  Vector **child;
} Tree;
void _dfs(int32_t curr, bool *visited, Vector **edges, int32_t N, Tree *tree) {
  visited[curr] = true;
  Node *itr = edges[curr]->front;
  while (itr != NULL) {
    if (!visited[itr->x]) {
      pushBack(tree->child[curr], itr->x);
      tree->parent[itr->x] = curr;
      _dfs(itr->x, visited, edges, N, tree);
    }
    itr = itr->next;
  }
}
Tree *initTree(Vector **edges, int32_t N) {
  Tree *tree = (Tree *)malloc(sizeof(Tree));
  tree->parent = (int32_t *)malloc(sizeof(int32_t) * N);
  tree->child = (Vector **)malloc(sizeof(Vector *) * N);
  bool *visited = (bool *)malloc(sizeof(bool) * N);
  for (int32_t i = 0; i < N; i++) {
    tree->parent[i] = -1;
    tree->child[i] = initVector();
    visited[i] = false;
  }

  _dfs(0, visited, edges, N, tree);

  free(visited);
  return tree;
}
void freeTree(Tree *tree, int32_t N) {
  if (tree == NULL)
    return;
  if (tree->parent != NULL) {
    free(tree->parent);
  }
  if (tree->child != NULL) {
    for (int32_t i = 0; i < N; i++)
      freeVector(tree->child[i]);
  }
  free(tree);
}

int32_t solve_dp(int32_t curr, bool parent_early, Tree *tree, int32_t **dp) {
  int32_t *ret = &dp[curr][parent_early];
  if (*ret != -1)
    return *ret;

  int32_t not_pick = INF;
  int32_t pick = 1;

  Node *itr = tree->child[curr]->front;
  while (itr != NULL) {
    pick += solve_dp(itr->x, true, tree, dp);
    itr = itr->next;
  }

  if (parent_early) {
    not_pick = 0;
    itr = tree->child[curr]->front;
    while (itr != NULL) {
      not_pick += solve_dp(itr->x, false, tree, dp);
      itr = itr->next;
    }
  }
  *ret = min2(not_pick, pick);
  return *ret;
}

int main(void) {
  // file stream
  FILE *fin;
  if (debug)
    fin = fopen("input.txt", "r");
  else
    fin = stdin;

  // init
  int32_t N, u, v, ans;
  int32_t **dp;
  Vector **edges;
  Tree *tree;

  fscanf(fin, "%" SCNd32, &N);
  dp = (int32_t **)malloc(sizeof(int32_t *) * N);
  edges = (Vector **)malloc(sizeof(Vector *) * N);
  for (int32_t i = 0; i < N; i++) {
    dp[i] = (int32_t *)malloc(sizeof(int32_t) * 2);
    dp[i][0] = -1;
    dp[i][1] = -1;
    edges[i] = initVector();
  }

  // input
  for (int32_t i = 0; i < N - 1; i++) {
    fscanf(fin,
           "%" SCNd32 " "
           "%" SCNd32,
           &u, &v);
    u--;
    v--;
    pushBack(edges[u], v);
    pushBack(edges[v], u);
  }

  // make tree
  tree = initTree(edges, N);

  // solve : dynamic programming
  ans = solve_dp(0, true, tree, dp);
  printf("%" PRId32 "\n", ans);

  // clear
  for (int32_t i = 0; i < N; i++)
    free(edges[i]);
  free(edges);
  freeTree(tree, N);

  if (debug)
    fclose(fin);
}