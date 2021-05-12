## 获得模块的文件路径和函数所属文件（模块）的文件路径简单方法

参考资料: [Python 模块的用法说明及模块文件位置/函数所属模块](https://blog.csdn.net/weixin_38284096/article/details/80434362)

* **1.获取模块的文件路径**

以`os`模块为例，获得该模块的文件路径。

```python
>>> import os
>>> os.__file__
'/home/zanghu/ProgramFiles/software/anaconda3/anaconda3-5.1.0/lib/python3.6/os.py'
>>>
```

* **3.利用help功能获得函数的所属文件（模块）的文件路径**

由于`python`中如果A模块中import了B模块的函数`func`，比如：

```python
"""A.py"""
import B

func = B.func
...
```

那么用户可以通过`import A; A.func()`的方式调用函数`func`，所以这种情况下在模块A的代码中找不到`func`的定义，如果需要阅读函数`func`源码的情况就很希望能查到包含该函数定义的`.py`文件的路径。

以`tensorflow.contrib.slim`模块为例，其中的`slim.fully_connected`就是一个这样的函数，如果使用find + grep的方式往往可能由于源码含有多个同名函数而不好用：

```shell
@ubuntu:~/ProgramFiles/anaconda3-5.1.0/lib/python3.6/site-packages$ find . -name "*.py" | xargs grep -n "def fully_connected"
./tensorflow/contrib/layers/python/layers/layers.py:1768:def fully_connected(inputs,
./tensorflow/contrib/timeseries/python/timeseries/model_utils.py:33:def fully_connected(inp,
```

正确的做法：

```python
>>>import tensorflow as tf
>>>import tensorflow.contrib.slim
>>>help(slim.fully_connected)
```

显示如下：

![](/assets/python021_002.png)

打开上图绿框中指定位置的文件，果然查找到函数定义：

![](/assets/python021_001.PNG)

