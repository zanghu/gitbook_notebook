#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ALPHABET_SIZE (256)

/**
 * 计算以s[idx]为起点的最长非重复子串
 */
void calLongest(const char *s, int len, int *mark, int *record)
{
    //fprintf(stdout, "s = %s, len = %d\n", s, len);
    int i = 0;
    int start = 0;
    int delta = 1;
    while (start < len) {
        //fprintf(stdout, "start = %d, delta = %d, i = %d\n", start, delta, i);
        //memset(mark, 0, ALPHABET_SIZE);
        ++mark[(unsigned char)(s[start])];
        for (i = start + delta; i < len; ++i) {
            //mark[(unsigned char)(s[i])] = 1;
            if (mark[(unsigned char)(s[i])] == 0) {
                //fprintf(stdout, "not dup, s[%d] = %c\n", i, s[i]);
                ++mark[(unsigned char)(s[i])];
            }
            else {
                //fprintf(stdout, "dup, break\n");
                break;
            }
        }
        if (i >= len) {
            break;
        }
        --mark[(unsigned char)(s[start])];
        record[start] = i - start;
        delta = i - start - 1;
        ++start;
    }
}

int lengthOfLongestSubstring_recur(const char *s, int len, int start, const int *record)
{
    if (len - start == 1) {
        return 1;
    }

    int cur = lengthOfLongestSubstring_recur(s, len, start + 1, record);
    return ((record[start] > cur)? record[start]: cur);
}

int lengthOfLongestSubstring(char* s)
{
    int len = strlen(s);
    if (len == 0) {
        return 0;
    }
    int mark[ALPHABET_SIZE];

    memset(mark, 0, ALPHABET_SIZE);
    int *record = calloc(len, sizeof(int));
    calLongest(s, len, mark, record);
    int i = 0;
    for (i = 0; i < len; ++i) {
        fprintf(stdout, "%d ", record[i]);
    }
    fprintf(stdout, "\n");

    int res = lengthOfLongestSubstring_recur(s, len, 0, record);
    free(record);
    return res;
}

int main()
{
    fprintf(stdout, "%d\n", lengthOfLongestSubstring("abcabcbb"));
    fprintf(stdout, "%d\n", lengthOfLongestSubstring("bbbbb"));
    fprintf(stdout, "%d\n", lengthOfLongestSubstring("pwwkew"));
    fprintf(stdout, "%d\n", lengthOfLongestSubstring("   "));
    fprintf(stdout, "%d\n", lengthOfLongestSubstring("aab"));
    fprintf(stdout, "%d\n", lengthOfLongestSubstring("abcb"));
    fprintf(stdout, "%d\n", lengthOfLongestSubstring("abcabcbb"));


    return 0;
}