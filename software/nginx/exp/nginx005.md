## Nginx: 常用操作命令

参考资料: 《Nginx模块开发与架构解析》, 陶辉

* 不带自定义模块
```shell
$ ./configure --prefix=/home/zanghu/ProgramFiles/nginx/test_basic \
              --with-openssl=~/soft_setup/openssl-1.1.0h \
              --with-pcre=~/soft_setup/pcre-8.40 \
              --with-zlib=~/soft_setup/zlib-1.2.11

```

* 带有自定义模块
```shell
$ ./configure --prefix=/home/zanghu/ProgramFiles/nginx/test_helloworld \
              --with-openssl=~/soft_setup/openssl-1.1.0h \
              --with-pcre=~/soft_setup/pcre-8.40 \
              --with-zlib=~/soft_setup/zlib-1.2.11 \
              --add-module=/home/zanghu/code_box/nginx_code/test_helloworld
```

* 打开debug功能（启动debug日志还需要在nginx.conf中配置）
```shell
--with-debug
```

* 增加编译参数
```shell
# 编译器参数
--cc-opt=...

# 链接器参数
--ld-opt=...
```
