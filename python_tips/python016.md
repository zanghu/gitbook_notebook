## Python: Linux下搭建Jupyter notebook Server（SSL方式、HTTPS服务）

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

### 3.生成本地证书

```shell
$ openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout mykey.key -out mycert.pem
```

### 4.修改配置文件

打开配置文件`jupyter_notebook_config.py`，修改以下项：

```python
# 必填配置项
c.NotebookApp.ip = '*' # Server所在机器的ip

c.NotebookApp.port = 9999 # 远程访问端口

c.NotebookApp.open_browser = False

c.NotebookApp.password = u'sha1:f72472bc8463:1cadf26cdd31accce99aef90e178bb67a1f6a8d3' # 第二步生成的hash密码

# 可选配置项
c.NotebookApp.notebook_dir = '/home/zanghu/workspaces/jupyter_workspace'
```

### 5.验证安装

首先，在服务器端启动jupyter notebook server

```shell
$ jupyter notebook --certfile=mycert.pem --keyfile mykey.key
```

建议把`mykey.key`保存在指定的自定义目录下，同时在`~/.bashrc`中配置

```shell
$ vim ~/.bashrc
```

加入以下内容：

```shell
alias ipynb="jupyter notebook --certfile=/path/to/your/mycert.pem --keyfile /path/to/your/mykey.key"
```

刷新

```shell
$ source ~/.bashrc
```

这样今后每次启动只要在命令行输入`ipynb`即可。

之后，从其他机器上打开浏览器（建议使用IE，使用chrome会在输入密码后无法打开新页面导致进入密码输入页面的死循环，应该是由于chrome的安全机制导致），在地址栏输入“服务器所在机器的ip:监听的端口”：

`https://192.168.3.7:9999` 

回车

