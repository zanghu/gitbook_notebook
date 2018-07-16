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