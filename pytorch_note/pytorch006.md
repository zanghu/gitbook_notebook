## Pytorch: 难懂函数——torch.gather

参考资料：[http://www.cnblogs.com/darkknightzh/p/6836568.html](http://www.cnblogs.com/darkknightzh/p/6836568.html)

torch.gather函数的官方文档中解释比较复杂，而例子有关于简单

### 1.torch.gather的作用

其实从torch.gather子啊官方文档中的例子可以看出，起本质上是用于对torch.Tensor类对象独享进行元素置换的函数。

### 2.torch.gather参数说明

* 函数原型

`torch.gather(input, dim, index, out=None) → Tenso`

```shell
CUDA_VISIBLE_DEVICES=1 python main.py
```

python代码中设定：

```python
import os
os.environ["CUDA_VISIBLE_DEVICES"] = "2"
```

参考网址：[http://www.cnblogs.com/darkknightzh/p/6591923.html](http://www.cnblogs.com/darkknightzh/p/6591923.html)

### 2.使用函数 set\_device

```python
import torch
torch.cuda.set_device(id)
```

该函数见 pytorch-master\torch\cuda\__init\__.py。

不过官方建议使用CUDA\_VISIBLE\_DEVICES，不建议使用 set\_device 函数。

