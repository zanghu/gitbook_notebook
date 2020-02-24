## 查看Python模块的依赖关系

参考资料: [StackOverflow: How to find a Python package's dependencies](https://stackoverflow.com/questions/29751572/how-to-find-a-python-packages-dependencies)

参考资料：[StackOverflow: List dependencies of Python wheel file](https://stackoverflow.com/questions/50170588/list-dependencies-of-python-wheel-file)

### 1.使用 pipdeptree 工具查看

`piptree`工具的详细用法（包括如何查看模块的反向依赖关系、生成`requirements.txt`文件的方法等等）

见官网：https://pypi.org/project/pipdeptree/

In addition to the pip show [package name] command, there is pipdeptree.

Just do

```shell
$ pip install pipdeptree
```

then run

```
$ pipdeptree -p wordcloud # 假设 wordcloud 是想查看依赖关系的模块名
```

and it will show you your dependencies in a tree form, e.g.,

```shell
wordcloud==1.6.0
  - matplotlib [required: Any, installed: 3.1.1]
    - cycler [required: >=0.10, installed: 0.10.0]
      - six [required: Any, installed: 1.12.0]
    - kiwisolver [required: >=1.0.1, installed: 1.1.0]
      - setuptools [required: Any, installed: 41.4.0]
    - numpy [required: >=1.11, installed: 1.17.3]
    - pyparsing [required: >=2.0.1,!=2.1.6,!=2.1.2,!=2.0.4, installed: 2.4.2]
    - python-dateutil [required: >=2.1, installed: 2.8.0]
      - six [required: >=1.5, installed: 1.12.0]
  - numpy [required: >=1.6.1, installed: 1.17.3]
  - pillow [required: Any, installed: 6.2.0]
```

> 缺点
> 此方法查看效果不稳定，对于某些模块（例如：torch、tensorflow等）无法准确显示依赖；

### 2.使用 pkginfo 工具查看*.whl安装包中的依赖

第一步：安装`pkginfo`工具

```shell
$ pip install pkginfo
```

第二步：查看指定whl安装包的依赖关系（模块的whl安装包可以从pypi网站上下载）

```shell
$ pkginfo tensorflow_gpu-1.14.0-cp36-cp36m-manylinux1_x86_64.whl
```

第三步：查看显示结果：

```shell
metadata_version: 2.0
name: tensorflow-gpu
version: 1.14.0
platforms: ['UNKNOWN']
summary: TensorFlow is an open source machine learning framework for everyone.
description: TensorFlow is an open source software library for high performance numerical
computation. Its flexible architecture allows easy deployment of computation
across a variety of platforms (CPUs, GPUs, TPUs), and from desktops to clusters
of servers to mobile and edge devices.

Originally developed by researchers and engineers from the Google Brain team
within Google's AI organization, it comes with strong support for machine
learning and deep learning and the flexible numerical computation core is used
across many other scientific domains.



keywords: tensorflow tensor machine learning
home_page: https://www.tensorflow.org/
author: Google Inc.
author_email: packages@tensorflow.org
license: Apache 2.0
classifiers: ['Development Status :: 5 - Production/Stable', 'Intended Audience :: Developers', 'Intended Audience :: Education', 'Intended Audience :: Science/Research', 'License :: OSI Approved :: Apache Software License', 'Programming Language :: Python :: 2', 'Programming Language :: Python :: 2.7', 'Programming Language :: Python :: 3', 'Programming Language :: Python :: 3.4', 'Programming Language :: Python :: 3.5', 'Programming Language :: Python :: 3.6', 'Programming Language :: Python :: 3.7', 'Topic :: Scientific/Engineering', 'Topic :: Scientific/Engineering :: Mathematics', 'Topic :: Scientific/Engineering :: Artificial Intelligence', 'Topic :: Software Development', 'Topic :: Software Development :: Libraries', 'Topic :: Software Development :: Libraries :: Python Modules']
download_url: https://github.com/tensorflow/tensorflow/tags
requires_dist: ['absl-py (>=0.7.0)', 'astor (>=0.6.0)', 'gast (>=0.2.0)', 'google-pasta (>=0.1.6)', 'keras-applications (>=1.0.6)', 'keras-preprocessing (>=1.0.5)', 'numpy (<2.0,>=1.14.5)', 'six (>=1.10.0)', 'protobuf (>=3.6.1)', 'tensorboard (<1.15.0,>=1.14.0)', 'tensorflow-estimator (<1.15.0rc0,>=1.14.0rc0)', 'termcolor (>=1.1.0)', 'wrapt (>=1.11.1)', 'grpcio (>=1.8.6)', 'wheel (>=0.26)']
```

上面显示的`requires_dist`开头的一行内容即是依赖关系。

> 缺点
> 此方法只能显示安装包的第一级依赖关系，不能显示递归依赖关系；

### 3.使用 pip show 命令

Try to use show command in pip, for example:

```shell
$ pip show tornado
---
Name: tornado
Version: 4.1
Location: *****
Requires: certifi, backports.ssl-match-hostname

```

> 缺点
> 此方法现实的依赖关系中不会显示所依赖模块的指定版本好，过于粗糙；

