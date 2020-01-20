## 字符串前的f：f-string

### 简介

`f-string`，亦称为格式化字符串常量（formatted string literals），是`Python3.6`新引入的一种字符串格式化方法，该方法源于PEP 498 – Literal String Interpolation，主要目的是使格式化字符串的操作更加简便。

`f-string`在形式上是以 f 或 F 修饰符引领的字符串（f'xxx' 或 F'xxx'），以大括号 {} 标明被替换的字段；f-string在本质上并不是字符串常量，而是一个在运行时运算求值的表达式.

`f-string`在功能方面不逊于传统的%-formatting语句和str.format()函数，同时性能又优于二者，且使用起来也更加简洁明了，因此对于`Python3.6`及以后的版本，推荐使用`f-string`进行字符串格式化。