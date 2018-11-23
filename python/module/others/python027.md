## Python: 使用operator模块提供的函数替代自定义lambda函数

`operator`模块为 Python 提供了一个 "功能性" 的标准操作符接口. 当使用`map`以及`filter`一类的函数的时候, `operator`模块中的函数可以替换一些自定义`lambda`函式.

这些函数在一些喜欢写晦涩代码的程序员中很流行.

### 1.问题引出

阅读 ctw_baseline/detection 代码是遇到如下函数：

```python
# 函数来自ctw_baseline/detection/darknet/darknet_tools.py
def last_backup(backup_root):

    ... # 其他代码
    
    # 找不到首选权重文件时，用名为yolo-chinese_数字.weights的权重文件代替
    # re.escape()的作用是将输入字符串内所有属于正则表达式特殊字符的字符作普通字符处理
    r = re.compile(r'^{}_(\d+)\.weights$'.format(re.escape(basename)))
    all = [(None, -1)]
    for filename in os.listdir(backup_root):
        filepath = os.path.join(backup_root, filename)
        if os.path.isfile(filepath):
            m = r.match(filename)
            if m:
                # 注意: 按照规定group(0)返回的是正则表达式命中的字符串整体, group(1)返回其中第一个括号组
                i = int(m.group(1)) 
                all.append((filepath, i))
    # 这里用到了operator.itemgetter()
    # max函数原型: max(iterable, *[, default=obj, key=func]) -> value
    return max(all, key=operator.itemgetter(1))[0] # 应该是先用operator.itemgetter定义的函数key对列表all中
```

### 2.operator模块

自从引入labmda函数之后，Python中很多容器操作函数都支持类似上面的`max`函数的功能，即支持输入一个对容器元素进行操作的函数参数。 

`operator`模块提供了很多常用的简单函数，灵活使用能够有效提高代码的可读性（摆脱繁琐的基本操作，突出代码的逻辑目的）。


