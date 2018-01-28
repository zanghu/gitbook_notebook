## pytorch中设定使用指定的GPU

参考资料：[http://www.cnblogs.com/darkknightzh/p/6836568.html](http://www.cnblogs.com/darkknightzh/p/6836568.html)

PyTorch默认使用从0开始的GPU，如果GPU0正在运行程序，需要指定其他GPU。

有如下两种方法来指定需要使用的GPU。

### 1.类似tensorflow指定GPU的方式，使用CUDA\_VISIBLE\_DEVICES。

直接终端中设定：

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

