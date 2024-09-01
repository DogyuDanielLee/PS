/*
BOJ # 2268
URL: https://www.acmicpc.net/problem/2268
Date: September 1, 2024
Memo: segment tree
Reference: Ries Naver Blog (https://blog.naver.com/kks227/220791986409)
*/

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

bool debug = false;

typedef struct _segment_tree{
  int64_t N; // # of input array
  int64_t twoN; // # of leaf node;
  int64_t tree_size; // 1 + 2 + 4 + ... + twoN
  int64_t * arr;
} SegmentTree;

int64_t _min_power_of_two(int64_t N) {
  int64_t i = 1;
  while (i < N)
    i *= 2;
  return i;
}

SegmentTree * initSegmentTree(int64_t N){
  SegmentTree * s_tree = (SegmentTree *) malloc(sizeof(SegmentTree));
  s_tree->N = N;
  s_tree->twoN = _min_power_of_two(N);
  s_tree->tree_size = 0;
  int64_t i = s_tree->twoN;
  s_tree->tree_size += i;
  while (i > 1){
    i /= 2;
    s_tree->tree_size += i;
  }
  s_tree->arr = (int64_t *) malloc(sizeof(int64_t) * (1 + s_tree->tree_size));
  for (int64_t i = 1; i <= s_tree->tree_size; i++)
    s_tree->arr[i] = 0;
  return s_tree;
}

void freeSegmentTree(SegmentTree * s_tree){
  if (s_tree != NULL){
    if (s_tree->arr != NULL)
      free(s_tree->arr);
    free(s_tree);
  }
}

void updateSegmentTree(SegmentTree * s_tree, int64_t idx, int64_t val){
  int64_t i = s_tree->twoN + idx - 1;
  s_tree->arr[i] = val;
  while (i > 1){
    i /= 2;
    s_tree->arr[i] = s_tree->arr[i * 2] + s_tree->arr[i * 2 + 1];
  }
}

int64_t rangeSum(SegmentTree * s_tree, int64_t targetL, int64_t targetR, int64_t idx, int64_t currL, int64_t currR){
  if (currR < targetL || targetR < currL)
    return 0;
  else if (targetL <= currL && currR <= targetR)
    return s_tree->arr[idx];

  int64_t mid = (currL + currR) / 2;
  return rangeSum(s_tree, targetL, targetR, idx * 2, currL, mid) +
         rangeSum(s_tree, targetL, targetR, idx * 2 + 1, mid + 1, currR);
}

int main(void) {
  // input mode for both debug & submit
  FILE *fin;
  if (debug)
    fin = fopen("input.txt", "r");
  else
    fin = stdin;

  int64_t N, M, op, a, b;
  SegmentTree * s_tree;

  fscanf(fin, "%" SCNd64 " " "%" SCNd64, &N, &M);

  s_tree = initSegmentTree(N);

  for (int64_t m = 0; m < M; m++){
    fscanf(fin, "%" SCNd64 " "
                "%" SCNd64 " "
                "%" SCNd64, &op, &a, &b);
    if (op == 0){ // sum
      if (a <= b)
        printf("%" PRId64 "\n", rangeSum(s_tree, a, b, 1, 1, s_tree->twoN));
      else
        printf("%" PRId64 "\n", rangeSum(s_tree, b, a, 1, 1, s_tree->twoN));
    }
    else if (op == 1){ // modify
      updateSegmentTree(s_tree, a, b);
    }
    else{
      perror("op should be either 0 or 1");
      exit(EXIT_FAILURE);
    }
  }

  // clear
  freeSegmentTree(s_tree);
  if (debug)
    fclose(fin);
}