#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_SIZE (4096 * 2)

char* countAndSay(int n) {
    char buf0[INIT_SIZE] = "1";
    char buf1[INIT_SIZE];
    char *pre = buf0;
    char *res = buf1;

    while (n > 1) {
        int i = 0;
        int len = strlen(pre);
        char cur = 0;
        int num = 0;
        int offset = 0;
        for (i = 0; i < len; ++i) {
            if (cur != pre[i]) {
                if (i > 0) {
                    int sp_res = snprintf(res + offset, INIT_SIZE - offset, "%d%c", num, cur);
                    offset += sp_res;
                    //fprintf(stdout, "num = %d, cur = %c, sp_res = %d\n", num, cur, sp_res);
                }
                num = 1;
                cur = pre[i];
            }
            else {
                ++num;
            }
        }

        // 写最后一段
        snprintf(res + offset, INIT_SIZE - offset, "%d%c", num, cur);
        //fprintf(stdout, "num = %d, cur = %c, res = %s\n", num, cur, res);

        char *tmp = pre;
        pre = res;
        res = tmp;
        --n;
    }
    return strdup(pre);
}

int main()
{
    int i = 0;
    char *res = NULL;
    for (i = 1; i <= 30; ++ i) {
        res = countAndSay(i);
        fprintf(stdout, "%d: %s\n", i, res);
        free(res);
    }

    return 0;
}