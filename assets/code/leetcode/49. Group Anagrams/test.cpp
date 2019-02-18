#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unistd.h>

using namespace std;

#define NUM_ALPHABET (26)

class Solution {
public:
    vector<vector<string> > groupAnagrams(vector<string>& strs)
    {
        vector<vector<string> > res;
        vector<int *> bit_set;
        vector<int *> bit_all; // 记录每个字符串的bit map

        int *mem_all = (int *)calloc(strs.size() * NUM_ALPHABET, sizeof(int));
        if (mem_all == NULL) {
            exit(EXIT_FAILURE);
        }

        int *mem_set = (int *)calloc(strs.size() * NUM_ALPHABET, sizeof(int));
        if (mem_set == NULL) {
            exit(EXIT_FAILURE);
        }

        for (unsigned int i = 0; i < strs.size(); ++i) {
            hash(mem_all + NUM_ALPHABET * i, strs[i].c_str());
            bit_all.push_back(mem_all + NUM_ALPHABET * i);
        }

        for (unsigned int i = 0; i < strs.size(); ++i) {
            bool match = false;
            for (unsigned int j = 0; j < res.size(); ++j) {
                if (strs[i].length() != res[j][0].length()) {
                    match = false;
                    continue;
                }
                if (is_eq(bit_all[i], bit_set[j])) {
                    res[j].push_back(strs[i]);
                    match = true;
                    break;
                }
            }
            if (!match) {
                vector<string> tmp(1, strs[i]);
                res.push_back(tmp);
                bit_set.push_back(bit_all[i]);
            }
        }

        free(mem_all);
        free(mem_set);

        return res;
    }

private:
    // 逐个比较数组元素是比较慢的方法, 如果是两个C数组，就可以直接memcmp
    inline bool is_eq(const int *bit1, const int *bit2)
    {
        if (memcmp(bit1, bit2, NUM_ALPHABET * sizeof(int)) == 0) {
            return true;
        }
        return false;
    }

    void hash(int *bit, const char *s)
    {
        int idx = 0;
        for (unsigned int i = 0; i < strlen(s); ++i) {
            idx = s[i] - 'a';
            ++(bit[idx]);
        }
        return;
    }
};

class Solution1 {
public:
    vector<vector<string> > groupAnagrams(vector<string>& strs)
    {
        vector<vector<string> > res;
        vector<vector<int> > bit_set;
        vector<vector<int> > bit_all; // 记录每个字符串的bit map

        for (unsigned int i = 0; i < strs.size(); ++i) {
            bit_all.push_back(hash(strs[i].c_str()));
        }

        for (unsigned int i = 0; i < strs.size(); ++i) {
            bool match = false;
            for (unsigned int j = 0; j < res.size(); ++j) {
                if (strs[i].length() != res[j][0].length()) {
                    match = false;
                    continue;
                }
                if (is_eq(bit_all[i], bit_set[j])) {
                    res[j].push_back(strs[i]);
                    match = true;
                    break;
                }
            }
            if (!match) {
                vector<string> tmp(1, strs[i]);
                res.push_back(tmp);
                bit_set.push_back(bit_all[i]);
            }
        }

        return res;
    }

private:
    // 逐个比较数组元素是比较慢的方法, 如果是两个C数组，就可以直接memcmp
    bool is_eq(const vector<int> &bit1, const vector<int> &bit2)
    {
        for (int i = 0; i < NUM_ALPHABET; ++i) {
            if (bit1[i] != bit2[i]) {
                return false;
            }
        }
        return true;
    }

    vector<int> hash(const char *s)
    {
        vector<int> bit(NUM_ALPHABET, 0);
        int idx = 0;
        for (unsigned int i = 0; i < strlen(s); ++i) {
            idx = s[i] - 'a';
            ++bit[idx];
        }

        return bit;
    }
};

void show(const vector<vector<string> > &res)
{
    for (unsigned int i = 0; i < res.size(); ++i) {
        cout << "res[" << i << "]: ";
        for (unsigned int j = 0; j < res[i].size(); ++j) {
            cout << "\"" << res[i][j] << "\" ";
        }
        cout << endl;
    }

    return;
}

int main()
{
    Solution s;

    vector<string> strs_1 {"eat", "tea", "tan", "ate", "nat", "bat"};
    vector<vector<string> > res_1 = s.groupAnagrams(strs_1);
    show(res_1);

    return 0;
}