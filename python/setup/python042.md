## 查看模块的依赖关系

参考资料: [清华大学Pypi镜像](https://mirror.tuna.tsinghua.edu.cn/help/pypi/)

参考资料：[Pypi官方网站](https://pypi.org/)

### 1.方法一：使用 pipdeptree 工具查看

In addition to the pip show [package name] command, there is pipdeptree.

Just do

```shell
$ pip install pipdeptree
```

then run

```
$ pipdeptree -p wordcloud # 想查看依赖关系的模块名
```

and it will show you your dependencies in a tree form, e.g.,

flake8==2.5.0
  - mccabe [required: >=0.2.1,<0.4, installed: 0.3.1]
  - pep8 [required: !=1.6.0,>=1.5.7,!=1.6.1,!=1.6.2, installed: 1.5.7]
  - pyflakes [required: >=0.8.1,<1.1, installed: 1.0.0]
ipdb==0.8
  - ipython [required: >=0.10, installed: 1.1.0]

`