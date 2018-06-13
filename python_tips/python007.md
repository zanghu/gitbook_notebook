## Python: 使用conda安装opencv

参考资料: [https://stackoverflow.com/questions/23119413/how-do-i-install-python-opencv-through-conda](https://stackoverflow.com/questions/23119413/how-do-i-install-python-opencv-through-conda)

#### 方法

目前Anaconda默认安装后不包含opencv, 即`import cv2`会失败。

经过测试，如下的安装方法均不可行：

```shell
conda install opencv
conda install cv2
```

推荐的使用conda安装opencv的方法：

```shell
conda install -c menpo opencv
```

