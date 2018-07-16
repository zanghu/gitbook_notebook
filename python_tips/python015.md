## Python: Linux下搭建Jupyter notebook Server

参考资料: [官方在线文档：Running a notebook server](http://jupyter-notebook.readthedocs.io/en/latest/public_server.htm)

### 1.生成jupyter配置文件

```shell
$ jupyter notebook --generate-config
```

生成的配置文件位于: `/home/USERNAME/.jupyter/jupyter_notebook_config.py`

### 2.生成密码

```shell
$ ipython
```

```python
In [1]: from notebook.auth import passwd
In [2]: passwd()
Enter password:
Verify password:
Out[2]: 'sha1:67c9e60bb8b6:9ffede0825894254b2e042ea597d771089e11aed'
```

### 3.修改配置文件

打开配置文件`jupyter_notebook_config.py`，修改以下项：

```python
# 必填配置项
c.NotebookApp.ip = '192.168.3.7' # Server所在机器的ip

c.NotebookApp.port = 9999 # 远程访问端口

c.NotebookApp.open_browser = False

c.NotebookApp.password = u'sha1:f72472bc8463:1cadf26cdd31accce99aef90e178bb67a1f6a8d3' # 第二步生成的hash密码

# 可选配置项
c.NotebookApp.notebook_dir = '/home/zanghu/workspaces/jupyter_workspace'
```

### 4.验证安装

从其他机器上打开浏览器，在地址栏输入：

`http://192.168.3.7:9999`

回车