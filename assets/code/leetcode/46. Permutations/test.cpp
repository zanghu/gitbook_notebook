#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums)
    {
        vector<vector<int> > res;
        vector<int> cur;
        vector<bool> mark(nums.size(), false);
        permute_recur(nums, res, cur, mark);
        return res; 
    }

private:
    void permute_recur(const vector<int> &nums, vector<vector<int> > &res, vector<int> &cur, vector<bool> &mark)
    {
        for (unsigned int i =0 ; i < nums.size(); ++i) {
            if (mark[i]) {
                continue;
            }
            cur.push_back(nums[i]);
            mark[i] = true;
            if (cur.size() == nums.size()) {
                res.push_back(cur);
            }
            else {
                permute_recur(nums, res, cur, mark);
            }
            cur.pop_back();
            mark[i] = false;
        }
    }
};

void show(const vector<vector<int> > &res)
{
    for (unsigned int i = 0; i < res.size(); ++i) {
        for (unsigned int j = 0; j < res[i].size() - 1; ++j) {
            cout << res[i][j] << ",";
        }
        cout << res[i][res[i].size() - 1] << endl;
    }
}

int main()
{
    Solution s;

    vector<int> nums {1,2,3};
    show(s.permute(nums));

    return 0;
}
