/*
BOJ # 12015
URL: https://www.acmicpc.net/problem/12015
Date: August 30, 2024
Memo: segment tree
Reference: Ries Naver Blog (https://blog.naver.com/kks227/220791983209)
*/

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

bool debug = false;

typedef struct _pair {
  int32_t val;
  int32_t idx;
} Pair;

int comp_pair(const void *a, const void *b) {
  /*
  priority 1) values should be in ascending order
  priority 2) indices should be in descending order
  */
  const Pair *ap = (const Pair *)a;
  const Pair *bp = (const Pair *)b;
  if (ap->val > bp->val)
    return 1;
  else if (ap->val < bp->val)
    return -1;

  if (ap->idx < bp->idx)
    return 1;
  else if (ap->idx > bp->idx)
    return -1;

  return 0;
}

typedef struct _segment_tree {
  int32_t size;
  int32_t n_org_leaf_node; // real values
  int32_t n_all_leaf_node; // make complete binary tree
  int32_t *arr;
} SegementTree;

int32_t max_(int32_t a, int32_t b) { return (a > b) ? (a) : (b); }

int32_t _min_power_of_two(int32_t N) {
  int32_t i = 1;
  while (i < N) {
    i *= 2;
  }
  return i;
}

SegementTree *initSegmentTree(int32_t N) {
  SegementTree *s_tree = (SegementTree *)malloc(sizeof(SegementTree));
  s_tree->n_org_leaf_node = N;
  s_tree->n_all_leaf_node = _min_power_of_two(N);

  s_tree->size = 0;
  int32_t i = s_tree->n_all_leaf_node;
  s_tree->size += i;
  while (i > 0) {
    i /= 2;
    s_tree->size += i;
  }

  s_tree->arr = (int32_t *)malloc(sizeof(int32_t) *
                                  (s_tree->size + 1)); // we don't use 0th idx

  for (int32_t i = 1; i <= s_tree->size; i++)
    s_tree->arr[i] = 0;

  return s_tree;
}

void freeSegmentTree(SegementTree *s_tree) {
  if (s_tree != NULL) {
    if (s_tree->arr != NULL)
      free(s_tree->arr);
    free(s_tree);
  }
}

void updateSegmentTree(SegementTree *s_tree, int32_t idx, int32_t val) {
  idx = s_tree->n_all_leaf_node + idx;
  s_tree->arr[idx] = val;

  while (idx > 1) {
    idx /= 2;
    s_tree->arr[idx] = max_(s_tree->arr[idx * 2], s_tree->arr[idx * 2 + 1]);
  }
}

int32_t findMaxSegmentTree(SegementTree *s_tree, int32_t targetL, int32_t targetR, 
                           int32_t idx, int32_t currL, int32_t currR) {
  if (targetR < currL || currR < targetL)
    return 0;
  else if (targetL <= currL && currR <= targetR)
    return s_tree->arr[idx];

  int64_t mid = (currL + currR) / 2;
  return max_(findMaxSegmentTree(s_tree, targetL, targetR, idx * 2, currL, mid),
              findMaxSegmentTree(s_tree, targetL, targetR, idx * 2 + 1, mid + 1, currR));
}

int main(void) {
  // input mode for both debug & submit
  FILE *fin;
  if (debug)
    fin = fopen("input.txt", "r");
  else
    fin = stdin;

  // declare
  int32_t N, curr_max;
  Pair p;
  Pair *arr;
  SegementTree *s_tree;

  // input
  fscanf(fin, "%" SCNd32, &N);

  s_tree = initSegmentTree(N);
  arr = (Pair *)malloc(sizeof(Pair) * N);

  for (int32_t i = 0; i < N; i++) {
    fscanf(fin, "%" SCNd32, &p.val);
    p.idx = i;
    arr[i] = p;
  }

  // sort
  qsort(arr, N, sizeof(Pair), comp_pair);

  // solve
  for (int32_t i = 0; i < N; i++) {
    curr_max = findMaxSegmentTree(s_tree, 0, arr[i].idx, 1, 
                                  0, s_tree->n_all_leaf_node - 1);
    updateSegmentTree(s_tree, arr[i].idx, curr_max + 1);
  }

  printf("%" PRId32 "\n", s_tree->arr[1]);

  // clear
  freeSegmentTree(s_tree);
  free(arr);
  if (debug)
    fclose(fin);
}