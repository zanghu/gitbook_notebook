## Python: 使用pip安装模块

参考资料: [https://stackoverflow.com/questions/23119413/how-do-i-install-python-opencv-through-conda](https://stackoverflow.com/questions/23119413/how-do-i-install-python-opencv-through-conda)

### 基本命令

更新本地`pip`版本

```python
# 从python2.7.9开始自带pip
$ pip install --upgrade pip
```

安装软件包(如果本地已有吗，不会尝试更新版版本)

```python
$ pip3 install numpy
```

会自动更新本地版本的安装

```python
$ pip3 install -U numpy
```

从指定源安装

```python
# 以下命令指定使用清华提供的ipv6镜像源（临时使用指定源）
$ pip install -i https://pypi.tuna.tsinghua.edu.cn/simple some-package
```

将指定源设为pip的默认源

```python
$ pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple
```