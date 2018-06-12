## Docker: 当前容器切换到后台运行

参考资料: [https://www.cnblogs.com/hanxing/p/7832178.html](https://www.cnblogs.com/hanxing/p/7832178.html)

Linux下有两个命令可以用来创建新用户：useradd和adduser


docker的linux镜像root用户没有默认密码，需要使用passwd命令手动设置
```shell
$ passwd
Enter new UNIX password:
Retype new UNIX password:
$
```
![](/assets/docker009_001.PNG)


具有sudo权限的用户都保存在/etc/sudoers文件中，可以使用下面的命令查看其中内容
```shell
$ cat /etc/sudoers
```

使用usemod命令将已存在的用户添加到指定用户组

```shell
# 给已有的用户增加工作组
$ usermod -G groupname username

# 例子，将用户zanghu添加到sudo用户组
$ usermod -G sudo zanghu
```

