## 协方差矩阵的定义和计算

### 1.协方差矩阵的概率学定义

下图来自[维基百科](https://zh.wikipedia.org/wiki/%E5%8D%8F%E6%96%B9%E5%B7%AE%E7%9F%A9%E9%98%B5)

![](/assets/math002_01.PNG)

### 2.协方差矩阵的统计学定义

设X是一个有M个分量的随机向量，样本矩阵D是由X的N的样本组成的，那么X基于样本集D的统计协方差矩阵为

![](/assets/math002_02.PNG)

### 3.统计协方差计算方法

`numpy.cov(a)`: a是样本矩阵的转置，即a的每一列是一个样本向量

