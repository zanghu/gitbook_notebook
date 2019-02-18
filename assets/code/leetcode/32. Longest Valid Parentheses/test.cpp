#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * 分析  此题要求用O(n)的时间算法计算出答案
 *       那点在于从某一点开始计算最长合法串到结尾时, 还要考虑统计到其内部的各个合法子串
 */
class Solution {
public:

    int longestValidParentheses(string s)
    {
        // record[i]表示stack[i+1]位置元素紧邻的下面的"缝隙"位置已经被括号匹配消除的括号数, 如果stack[i+1]被消除，那么record[i]+=2(因为每次匹配消除两个括号);
        vector<int> stack;
        vector<int> record(1, 0);
        int res = 0;
        for (unsigned int i = 0; i < s.size(); ++i) {
            if (s[i] == '(') {
                stack.push_back('(');
                record.push_back(0);
            }
            else {
                if (!stack.empty() && stack.back() == '(') {
                    stack.pop_back();
                    int tmp = record.back();
                    record.pop_back();
                    record.back() = record.back() + (tmp + 2); // +2是因为每次匹配被消除的括号是两个
                }
                else { // 发生断链, 即后面的部分绝对不可能和前面的部分组成连续的合法括号串. 所以先统计当前结果, 之后清空stack和record
                    for (unsigned int j = 0; j < record.size(); ++j) {
                        res = ((record[j] > res)? record[j]: res); 
                    }
                    stack.clear();
                    record.clear();
                    record.push_back(0);
                }
            }
        }

        for (unsigned int j = 0; j < record.size(); ++j) {
            res = ((record[j] > res)? record[j]: res); 
        }

        return res;
    }
};

int main()
{
    Solution s;

    cout << s.longestValidParentheses("(()") << endl;
    cout << s.longestValidParentheses(")()())") << endl;
    cout << s.longestValidParentheses(")()(((())))(") << endl;
    cout << s.longestValidParentheses(")()())()()(") << endl;

    return 0;
}
