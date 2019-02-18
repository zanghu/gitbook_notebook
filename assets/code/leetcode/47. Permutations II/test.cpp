#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

class Solution {
public:
    vector<vector<int>> permuteUnique(vector<int>& nums)
    {
        // 统计元素个数
        map<int, int> dict;
        for (unsigned int i = 0; i < nums.size(); ++i) {
            if (dict.find(nums[i]) == dict.end()) {
                dict[nums[i]] = 1;
            }
            else {
                ++dict[nums[i]];
            }
        }
        vector<vector<int> > res;
        vector<int> cur;
        vector<int> left_unique;
        vector<int> nums_unique;
        for (map<int, int>::iterator it = dict.begin(); it != dict.end(); ++it) {
            nums_unique.push_back(it->first);
            left_unique.push_back(it->second);
        }

        permute_recur(nums_unique, left_unique, res, cur, nums.size());
        return res;
    }

private:
    /**
     * @brief 递归的子问题: 当前k个位置元素确定且唯一时, 后面n-k元素的所有可能组合与前k个元素拼在一起一定也是一个唯一的组合
     */
    void permute_recur(const vector<int> &nums, vector<int> &left, vector<vector<int> > &res, vector<int> &cur, unsigned int num_elem)
    {
        if (cur.size() == num_elem) {
            res.push_back(cur);
            return;
        }

        // 如果当前是第k轮递归, 那么for循环就是依次尝试选不同值元素作为cur[k]
        for (unsigned int i = 0; i < nums.size(); ++i) {
            if (left[i]) {
                cur.push_back(nums[i]);
                --left[i];
                permute_recur(nums, left, res, cur, num_elem);
                ++left[i];
                cur.pop_back();
            }
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

    vector<int> nums {1,1,2};
    show(s.permuteUnique(nums));

    return 0;
}
