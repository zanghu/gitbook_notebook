## Jekins哈希函数

**参考资料**

wiki：[Jenkins hash function](https://en.wikipedia.org/wiki/Jenkins_hash_function)

算法介绍：[Wang/Jenkins Hash算法](http://d0evi1.com/wang-jenkins-hash/)

新浪博客：[常用哈希函数笔记](http://blog.sina.com.cn/s/blog_70b2b6020100lexb.html)

搜狐：[推荐系统候选池的两种去重策略](https://www.sohu.com/a/231215520_499730)

Wang/Jenkins Hash算法在网上提到的也甚多，但是很少有人或有文章能系统地能将该算法的来龙去脉说明白。于是，我就充当了该苦工，幸好还是找到了一些东西，尝试着将该算法简单道来。

最早，Bob Jenkins提出了多个基于字符串通用Hash算法（搜Jenkins Hash就知道了），而Thomas Wang在Jenkins的基础上，针对固定整数输入做了相应的Hash算法。因而，其名字也就成了Wang/Jenkins Hash，其64位版本的 Hash算法如下：

```
uint64_t hash(uint64_t key) {
    key = (~key) + (key << 21); // key = (key << 21) - key - 1;
    key = key ^ (key >> 24);
    key = (key + (key << 3)) + (key << 8); // key * 265
    key = key ^ (key >> 14);
    key = (key + (key << 2)) + (key << 4); // key * 21
    key = key ^ (key >> 28);
    key = key + (key << 31);
    return key;
}
```

其关键特性是：

1.雪崩性（更改输入参数的任何一位，就将引起输出有一半以上的位发生变化）
2.可逆性（input ==> hash ==> inverse_hash ==> input）