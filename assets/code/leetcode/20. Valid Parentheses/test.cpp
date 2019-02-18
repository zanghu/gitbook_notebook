#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
public:
    bool isValid(string s)
    {
        vector<char> stack;
        for (unsigned int i = 0; i < s.size(); ++i) {
            //cout << "s[" << i << "] = " << s[i] << endl;
            if (s[i] == '{' || s[i] == '[' || s[i] == '(') {
                stack.push_back(s[i]);
            }
            else {
                switch (s[i]) {

                    case '}':
                    if (stack.empty()) {
                        return false;
                    }
                    if (stack.back() != '{') {
                        return false;
                    }
                    stack.pop_back();
                    break;

                    case ']':
                    if (stack.empty()) {
                        return false;
                    }
                    if (stack.back() != '[') {
                        return false;
                    }
                    stack.pop_back();
                    break;

                    case ')':
                    if (stack.empty()) {
                        return false;
                    }
                    if (stack.back() != '(') {
                        return false;
                    }
                    stack.pop_back();
                    break;

                    default:
                    break;
                }
            }
        }

        if (!stack.empty()) {
            return false;
        }

        return true;
    }
};

int main()
{
    Solution s;

    cout << s.isValid("()") << endl;
    cout << s.isValid("()[]{}") << endl;
    cout << s.isValid("(]") << endl;
    cout << s.isValid("([)]") << endl;
    cout << s.isValid("{[]}") << endl;
    cout << s.isValid("){") << endl;

    return 0;
}