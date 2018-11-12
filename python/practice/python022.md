## 远程命令行执行程序时matplotlib绘图命令报错的问题之解决方法

参考资料: [https://github.com/matplotlib/matplotlib/issues/3466](https://github.com/matplotlib/matplotlib/issues/3466)

* **方法一**

下面的方法据称在Amazon的AWS云计算服务器上有效，但本人在ubuntu server 16.04LTS下测试无效。

```python
import matplotlib.pyplot as plt
plt.switch_backend('agg')
```

* **方法二**

下面的方法本人在ubuntu server 16.04LTS下测试有效，但据称在Amazon的AWS云计算服务器上无效。

```python
import matplotlib
matplotlib.use('agg')
import matplotlib.pyplot as plt
```
