# \[Python\]numpy的array方法和asarray方法的区别

下面演示对使用函数装饰器的函数调用自省/反射方法**name**和**doc**的效果

```python
def wrap(func):  
    def call_it(*args, **kwargs):  
        """wrap func: call_it"""  
        print 'before call'  
        return func(*args, **kwargs)  
    return call_it

@wrap  
def hello():  
    """say hello"""  
    print 'hello world'

from functools import update_wrapper  
def wrap2(func):  
    def call_it(*args, **kwargs):  
        """wrap func: call_it2"""  
        print 'before call'  
        return func(*args, **kwargs)  
    return update_wrapper(call_it, func)

@wrap2  
def hello2():  
    """test hello"""  
    print 'hello world2'

if __name__ == '__main__':  
    hello()  
    print hello.__name__  
    print hello.__doc__

    print  
    hello2()  
    print hello2.__name__  
    print hello2.__doc__
```

返回结果如下, 使用functools.update\_wrapper\(\)前被修饰函数的函数签名是call\_it, 使用后则变回hello2:

```python
>>before call
hello world
call_it
wrap func: call_it

before call
hello world2
hello2
test hello
```

参考链接: [http://www.jb51.net/article/65778.htm](http://www.jb51.net/article/65778.htm)

