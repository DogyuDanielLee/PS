/*
BOJ # 2357
URL: https://www.acmicpc.net/problem/2357
Date: August 30, 2024
Memo: segment tree
Reference: Ries Naver Blog (https://blog.naver.com/kks227/220791986409)
*/

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define CHANGE_VALUE 1
#define CALCULATE_PRODUCT 2

#define MAX_SEGMENT_TREE 0
#define MIN_SEGMENT_TREE 1

bool debug = false;

int64_t _min_power_of_two(int64_t N) {
  int64_t i = 1;
  while (i < N) {
    i *= 2;
  }
  return i;
}

int64_t min_(int64_t a, int64_t b) { return (a > b) ? (b) : (a); }

int64_t max_(int64_t a, int64_t b) { return (a > b) ? (a) : (b); }

typedef struct _segment_tree {
  int64_t size;
  int64_t n_org_leaf_node; // real values
  int64_t n_all_leaf_node; // make complete binary tree
  int64_t *arr;
  bool mode; // max or min
} SegementTree;

SegementTree *initSegmentTree(int64_t N, bool mode_maxmin) {
  SegementTree *s_tree = (SegementTree *)malloc(sizeof(SegementTree));
  s_tree->n_org_leaf_node = N;
  s_tree->n_all_leaf_node = _min_power_of_two(N);

  s_tree->size = 0;
  int64_t i = s_tree->n_all_leaf_node;
  s_tree->size += i;
  while (i > 0) {
    i /= 2;
    s_tree->size += i;
  }

  s_tree->arr = (int64_t *)malloc(sizeof(int64_t) *
                                  (s_tree->size + 1)); // we don't use 0th idx

  s_tree->mode = mode_maxmin;
  return s_tree;
}

void freeSegmentTree(SegementTree *s_tree) {
  if (s_tree != NULL) {
    if (s_tree->arr != NULL)
      free(s_tree->arr);
    free(s_tree);
  }
}

void inputSegmentTree(SegementTree *s_tree, int64_t *arr) {
  int64_t cnt = 0;
  for (int64_t i = s_tree->n_all_leaf_node; i <= s_tree->size; i++) {
    if (cnt >= s_tree->n_org_leaf_node) {
      if (s_tree->mode == MAX_SEGMENT_TREE)
        s_tree->arr[i] = 0;
      else // s_tree->mode == MIN_SEGMENT_TREE
        s_tree->arr[i] = 1000000001;
    } else {
      s_tree->arr[i] = arr[cnt++];
    }
  }
}

void buildSegmentTree(SegementTree *s_tree) {
  for (int64_t i = s_tree->n_all_leaf_node - 1; i >= 1; i--) {
    if (s_tree->mode == MAX_SEGMENT_TREE)
      s_tree->arr[i] = max_(s_tree->arr[i * 2], s_tree->arr[i * 2 + 1]);
    else
      s_tree->arr[i] = min_(s_tree->arr[i * 2], s_tree->arr[i * 2 + 1]);
  }
}

int64_t findSegmentTree(SegementTree *s_tree, int64_t L, int64_t R, int64_t idx,
                        int64_t node_L, int64_t node_R) {
  if (s_tree->mode == MAX_SEGMENT_TREE) {
    if (R < node_L || node_R < L)
      return 0;
    else if (L <= node_L && node_R <= R)
      return s_tree->arr[idx];

    int64_t mid = (node_L + node_R) / 2;
    return max_(findSegmentTree(s_tree, L, R, idx * 2, node_L, mid),
                findSegmentTree(s_tree, L, R, idx * 2 + 1, mid + 1, node_R));
  } else {
    if (R < node_L || node_R < L)
      return 1000000001;
    else if (L <= node_L && node_R <= R)
      return s_tree->arr[idx];

    int64_t mid = (node_L + node_R) / 2;
    return min_(findSegmentTree(s_tree, L, R, idx * 2, node_L, mid),
                findSegmentTree(s_tree, L, R, idx * 2 + 1, mid + 1, node_R));
  }
}

int main(void) {
  FILE *fin;
  if (debug)
    fin = fopen("input.txt", "r");
  else
    fin = stdin;

  int64_t N, M, l, r, minval, maxval;
  SegementTree *s_max_tree;
  SegementTree *s_min_tree;
  int64_t *temp_arr;

  fscanf(fin,
         "%" SCNd64 " "
         "%" SCNd64,
         &N, &M);
  s_max_tree = initSegmentTree(N, MAX_SEGMENT_TREE);
  s_min_tree = initSegmentTree(N, MIN_SEGMENT_TREE);
  temp_arr = (int64_t *)malloc(sizeof(int64_t) * N);

  for (int64_t i = 0; i < N; i++)
    fscanf(fin, "%" SCNd64, &temp_arr[i]);
  inputSegmentTree(s_max_tree, temp_arr);
  inputSegmentTree(s_min_tree, temp_arr);
  free(temp_arr);

  buildSegmentTree(s_max_tree);
  buildSegmentTree(s_min_tree);

  for (int64_t i = 0; i < M; i++) {
    fscanf(fin,
           "%" SCNd64 " "
           "%" SCNd64,
           &l, &r);
    minval =
        findSegmentTree(s_min_tree, l, r, 1, 1, s_min_tree->n_all_leaf_node);
    maxval =
        findSegmentTree(s_max_tree, l, r, 1, 1, s_min_tree->n_all_leaf_node);
    printf("%" PRId64 " "
           "%" PRId64 "\n",
           minval, maxval);
  }

  // clear
  freeSegmentTree(s_max_tree);
  freeSegmentTree(s_min_tree);
  if (debug)
    fclose(fin);
  return 0;
}