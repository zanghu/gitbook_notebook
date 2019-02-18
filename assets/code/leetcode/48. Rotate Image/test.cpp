#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    void rotate(vector<vector<int>>& matrix)
    {
        // 旋转后矩阵
        int n = matrix.size();
        vector<vector<int> > matrix_new(matrix);

        // 求中心点坐标
        int mid = n / 2;

        int x = 0;
        int y = 0;
        int dx = 0;
        int dy = 0;
        if (n % 2 == 1) { // 边长为奇数
            // 旋转
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {

                    if (i <= mid && j <= mid) {
                        dx = mid - i;
                        dy = mid - j;
                        x = mid - dy;
                        y = mid + dx;  
                    }
                    else {
                        if (i >= mid && j <= mid) {
                            dx = i - mid;
                            dy = mid - j;
                            x = mid - dy;
                            y = mid - dx;  
                        }
                        else {
                            if (i >= mid && j >= mid) {
                                dx = i - mid;
                                dy = j - mid;
                                x = mid + dy;
                                y = mid - dx;  
                            }
                            else {
                                dx = mid - i;
                                dy = j - mid;
                                x = mid + dy;
                                y = mid + dx;  
                            }
                        }
                    }

                    matrix[x][y] = matrix_new[i][j];
                }
            }
        }
        else { // 边长为偶数;
            mid -= 1;
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (i <= mid && j <= mid) {
                        dx = mid - i;
                        dy = mid - j;
                        x = mid - dy;
                        y = mid  + 1 + dx;
                    }
                    else {
                        if (i > mid && j <= mid) {
                            dx = i - (mid + 1);
                            dy = mid - j;
                            x = mid - dy;
                            y = mid - dx;
                        }
                        else {
                            if (i > mid && j > mid) {
                                dx = i - (mid + 1);
                                dy = j - (mid + 1);
                                x = mid + 1 + dy;
                                y = mid - dx;
                            }
                            else {
                                dx = mid - i;
                                dy = j - (mid + 1);
                                x = mid + 1 + dy;
                                y = mid + 1 + dx;
                            }
                        }
                    }
                    //fprintf(stdout, "x = %d, y = %d, i = %d, j = %d\n", x, y, i, j);
                    matrix[x][y] = matrix_new[i][j];
                }
            }
        }
    }
};

void show(vector<vector<int> > matrix) {
    int n = matrix.size();
    for (int i = 0; i < n; ++i) {
        for(int j = 0; j < n - 1; ++j) {
            cout << matrix[i][j] << ",";
        }
        cout << matrix[i][n - 1] << endl;
    }
}

int main()
{
    Solution s;

    vector<vector<int> > img_0 {{1,2,3}, {4,5,6}, {7,8,9}};
    s.rotate(img_0);
    show(img_0);

    vector<vector<int> > img_1 {{5, 1, 9,11}, {2, 4, 8,10}, {13, 3, 6, 7}, {15,14,12,16}};
    s.rotate(img_1);
    show(img_1);

    return 0;
}