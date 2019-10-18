### Python: 安装opencv

参考资料: [https://stackoverflow.com/questions/23119413/how-do-i-install-python-opencv-through-conda](https://stackoverflow.com/questions/23119413/how-do-i-install-python-opencv-through-conda)

#### 1.使用conda安装opencv（不再推荐，2019.10.18）

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

#### 2.使用pip安装opencv

* 最简单的方法：

```shell
$ pip install opencv-python
```

* 详细说明

`pypi`网站中对`opencv`的情况进行了描述，简单地说`opencv`（也就是`import cv2`）在`pypi`库中共有四个互相独立的安装包，用户应该只选择其中之一进行安装，原因是这些安装包共用了`cv2`名空间。如果安装了上述四个安装包中的多于一个，那么应该用`pip uninstall`全部删除之后再重新安装唯一的一个需要的安装包。

