## Nginx: 常用操作命令

参考资料: 《Nginx模块开发与架构解析》, 陶辉

### 1.准备

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

