/*
BOJ # 1149
URL: https://www.acmicpc.net/problem/1149
Date: September 9, 2024
Memo: dynamic programming
Reference:
*/

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define INF 2147483647

bool debug = false;

int32_t min2(int32_t a, int32_t b) { return (a > b) ? (b) : (a); }
int32_t min3(int32_t a, int32_t b, int32_t c) { return min2(min2(a, b), c); }

int32_t solve_dp(int32_t curr, int32_t cannot_use, int32_t **cost, int32_t **dp,
                 int32_t N) {
  int32_t a, b, c;
  if (curr == 0) {
    a = cost[curr][0] + solve_dp(curr + 1, 0, cost, dp, N);
    b = cost[curr][1] + solve_dp(curr + 1, 1, cost, dp, N);
    c = cost[curr][2] + solve_dp(curr + 1, 2, cost, dp, N);
    return min3(a, b, c);
  }

  int32_t * ret = &dp[curr][cannot_use];
  if (*ret != -1)
    return *ret;

  *ret = INF;
  if (curr == N - 1){
    for (int i = 0; i < 3; i++){
      if (i != cannot_use)
        * ret = min2(*ret, cost[curr][i]);
    }
  }
  else{
    for (int i = 0; i < 3; i++){
      if (i != cannot_use){
        *ret = min2(*ret, cost[curr][i] + solve_dp(curr + 1, i, cost, dp, N));
      }
    }
  }
  return *ret;
}

int main(void) {
  FILE *fin;
  if (debug)
    fin = fopen("input.txt", "r");
  else
    fin = stdin;

  int32_t N, ans;
  int32_t **cost;
  int32_t **dp;

  // input & initialize
  fscanf(fin, "%" SCNd32, &N);
  dp = (int32_t **)malloc(sizeof(int32_t *) * N);
  cost = (int32_t **)malloc(sizeof(int32_t *) * N);
  for (int32_t i = 0; i < N; i++) {
    dp[i] = (int32_t *)malloc(sizeof(int32_t) * 3);
    cost[i] = (int32_t *)malloc(sizeof(int32_t) * 3);

    fscanf(fin,
           "%" SCNd32 " "
           "%" SCNd32 " "
           "%" SCNd32,
           &cost[i][0], &cost[i][1], &cost[i][2]);
    dp[i][0] = -1;
    dp[i][1] = -1;
    dp[i][2] = -1;
  }

  // solve
  ans = solve_dp(0, -1, cost, dp, N);
  printf("%" PRId32 "\n", ans);

  // clear
  for (int32_t i = 0; i < N; i++){
    free(dp[i]);
    free(cost[i]);
  }
  free(dp);
  free(cost);
  
  if (debug)
    fclose(fin);
}