/*
BOJ # 12837
URL: https://www.acmicpc.net/problem/12837
Date: August 30, 2024
Memo: segment tree
Reference: Ries Naver Blog (https://blog.naver.com/kks227/220791986409)
*/

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ADD_VALUE 1
#define CALCULATE_SUM 2

bool debug = false;

int64_t _min_power_of_two(int64_t N) {
  int64_t i = 1;
  while (i < N) {
    i *= 2;
  }
  return i;
}

typedef struct _segment_tree {
  int64_t size;
  int64_t n_org_leaf_node; // real values
  int64_t n_all_leaf_node; // make complete binary tree
  int64_t *arr;
} SegementTree;

SegementTree *initSegmentTree(int64_t N) {
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

  for (int64_t i = 1; i <= s_tree->size; i++)
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

void updateSegmentTree(SegementTree *s_tree, int64_t idx, int64_t val) {
  idx = s_tree->n_all_leaf_node + idx - 1;
  s_tree->arr[idx] += val;

  while (idx > 1) {
    idx /= 2;
    s_tree->arr[idx] = s_tree->arr[idx * 2] + s_tree->arr[idx * 2 + 1];
  }
}

int64_t rangeSum(SegementTree *s_tree, int64_t targetL, int64_t targetR,
                 int64_t idx, int64_t currL, int64_t currR) {
  if (currR < targetL || targetR < currL)
    return 0;
  else if (targetL <= currL && currR <= targetR)
    return s_tree->arr[idx];

  int64_t mid = (currL + currR) / 2;
  return rangeSum(s_tree, targetL, targetR, idx * 2, currL, mid) +
         rangeSum(s_tree, targetL, targetR, idx * 2 + 1, mid + 1, currR);
}

int main(void) {
  FILE *fin;
  if (debug)
    fin = fopen("input.txt", "r");
  else
    fin = stdin;

  int64_t N, Q, op, a, b;
  SegementTree *s_tree;

  fscanf(fin,
         "%" SCNd64 " "
         "%" SCNd64,
         &N, &Q);

  s_tree = initSegmentTree(N);

  for (int64_t q = 0; q < Q; q++) {
    fscanf(fin,
           "%" SCNd64 " "
           "%" SCNd64 " "
           "%" SCNd64,
           &op, &a, &b);
    if (op == ADD_VALUE) {
      updateSegmentTree(s_tree, a, b);
    } 
    else if (op == CALCULATE_SUM) {
      printf("%" PRId64 "\n",
             rangeSum(s_tree, a, b, 1, 1, s_tree->n_all_leaf_node));
    } 
    else {
      perror("wrong value for op - only 1 and 2 are valid");
      exit(EXIT_FAILURE);
    }
  }

  // clear
  freeSegmentTree(s_tree);
  if (debug)
    fclose(fin);
  return 0;
}