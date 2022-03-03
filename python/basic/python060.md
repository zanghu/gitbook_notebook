## str与bytes相互转换

**参考资料**

cnblogs：[python str与bytes之间的转换](https://www.cnblogs.com/zqifa/p/python-7.html)

### 说明

Python2时代打开文件习惯一律用`'rb'`或者`'wb'`，但是Python3中如果还这样做，后续操作都需要围绕bytes展开，因此就需要经常用到字符串`str`和二进制字节`bytes`。

### 示例

```python
# bytes object  
b = b"example"

# str object  
s = "example"  

# str to bytes  
sb = bytes(s, encoding = "utf8")  

# bytes to str  
bs = str(b, encoding = "utf8")  

# an alternative method  
# str to bytes  
sb2 = str.encode(s)  

# bytes to str  
bs2 = bytes.decode(b)
```