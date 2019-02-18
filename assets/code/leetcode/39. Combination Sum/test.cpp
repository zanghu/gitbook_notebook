#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * @brief 递归枚举，这样做会获得重复的答案, 即232和223都出现在返回结果中
 */
class Solution1 {
public:
    vector<vector<int> > combinationSum(vector<int>& candidates, int target) {
        vector<int> cur;
        vector<vector<int> > res;
        sort(candidates.begin(), candidates.end());
        combinationSum_recur(candidates, target, res, cur);

        return res;
    }

    void combinationSum_recur(vector<int> &candidates, int target, vector<vector<int> > &res, vector<int> &cur) 
    {
        for (vector<int>::iterator it = candidates.begin(); it != candidates.end(); ++it) {
            int val = *it;
            cur.push_back(val);
            if (val == target) {
                res.push_back(cur);
            }
            else {
                if (val < target) {
                    combinationSum_recur(candidates, target - val, res, cur);
                }
            }
            cur.pop_back();
        }

        return;
    }
};

/**
 * @brief 修改了递归子问题. 子问题变成: 答案中有且仅有1个elem[0]时, 递归由elem[1]-elem[N]凑出target-elem[0]; 答案中有且仅有2个elem[0]时,递归由elem[1]-elem[N]凑出target-2*elem[0]; ...  
 */
class Solution {
public:
    vector<vector<int> > combinationSum(vector<int>& candidates, int target) {
        vector<int> cur;
        vector<vector<int> > res;
        sort(candidates.begin(), candidates.end());
        combinationSum_recur(candidates, target, res, cur, 0);

        return res;
    }

    void combinationSum_recur(vector<int> &candidates, int target, vector<vector<int> > &res, vector<int> &cur, int start) 
    {
        //fprintf(stdout, "start = %d+++++\n", start);
        for (unsigned i = start; i < candidates.size(); ++i) {
            int val = candidates[i];
            int left = target;
            int cnt = 0;
            while (val <= left) {
                cur.push_back(val);
                //fprintf(stdout, "push %d\n", val);
                ++cnt;
                left -= val;
                if (left == 0) {
                    res.push_back(cur);
                    //fprintf(stdout, "命中\n");
                    break;
                }
                combinationSum_recur(candidates,  left, res, cur, i + 1);
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
    Solution1 s;
/*
    vector<int> candidates_1 {2,3,6,7};
    int target_1 = 7;
    vector<vector<int> > res_1 = s.combinationSum(candidates_1, target_1);
    show(res_1);
*/
    vector<int> candidates_2 {2,3,5};
    int target_2 = 8;
    vector<vector<int> > res_2 = s.combinationSum(candidates_2, target_2);
    show(res_2);

    return 0;
}