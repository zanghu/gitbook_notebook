## Python: 展示对象所有属性的类型的方法

参考资料:

#### 1.实现思路

* 首先，利用内建函数dir()列出对象的所有成员名称

* 之后，利用内建函数eval()重新获得成员句柄

* 最后，利用types模块提供的类型，配合内建函数isinstance()，判断每个成员句柄的类型

此外，代码中给出了一段使用使用locals()和id()两个内建函数相互配合获取变量名称的字符串的方法（虽然其实在这里也可以固定使用形参的名字的字符串）。

#### 2.实现代码

```python
#coding=utf-8
#!/bin/env python

"""
docstring
"""

import types
import numpy


def get_type_handlers():
    """返回从types模块中获取的全部类型句柄"""
    tool_names = [tool for tool in dir(types) if tool.endswith(r'Type')]
    assert len(tool_names) == 36

    #tools = [eval('.'.join(['types', name])) for name in tool_names]
    tools = [getattr(types, name) for name in tool_names]

    return tools


def filter_attrs(attr_names, filter_prefix, filter_suffix, \
        filter_surrounding):
    """返回按照过滤规则过滤后的属性名称列表"""
    # 参数检查
    assert isinstance(attr_names, list)
    assert (filter_prefix is None) or (isinstance(filter_prefix, (list, tuple)))
    assert (filter_suffix is None) or (isinstance(filter_suffix, (list, tuple)))
    assert (filter_surrounding is None) or \
           (isinstance(filter_surrounding, (list, tuple)))

    # 注意下面再判断列表为空时没有使用len()，而是利用了python将[], (), {}视为False\
    # 的特性. 如果使用len(a)>0判断那么pylint会报错

    if (filter_prefix is not None) and filter_prefix: # 按照指定前缀进行
        for prefix in filter_prefix:
            attr_names = [name for name in attr_names if not \
                          name.startswith(prefix)]

    if (filter_suffix is not None) and filter_suffix: # 按照指定后缀进行过滤
        for suffix in filter_suffix:
            attr_names = [name for name in attr_names \
                          if not name.endswith(suffix)]

    if (filter_surrounding is not None) and filter_surrounding: # 按照指定包围项进行过滤
        #print '过滤包围项'
        for pre, suf in filter_surrounding:
            attr_names = [name for name in attr_names \
            if ((not name.endswith(suf)) and (not name.startswith(pre)))]

    return attr_names


def show_attr_detail(obj, fmt=2, filter_prefix=None, filter_suffix=None, \
        filter_surrounding=(('__', '__'), )):
    """展示输入对象的所有成员的类型, 可以帮助用户判断哪些成员是方法，那些成员是属性。

    Args:
        obj: 需要获取属性类型的目标
        fmt: 结果显示格式
        filter_prefix: 指出具有哪些特定前缀的成员无需展示
        filter_suffix: 指出具有哪些特定后缀的成员无需展示
        filter_surrounding: 指出具有哪些特定"前缀+后缀"的成员无需展示,
                默认会过滤__xx__形式的成员

    """
    # 参数检查
    assert (fmt is None or fmt == 1 or fmt == 2)

    # 类型检查工具
    tools = get_type_handlers()

    # 待检查内容
    attr_name_list = dir(obj)

    # 过滤
    attr_name_list = filter_attrs(attr_name_list, filter_prefix, filter_suffix,
                 filter_surrounding)

    if fmt == 1:
        for attr_name in attr_name_list:
            #full_name = '.'.join([obj_name, attr_name])
            full_name = '.'.join(['obj', attr_name]) # 此处列表第一个参数必须保持和函数第一个形参一致
            res_list = []
            for tool in tools:
                #if isinstance(eval(full_name), tool):
                if isinstance(getattr(obj, attr_name), tool):
                    res_list.append(1)
                else:
                    res_list.append(0)
            print '{0}: {1}'.format(full_name, res_list)
    else:
        tool_names = [tool for tool in dir(types) if tool.endswith(r'Type')]
        for attr_name in attr_name_list:
            #full_name = '.'.join([obj_name, attr_name])
            full_name = '.'.join(['obj', attr_name]) # 此处列表第一个参数必须保持和函数第一个形参一致
            res_list = []
            for cnt, tool in enumerate(tools):
                #if isinstance(eval(full_name), tool):
                if isinstance(getattr(obj, attr_name), tool):
                    res_list.append(tool_names[cnt])
            print '{0}: {1}'.format(full_name, res_list)

class A(object):
    """class Aa"""

    def __init__(self):
        """init"""
        self.a_0 = 100
        self.b_0 = 20

    def say(self, content):
        """say"""
        print '{0}, {1}, {2}'.format(self.a_0, self.b_0, content)

if __name__ == '__main__':
    a = A()
    dir(a)
    show_attr_detail(a)
    print "=========="
    b = numpy.array([[1, 2]])
    show_attr_detail(b)
```

### 4.其他一些小技术

* 如何遍历一个类或模块的全部成员的**句柄**

将内建函数`dir()`和`getattr()`搭配使用
