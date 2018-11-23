## Python: 使用operator模块提供的函数替代自定义lambda函数

`operator`模块为 Python 提供了一个 "功能性" 的标准操作符接口. 当使用`map`以及`filter`一类的函数的时候, `operator`模块中的函数可以替换一些自定义`lambda`函式.

这些函数在一些喜欢写晦涩代码的程序员中很流行.

### 1.问题引出

```python
def last_backup(backup_root):
    ...
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
    return max(all, key=operator.itemgetter(1))[0] # 应该是先用operator.itemgetter定义的函数key对列表all中
    ...
```