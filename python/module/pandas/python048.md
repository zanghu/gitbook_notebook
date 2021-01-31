## Python: 使用operator模块提供的函数替代自定义lambda函数

使用正则表达式时，希望匹配包含某种特殊模式子串的字符串，并得到其中匹配的子串。

group（）就能满足此种需求，用来提出分组截获的字符串，（）用来分组。

```python
import re

a = "123abc456"
print re.search("([0-9]*)([a-z]*)([0-9]*)",a).group(0)   #123abc456,返回整体
print re.search("([0-9]*)([a-z]*)([0-9]*)",a).group(1)   #123
print re.search("([0-9]*)([a-z]*)([0-9]*)",a).group(2)   #abc
print re.search("([0-9]*)([a-z]*)([0-9]*)",a).group(3)   #456
```