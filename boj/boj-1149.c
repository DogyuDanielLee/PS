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
  } else if (curr == N - 1) {
    if (dp[curr][cannot_use] != -1)
      return dp[curr][cannot_use];

    if (cannot_use == 0)
      dp[curr][cannot_use] = min2(cost[curr][1], cost[curr][2]);
    else if (cannot_use == 1)
      dp[curr][cannot_use] = min2(cost[curr][0], cost[curr][2]);
    else if (cannot_use == 2)
      dp[curr][cannot_use] = min2(cost[curr][0], cost[curr][1]);
    else {
      perror("cannotuse must be 0 or 1 or 2");
      exit(EXIT_FAILURE);
    }
    return dp[curr][cannot_use];
  } else {
    if (dp[curr][cannot_use] != -1)
      return dp[curr][cannot_use];

    if (cannot_use == 0) {
      a = cost[curr][1] + solve_dp(curr + 1, 1, cost, dp, N);
      b = cost[curr][2] + solve_dp(curr + 1, 2, cost, dp, N);
      dp[curr][cannot_use] = min2(a, b);
    } else if (cannot_use == 1) {
      a = cost[curr][0] + solve_dp(curr + 1, 0, cost, dp, N);
      b = cost[curr][2] + solve_dp(curr + 1, 2, cost, dp, N);
      dp[curr][cannot_use] = min2(a, b);
    } else if (cannot_use == 2) {
      a = cost[curr][0] + solve_dp(curr + 1, 0, cost, dp, N);
      b = cost[curr][1] + solve_dp(curr + 1, 1, cost, dp, N);
      dp[curr][cannot_use] = min2(a, b);
    } else {
      perror("cannotuse must be 0 or 1 or 2");
      exit(EXIT_FAILURE);
    }
    return dp[curr][cannot_use];
  }
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
  for (int32_t i = 0; i < N; i++)
    free(dp[i]);
  free(dp);

  if (debug)
    fclose(fin);
}