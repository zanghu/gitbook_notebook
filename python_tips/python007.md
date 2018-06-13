# \[python\]展示对象所有属性的类型的方法

参考资料:

#### 1.实现思路

* 首先，利用内建函数dir()列出对象的所有成员名称

* 之后，利用内建函数eval()重新获得成员句柄

* 最后，利用types模块提供的类型，配合内建函数isinstance()，判断每个成员句柄的类型

#### 2.实现代码

```python
#coding=utf-8
#!/bin/env python

import types
import numpy
import copy

def show_attr_detail(obj, fmt=2, filter_prefix=[], filter_suffix=[], 
            filter_surrounding=[('__', '__')]):
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
    assert (filter_prefix is None) or (isinstance(filter_prefix, list))
    assert (filter_suffix is None) or (isinstance(filter_suffix, list))
    assert (filter_surrounding is None) or (isinstance(filter_surrounding, list))
        
    # 检查工具
    tool_names = [tool for tool in dir(types) if tool.endswith(r'Type')]
    assert len(tool_names) == 36
    
    tools = [eval('.'.join(['types', name])) for name in tool_names]
    
    # 待检查内容
    attr_name_list = dir(obj)
    
    # 过滤
    
    if (filter_prefix is not None) and (len(filter_prefix) > 0): # 过滤前缀
        for prefix in filter_prefix:
            attr_name_list = [name for name in attr_name_list if not name.startswith(prefix)]
            
    if (filter_suffix is not None) and (len(filter_suffix) > 0): # 过滤后缀
        for suffix in filter_suffix:
            attr_name_list = [name for name in attr_name_list if not name.endswith(suffix)]
            
    if (filter_surrounding is not None) and (len(filter_surrounding) > 0): # 过滤包围项
        #print '过滤包围项'
        for pre, suf in filter_surrounding:
            attr_name_list = [name for name in attr_name_list if ((not name.endswith(suf)) and \
             (not name.startswith(pre)))]
             
     # 获取变量的名称字符串, 其实就是'obj'
    obj_name = None
    var_names = copy.copy(locals())
    for k, v in var_names.iteritems():
        if id(v) == id(obj):
            obj_name = k
            break
    assert obj_name is not None
    
    if fmt == 1:
        for attr_name in attr_name_list:
            full_name = '.'.join([obj_name, attr_name])
            res_list = []
            for tool in tools:
                if isinstance(eval(full_name), tool):
                    res_list.append(1)
                else:
                    res_list.append(0)
            print '{0}: {1}'.format(full_name, res_list)
    else:
        for attr_name in attr_name_list:
            full_name = '.'.join([obj_name, attr_name])
            res_list = []
            for n, tool in enumerate(tools):
                if isinstance(eval(full_name), tool):
                    res_list.append(tool_names[n])
            print '{0}: {1}'.format(full_name, res_list)
    
class A(object):
    
    def __init__(self):
        self.a = 100
        self.b = 20
        
    def say(x):
        print x
        
if __name__ == '__main__':
    #test_type(numpy)
    a = A()
    dir(a)
    show_attr_detail(a)
    b = numpy.array([[1,2]])
    show_attr_detail(b)
```
