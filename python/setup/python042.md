## 查看模块的依赖关系

参考资料: [清华大学Pypi镜像](https://mirror.tuna.tsinghua.edu.cn/help/pypi/)

参考资料：[Pypi官方网站](https://pypi.org/)

### 1.使用 pipdeptree 工具查看

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

> note
> 此方法查看效果不稳定，对于某些模块（例如：torch、tensorflow等）无法准确显示依赖；

### 2.使用 pkginfo 工具查看*.whl安装包中的依赖

第一步：安装`pkginfo`工具

```shell
$ pip install pkginfo
```

第二步：
