## Python字符串开头字母的含义：ubrf

**参考资料**

CSDN：[Python格式化字符串f-string概览](https://blog.csdn.net/sunxb10/article/details/81036693)

### 解释

`b`开头的字符串：bytes字节符

```python
>>> b= b'have'
>>> print(b)
b'have'
```

`r`开头的字符串：非转义原生（raw）字符串，原生字符串中的`\n`、`\r`等特殊字符将被作为字面值而不是换行符、回车符处理。

```python
>>> print(r'have\n')
have\n
```

`u`开头的字符串，unicode编码字符，python3默认字符串编码方式

```
>>> print(u'have\n')
have
 
>>>
```

`f`开头的字符串，`f-string`，Python36新增的格式化输出方法