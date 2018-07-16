# \[python\]快速配置 jupyter notebook 服务器（本文废弃，请参考最新版文章）

参考资料（官网）: http://jupyter-notebook.readthedocs.io/en/latest/public\_server.html

#### 1.生成本地配置文件

执行下面的命令后, 会生成~/.jupyter目录, 其中有jupyter notebook的配置文件jupyter\_notebook\_config.py

```python
jupyter notebook --generate-config
```

#### 2.生成sha1密码

进入ipython

```shell
ipython
```

在ipython下执行如下命令:

```python
In [1]: from notebook.auth import passwd
In [2]: passwd()
Enter password:
Verify password:
Out[2]: 'sha1:67c9e60bb8b6:9ffede0825894254b2e042ea597d771089e11aed'
```

#### 3.设置jupyter notebook配置文件

打开配置文件~/.jupyter\_notebook\_config.py  
修改以下行

```python
## The IP address the notebook server will listen on.
c.NotebookApp.ip = '*'

## The directory to use for notebooks and kernels.
c.NotebookApp.notebook_dir = u'/home/zanghu/ProgramFiles/workspaces/jupyter_workspace'

## Whether to open in a browser after starting. The specific browser used is
#  platform dependent and determined by the python standard library `webbrowser`
#  module, unless it is overridden using the --browser (NotebookApp.browser)
#  configuration option.
c.NotebookApp.open_browser = False


## Hashed password to use for web authentication.
#  
#  To generate, type in a python/IPython shell:
#  
#    from notebook.auth import passwd; passwd()
#  
#  The string should be of the form type:salt:hashed-password.
c.NotebookApp.password = u'sha1:40605d0ceb4a:8cfaf89d785a7d91d4e7ebb288eab9fb3efec385'


## The port the notebook server will listen on.
c.NotebookApp.port = 8888
```

保存退出, 修改后的配置文件参考:  
[assets/jupyter\_notebook\_config.py](/assets/jupyter_notebook_config.py)

#### 3.修改~/.bashrc, 建立快速启动

```shell
vim ~/.bashrc
```

加入下面的内容

```shell
# jupyter notebook
alias ipy="jupyter notebook"
```



