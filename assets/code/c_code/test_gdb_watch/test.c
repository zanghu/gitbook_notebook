#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct A
{
    char *a;
    char b[2];
    int c[3];
};

int main()
{
    char a[100];
    memset(a, 'x', 100);
    printf("addr = %p\n", a);
    strncpy(a, "abc", 3);
    strncpy(a + 3, "def", 3);
    a[6] = '\0';
    struct A x;
    printf("size = %d\n", (int)sizeof(struct A));
    memcpy(a, &x, sizeof(struct A));

    return 0;
}