/*
BOJ # 2042
URL: https://www.acmicpc.net/problem/2042
Date: August 29, 2024
Memo: segment tree
Reference: Ries Naver Blog (https://blog.naver.com/kks227/220791986409)
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

#define CHANGE_VALUE 1
#define CALCULATE_SUM 2

bool debug = false;

int64_t _min_power_of_two(int64_t N){
  int64_t i = 1;
  while (i < N){
    i *= 2;
  }
  return i;
}

typedef struct _segment_tree{
  int64_t size;
  int64_t n_org_leaf_node; // real values
  int64_t n_all_leaf_node; // 0 for making complete binary tree
  int64_t * arr;
} SegementTree;

SegementTree * initSegmentTree(int64_t N){
  SegementTree * s_tree = (SegementTree *) malloc(sizeof(SegementTree));
  s_tree->n_org_leaf_node = N;
  s_tree->n_all_leaf_node = _min_power_of_two(N);

  s_tree->size = 0;
  int64_t i = s_tree->n_all_leaf_node;
  s_tree->size += i;
  while (i > 0){
    i /= 2;
    s_tree->size += i;
  }

  s_tree->arr = (int64_t *) malloc(sizeof(int64_t) * (s_tree->size + 1)); // we don't use 0th idx
  return s_tree;
}

void freeSegmentTree(SegementTree * s_tree){
  if (s_tree != NULL){
    if (s_tree->arr != NULL)
      free(s_tree->arr);
    free(s_tree);
  }
}

void inputSegmentTree(SegementTree * s_tree, FILE * fin){
  int64_t cnt = 0;
  for (int64_t i = s_tree->n_all_leaf_node; i <= s_tree->size; i++){
    if (cnt >= s_tree->n_org_leaf_node)
      s_tree->arr[i] = 0;
    else{
      fscanf(fin, "%" SCNd64, &s_tree->arr[i]);
      cnt++;
    }

  }
}

void buildSegmentTree(SegementTree * s_tree){
  for (int64_t i = s_tree->n_all_leaf_node - 1; i >= 1; i--)
    s_tree->arr[i] = s_tree->arr[i * 2] + s_tree->arr[i * 2 + 1];
}

void updateSegmentTree(SegementTree * s_tree, int64_t idx, int64_t change_val){
  int64_t i = s_tree->n_all_leaf_node + idx - 1;
  s_tree->arr[i] = change_val;

  while (i > 1){
    i /= 2;
    s_tree->arr[i] = s_tree->arr[i * 2] + s_tree->arr[i * 2 + 1];
  }
}

int64_t rangeSum(SegementTree * s_tree, int64_t L, int64_t R, int64_t idx, int64_t node_L, int64_t node_R){
  if (R < node_L  || node_R < L)
    return 0;
  else if (L <= node_L && node_R <= R)
    return s_tree->arr[idx];

  int64_t mid = (node_L + node_R) / 2;
  return rangeSum(s_tree, L, R, idx * 2, node_L, mid) + rangeSum(s_tree, L, R, idx * 2 + 1, mid + 1, node_R);
}

int main(void){
  FILE * fin;
  if (debug)
    fin = fopen("input.txt", "r");
  else
    fin = stdin;

  int64_t N, M, K, op, l, r;
  SegementTree * s_tree;

  fscanf(fin, "%" SCNd64 " " "%" SCNd64 " " "%" SCNd64, &N, &M, &K);
  s_tree = initSegmentTree(N);
  inputSegmentTree(s_tree, fin);
  buildSegmentTree(s_tree);

  for (int64_t i = 0; i < M + K; i++){
    fscanf(fin, "%" SCNd64 " " "%" SCNd64 " " "%" SCNd64, &op, &l, &r);

    if (op == CHANGE_VALUE){
      updateSegmentTree(s_tree, l, r);
    }
    else if (op == CALCULATE_SUM){\
      printf("%" PRId64 "\n", rangeSum(s_tree, l, r, 1, 1, s_tree->n_all_leaf_node));
    }
    else{
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