## 术语：CDF、KDE、ECDF

参考资料：[知乎：[数值计算] 条件数](https://zhuanlan.zhihu.com/p/82057409)

参考资料：[维基百科：条件数](https://zh.wikipedia.org/wiki/%E6%9D%A1%E4%BB%B6%E6%95%B0)

### 0.条件数的意义

> In the field of numerical analysis, the condition number of a function with respect to an argument measures how much the output value of the function can change for a small change in the input argument

也就是说条件数是衡量输入參数的微小变化对输出值的影响。

对于一个线性方程Ax=b，当系数矩阵A 一个很小的扰动只引起解向量x很小的扰动时，就称矩阵A 是“良态”矩阵（well-conditioned matrix）。若系数矩阵A 一个很小的扰动会引起解向量x很大的扰动，则称矩阵A 是“病态”矩阵（ill-conditioned matrix）。条件数刻画了求解线性方程时，误差经过矩阵A 的传播扩大为解向量的误差的程度，因此是衡量线性方程数值稳定性的一个重要指标。

### 1.函数的条件数

![](/assets/math005_01.PNG)

### 2.矩阵的条件数

![](/assets/math005_02.PNG)


