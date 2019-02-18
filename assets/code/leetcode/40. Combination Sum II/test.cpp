#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target)
    {
        vector<int> cur;
        vector<vector<int> > res;
        sort(candidates.begin(), candidates.end());
        vector<int> limit;
        vector<int> candidates_new;
        int idx = 0;
        int val = -1;
        for (unsigned int i = 0; i < candidates.size(); ++i) {
            if (val != candidates[i]) {
                val = candidates[i];
                candidates_new.push_back(val);
                limit.push_back(1);
                idx = limit.size() - 1;
            }
            else {
                ++limit[idx];
            }
        }
        combinationSum2_recur(candidates_new, target, res, cur, 0, limit);

        return res;
    }

    void combinationSum2_recur(vector<int> &candidates, int target, vector<vector<int> > &res, vector<int> &cur, int start, const vector<int> &limit) 
    {
        //fprintf(stdout, "start = %d+++++\n", start);
        for (unsigned i = start; i < candidates.size(); ++i) {
            int val = candidates[i];
            int left = target;
            int cnt = 0;
            while (cnt < limit[i] && val <= left) {
                cur.push_back(val);
                //fprintf(stdout, "push %d\n", val);
                ++cnt;
                left -= val;
                if (left == 0) {
                    res.push_back(cur);
                    //fprintf(stdout, "命中\n");
                    break;
                }
                combinationSum2_recur(candidates,  left, res, cur, i + 1, limit);
            }
            for (int j = 0; j < cnt; ++j) {
                cur.pop_back();
                //fprintf(stdout, "pop\n");
            }
        }
        //fprintf(stdout, "end----------\n");
        return;
    }
};

void show(const vector<vector<int> > &res)
{
    for (unsigned int i = 0; i < res.size(); ++i) {
        cout << "res[" << i << "]: ";
        for (unsigned int j = 0; j < res[i].size(); ++j) {
            cout << res[i][j] << " ";
        }
        cout << endl;
    }

    return;
}

int main()
{
    Solution s;

    vector<int> candidates_1 {2,3,6,7};
    int target_1 = 7;
    vector<vector<int> > res_1 = s.combinationSum2(candidates_1, target_1);
    show(res_1);

    vector<int> candidates_2 {2,3,5};
    int target_2 = 8;
    vector<vector<int> > res_2 = s.combinationSum2(candidates_2, target_2);
    show(res_2);

    vector<int> candidates_3 {10,1,2,7,6,1,5};
    int target_3 = 8;
    vector<vector<int> > res_3 = s.combinationSum2(candidates_3, target_3);
    show(res_3);

    return 0;
}