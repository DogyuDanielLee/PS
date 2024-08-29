/*
BOJ # 17398
URL: https://www.acmicpc.net/problem/17398
Date: August 24, 2024
*/

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

bool debug = false;

typedef struct _data {
  bool active;
  int64_t node1;
  int64_t node2;
} data;

int64_t unionfind_find(int64_t *p, int64_t i) {
  if (p[i] < 0)
    return i;
  p[i] = unionfind_find(p, p[i]);
  return p[i];
}

void unionfind_union(int64_t *p, int64_t a, int64_t b) {
  a = unionfind_find(p, a);
  b = unionfind_find(p, b);
  if (a != b) {
    if (p[a] < p[b]) {
      p[a] += p[b];
      p[b] = a;
    } else {
      p[b] += p[a];
      p[a] = b;
    }
  }
}

void fill_arr(int64_t *p, int64_t N, int64_t val) {
  for (int64_t i = 0; i < N; i++)
    p[i] = val;
}

int main(void) {
  FILE *fin;
  if (debug)
    fin = fopen("input.txt", "r");
  else
    fin = stdin;

  int64_t N, M, Q, n1, n2, del_idx, cost, p1, p2;
  data *edges;
  int64_t *p;
  int64_t *q;

  fscanf(fin,
         "%" SCNd64 " "
         "%" SCNd64 " "
         "%" SCNd64,
         &N, &M, &Q);

  // init
  edges = (data *)malloc(sizeof(data) * M);
  p = (int64_t *)malloc(sizeof(int64_t) * N);
  q = (int64_t *)malloc(sizeof(int64_t) * Q);
  fill_arr(p, N, -1);

  // input
  for (int64_t i = 0; i < M; i++) {
    fscanf(fin,
           "%" SCNd64 " "
           "%" SCNd64,
           &n1, &n2);
    edges[i].active = true;
    edges[i].node1 = n1 - 1;
    edges[i].node2 = n2 - 1;
  }
  for (int64_t i = 0; i < Q; i++) {
    fscanf(fin, "%" SCNd64, &del_idx);
    del_idx--;
    edges[del_idx].active = false;
    q[i] = del_idx;
  }

  // solve
  cost = 0;
  // union all except for disabled
  for (int64_t i = 0; i < M; i++) {
    if (edges[i].active)
      unionfind_union(p, edges[i].node1, edges[i].node2);
  }
  // union one by one from the back
  for (int64_t i = Q - 1; i >= 0; i--) {
    p1 = unionfind_find(p, edges[q[i]].node1);
    p2 = unionfind_find(p, edges[q[i]].node2);
    if (p1 != p2) {
      cost += p[p1] * p[p2];
      unionfind_union(p, p1, p2);
    }
  }

  // answer
  printf("%" PRId64 "\n", cost);

  // free
  free(edges);
  free(p);
  free(q);

  if (debug)
    fclose(fin);
}