/*
 * Date: 2022-12-09 15:15:51
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

#include "statistic.h"

static uint64_t count_total = 0;
static uint64_t count_unknow = 0;

static uint64_t count_K[8] = {0};

void increase(int size)
{
        count_total++;

        if(size <= 0 || size > 1024 * 8) {
                count_unknow++;
                return;
        }
        count_K[(size - 1) / 1024]++;
}

void dump_statistic()
{
        printf("TOTAL:%"PRIu64"    UNKNOW:%"PRIu64"\n", count_total, count_unknow);
        printf("========================\n");
        printf("0K~1K: %"PRIu64"\n", count_K[0]);
        printf("1K~2K: %"PRIu64"\n", count_K[1]);
        printf("2K~3K: %"PRIu64"\n", count_K[2]);
        printf("3K~4K: %"PRIu64"\n", count_K[3]);
        printf("4K~5K: %"PRIu64"\n", count_K[4]);
        printf("5K~6K: %"PRIu64"\n", count_K[5]);
        printf("6K~7K: %"PRIu64"\n", count_K[6]);
        printf("7K~8K: %"PRIu64"\n", count_K[7]);
}