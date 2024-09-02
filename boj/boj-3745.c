/*
BOJ # 3745
URL: https://www.acmicpc.net/problem/3745
Date: September 1, 2024
Memo: segment tree (LIS : Longest Increasing Sequence)
Reference: Ries Naver Blog (https://blog.naver.com/kks227/220791986409)
*/

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_N 100000

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

int32_t _min_power_of_two(int32_t N) {
  int32_t i = 1;
  while (i < N)
    i *= 2;
  return i;
}

void fill_array(int32_t * arr, int32_t size, int32_t val){
  for (int32_t i = 0; i < size; i++)
    arr[i] = val;
}

int32_t max_(int32_t a, int32_t b) { return (a > b) ? (a) : (b); }

int32_t findMaxSegmentTree(int32_t * s_tree_arr, int32_t twoN, int32_t idx, 
                           int32_t targetL, int32_t targetR, int32_t currL, int32_t currR){
  if (targetR < currL || currR < targetL)
    return 0;
  else if (targetL <= currL && currR <= targetR)
    return s_tree_arr[idx];

  int32_t mid = (currL + currR) / 2;

  return max_(findMaxSegmentTree(s_tree_arr, twoN, idx * 2, targetL, targetR, currL, mid),  
              findMaxSegmentTree(s_tree_arr, twoN, idx * 2 + 1, targetL, targetR, mid + 1, currR));
}

void updateSegmentTree(int32_t * s_tree_arr, int32_t twoN, int32_t idx, int32_t val){
  idx = idx + twoN;
  s_tree_arr[idx] = val;

  while (idx > 1){
    idx /= 2;
    s_tree_arr[idx] = max_(s_tree_arr[idx * 2], s_tree_arr[idx * 2 + 1]);
  }
}

int main(void) {
  // input mode for both debug & submit
  FILE *fin;
  if (debug)
    fin = fopen("input.txt", "r");
  else
    fin = stdin;

  // declare
  int32_t N, twoN, tarr_size, i, curr_max;
  int32_t * tarr; // arr for segment tree
  Pair p;
  Pair * parr; // arr for pair (index and value)

  // init considering maximum N 
  N = MAX_N;
  twoN = _min_power_of_two(N);
  i = twoN;
  tarr_size = i;
  while (i > 1){
    i /= 2;
    tarr_size += i;
  }
  tarr = (int32_t *) malloc(sizeof(int32_t) * (1 + tarr_size));
  parr = (Pair *) malloc(sizeof(Pair) * N);

  while (fscanf(fin, "%" SCNd32, &N) != EOF){
    twoN = _min_power_of_two(N);
    i = twoN;
    tarr_size = i;
    while (i > 1){
      i /= 2;
      tarr_size += i;
    }
    fill_array(tarr, tarr_size, 0);

    for (int32_t idx = 0; idx < N; idx++){
      fscanf(fin, "%" SCNd32, &p.val);
      p.idx = idx;
      parr[idx] = p;
    }

    qsort(parr, N, sizeof(Pair), comp_pair);

    for (int32_t i = 0; i < N; i++){
      curr_max = findMaxSegmentTree(tarr, twoN, 1,
                                    0, parr[i].idx, 0, twoN - 1);
      updateSegmentTree(tarr, twoN, parr[i].idx, curr_max + 1);
    }
    printf("%" PRId32 "\n", tarr[1]);
  }

  // clear
  free(tarr);
  free(parr);
  if (debug)
    fclose(fin);
}