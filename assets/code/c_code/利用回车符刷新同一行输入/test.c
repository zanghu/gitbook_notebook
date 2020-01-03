#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int i = 0;
    while (1) {
        printf("%ca=%8d", '\r', i); //  linux下使用回车符，将光标重置到行首，达到在同一行不断刷新输出内容的目的
        fflush(stdout);
        if (i > 10000000) {
            i = 0;
        }
        ++i;
    }
    return 0;
}