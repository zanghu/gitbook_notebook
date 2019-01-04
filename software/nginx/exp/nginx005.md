## Nginx: 常用操作命令

参考资料: 《Nginx模块开发与架构解析》, 陶辉

```shell

```

* 带有自定义模块
```shell
$ ./configure --prefix=/home/zanghu/ProgramFiles/nginx/test_helloworld \
              --with-openssl=~/soft_setup/openssl-1.1.0h \
              --with-pcre=~/soft_setup/pcre-8.40 \
              --with-zlib=~/soft_setup/zlib-1.2.11 \
              --add-module=/home/zanghu/code_box/nginx_code/test_helloworld
```
