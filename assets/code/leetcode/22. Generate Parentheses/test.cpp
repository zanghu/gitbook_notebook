#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    vector<string> generateParenthesis(int n)
    {
        vector<char> stack;
        string cur("");
        vector<string> res;
        generateParenthesis_recur(stack, res, cur, n, n);
        return res;
    }

private:
    void generateParenthesis_recur(vector<char> &stack, vector<string > &res, string &cur, int num_left, int num_right)
    {
        if (num_left == 0 && num_right == 0) {
            res.push_back(cur);
            return;
        }

        if (stack.empty()) {
            if (num_left <= 0) {
                return;
            }
            else {
                cur.push_back('(');
                stack.push_back('(');
                generateParenthesis_recur(stack, res, cur, num_left - 1, num_right);
                stack.pop_back();
                cur.pop_back();
            }
        }
        else {
            if (num_left > 0) {
                cur.push_back('(');
                bool is_push = false;
                if (stack.back() == '(') {
                    stack.push_back('(');
                    is_push = true;
                }
                else {
                    stack.pop_back();
                }
                generateParenthesis_recur(stack, res, cur, num_left - 1, num_right);
                if (is_push) {
                    stack.pop_back();
                }
                else {
                    stack.push_back(')');
                }
                cur.pop_back();
            }
            if (num_right > 0) {
                cur.push_back(')');
                bool is_push = false;
                if (stack.back() == ')') {
                    stack.push_back(')');
                    is_push = true;
                }
                else {
                    stack.pop_back();
                }
                generateParenthesis_recur(stack, res, cur, num_left, num_right - 1);
                if (is_push) {
                    stack.pop_back();
                }
                else {
                    stack.push_back('(');
                }
                cur.pop_back();
            }
        }
    }
};

void show(const vector<string> &res)
{
    for (unsigned int i = 0; i < res.size(); ++i) {
        cout << res[i] << endl;
    }
}

int main()
{
    Solution s;

    vector<string> res = s.generateParenthesis(3);
    show(res);

    return 0;
}