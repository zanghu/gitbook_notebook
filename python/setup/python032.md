## Python: 使用pip安装模块

参考资料: [清华大学Pypi镜像](https://mirror.tuna.tsinghua.edu.cn/help/pypi/)

参考资料：[Pypi官方网站](https://pypi.org/)

### 1.什么是pip和pypi

`pip`是一个现代的，通用的 Python 包管理工具，是easy_install的替代品，英文`python install packages`

Python包索引（`PyPI`）是Python编程语言的软件存储库。

`PyPI(Python Package Index)`是python官方的第三方库的仓库，所有人都可以下载第三方库或上传自己开发的库到`PyPI`。`PyPI`推荐使用`pip`包管理器来下载第三方库。

### 2.基本命令

* 更新本地`pip`版本
```python
# 从python2.7.9开始自带pip
$ pip install --upgrade pip
```

* 安装软件包(如果本地已有吗，不会尝试更新版版本)
```python
$ pip3 install numpy
```

* 会自动更新本地版本的安装
```python
$ pip3 install -U numpy
```

* 从指定源安装
```python
# 以下命令指定使用清华提供的ipv6镜像源（临时使用指定源）
$ pip install -i https://pypi.tuna.tsinghua.edu.cn/simple some-package
```

* 将指定源设为pip的默认源
```python
$ pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple
```

* 从本地`wheel`文件安装
```python
$ pip install torch-1.2.0-cp36-cp36m-manylinux1_x86_64.whl
```

### 3.下载wheel格式的离线安装包

Pypi官网地址: [https://pypi.org/](https://pypi.org/)

在首页搜索模块名称

![](/assets/python032_01_1.PNG)

在搜索结果中找到目标模块

![](/assets/python032_02_1.PNG)

下载适合自己当前环境的安装包

![](/assets/python032_03_1.PNG)

