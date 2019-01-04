## Nginx: 常用操作命令

参考资料: 《Nginx模块开发与架构解析》, 陶辉

### 1.操作

* 启动
```shell
$ cd path/to/nginx/dir
$ cd sbin
$ ./nginx
```

* 立即终止
```sehll
$ ./nginx -s stop
```

* 优雅终止
```sehll
$ ./nginx -s quit
```

### 2.运维

* 查看指定端口被哪些进程占用
```shell
# 例如: lsof -i:8008
$ lsof -i:[端口号]
```

* 强制结束指定进程
```shell
$ kill -s SIGKILL [进程号]
```