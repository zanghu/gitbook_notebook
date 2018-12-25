## Nginx: 自定义helloworld模块的例子

参考资料: 《Nginx模块开发与架构解析》, 陶辉

### 1.准备

* nginx安装包及其依赖库的安装包, 全部放置在`~/soft_setup`目录下
nginx-1.14.2.tar.gz
openssl-1.1.0h.tar.gz
pcre-8.40.tar.gz
zlib-1.2.11.tar.gz

* 将安装包依次解压
```shell
$ cd ~/soft_setup/
$ tar -zxvf nginx-1.14.2.tar.gz
$ tar -zxvf openssl-1.1.0h.tar.gz
$ tar -zxvf pcre-8.40.tar.gz
$ tar -zxvf zlib-1.2.11.tar.gz
```

* 自定义模块源码放置在`~/code_box/nginx_code/test_helloword`目录下，包括以下两个文件
ngx_http_mytest_module.c
config

note
> （1） config文件内容
```shell
ngx_addon_name=ngx_http_mytest_module
HTTP_MODULES="$HTTP_MODULES ngx_http_mytest_module"
NGX_ADDON_SRCS="$NGX_ADDON_SRCS $ngx_addon_dir/ngx_http_mytest_module.c"
```
> （2） ngx_http_mytest_module.c文件内容见单独示例

### 2.安装

* 配置
```shell
$ cd nginx-1.14.2/
$ ./configure --prefix=/home/zanghu/ProgramFiles/nginx/test_helloworld \
              --with-openssl=~/soft_setup/openssl-1.1.0h \
              --with-pcre=~/soft_setup/pcre-8.40 \
              --with-zlib=~/soft_setup/zlib-1.2.11 \
              --add-module=/home/zanghu/code_box/nginx_code/test_helloworld
```

* 编译安装
```shell
$ make -j4
$ make install
```

注意：
> ./config的--prefix的值中不能包含相对路径或~, 否则安装或运行时会出错

### 3.运行

* 进入安装目录
```shell
$ cd /home/zanghu/ProgramFiles/nginx/test_helloworld
```

* 修改配置文件
（1） 将配置文件conf/nginx.conf中的监听端口由80改为8008；
（2） 在配置文件conf/nginx.conf的http->server下增加一个location段，内容如下：
```shell
http {
    include       mime.types;
    default_type  application/octet-stream;

    ...

    server {
        listen       8008;
        server_name  localhost;

        location / {
            root   html;
            index  index.html index.htm;
        }
        
        location /test {
            mytest;
        }
```

* 启动nginx服务
```shell
$ ./nginx
```

注意：
> nginx默认监听80端口，但是有时会由于权限问题导致启动失败. 解决方法: 将conf/nginx.conf中的监听端口由80改为8008，保存退出

# 4.验证
启动nginx服务后，在其他机器上打开浏览器，输入: http://nginx服务所在机器ip:8008，显示如下页面：

![](/assets/nginx001.PNG)