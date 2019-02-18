/**
 * 1.计算速度
 * 2.上溢
 * 3.下溢
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

static double m_cache[64];
static int m_top = 0;


double myPow_recur(double x, long n)
{
    if (n == 0) {
        return 1.0;
    }

    double res = x;
    int m = 1;
    int idx = 0;
    //while (m < 1073741824 && m * 2 <= n) { // 由于INT_MAX=2147483647, 所以当m = 1073741824时, m*2会发生上溢导致m*2<INT_MAX成立
    while (m <= INT_MAX / 2 && m * 2 <= n) {
        m = m * 2;
        ++idx;
    }

    while (m_top <= idx) {
        m_cache[m_top] = m_cache[m_top - 1] * m_cache[m_top - 1];
        ++m_top;
    }
    res = m_cache[idx];

    if (m < n) {
        res = res * myPow_recur(x, n - m);
    }
 
    return res;
}

double myPow(double x, int n)
{
    m_cache[0] = x;
    m_top = 1;

    int neg = 0;
    int delta = 0;
    if (n < 0) {
        neg = 1;
        if (n == INT_MIN) { // int最小值是-2147483648, 最大值是2147483647, 因此最小值*-1会导致正向溢出
            delta = 1;
            ++n;
        }
        n *= -1;
    }

    double res = myPow_recur(x, n);
    if (delta) {
        res *= x;
    }

    if (neg) {
        res = 1.0 / res;
    }
    return res;
}

int main()
{
    double x;
    int n;
#ifdef _TEST
    FILE *fp = fopen("1.txt", "rb");
    if (fp == NULL) {
        fprintf(stderr, "fopen failed\n");
        return 1;
    }
    while (fscanf(fp, "%lf, %d", &x, &n) != EOF) {
#else
    while (fscanf(stdin, "%lf, %d", &x, &n) != EOF) {
#endif
        //gettimeofday(&t0, NULL);
        fprintf(stdout, "%lf\n", myPow(x, n));
        //gettimeofday(&t1, NULL);
        //timersub(&t1, &t0, &t2);
    }

#ifdef _TEST
    fclose(fp);
#endif

    return 0;
}