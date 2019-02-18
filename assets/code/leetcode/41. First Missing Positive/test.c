#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int firstMissingPositive(int* nums, int numsSize)
{
    // 先计算最大和最小值
    int min_val = nums[0];
    //int min_idx = 0;
    int max_val = nums[0];
    //int max_idx = 0;
    int i = 0;
    for (i = 0; i < numsSize; ++i) {
        if (nums[i] < min_val) {
            min_val = nums[i];
        }
        if (nums[i] > max_val) {
            max_val = nums[i];
        }
    }

    // 考虑特殊情况
    if (max_val <= 0 || min_val > 1) {
        return 1;
    }

    if (min_val <= 0) {
        min_val = 1;
    }

    // 填充位图
    int num_mark = max_val - min_val + 1;
    char *mark = calloc(num_mark, sizeof(char));
    if (mark == NULL) {
        exit(EXIT_FAILURE);
    }

    int idx = 0;
    for (i = 0; i < numsSize; ++i) {
        if (nums[i] <= 0) {
            continue;
        }
        idx = nums[i] - min_val;
        mark[idx] = 1;
    }

    // 检查位图中缺失的第一个元素 
    for (i = 0; i < num_mark; ++i) {
        if (mark[i] == 0) {
            break;
        }
    }
    return i + min_val;
}

int main()
{
    int input_1[] = {1,2,0};
    fprintf(stdout, "%d\n", firstMissingPositive(input_1, sizeof(input_1) / sizeof(int)));

    int input_2[] = {3,4,-1,1};
    fprintf(stdout, "%d\n", firstMissingPositive(input_2, sizeof(input_2) / sizeof(int)));

    int input_3[] = {7,8,9,11,12};
    fprintf(stdout, "%d\n", firstMissingPositive(input_3, sizeof(input_3) / sizeof(int)));

    return 0;
}