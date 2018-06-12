## Docker: 当前容器切换到后台运行

参考资料: [https://www.cnblogs.com/hanxing/p/7832178.html](https://www.cnblogs.com/hanxing/p/7832178.html)

### 1.添加新用户

Linux下有两个命令可以用来创建新用户：useradd和adduser

* **useradd**

在root权限下，useradd只是创建了一个用户名，并没有在/home目录下创建同名文件夹，也没有创建密码，因此利用这个用户登录系统，是登录不了的，为了避免这样的情况出现，可以用 （useradd -m +用户名）的方式创建，它会在/home目录下创建同名文件夹，然后利用（ passwd + 用户名）为指定的用户名设置密码。

使用useradd添加用户的命令：

```shell
$ useradd -m 用户名  然后设置密码  passwd 用户名
```

* **adduser**

adduser创建新用户时在/home目录下会自动创建同名文件夹（创建用户的家目录），会建立同名组，建立新用户密码，还会从/etc/SKEL目录下拷贝文件到家目录，完成初始化、是否加密主目录等等。

使用adduser添加用户的命令：

```shell
$ adduser + 用户名  
```

docker的linux镜像root用户没有默认密码，需要使用passwd命令手动设置

```shell
$ passwd
Enter new UNIX password:
Retype new UNIX password:
$
```

![](/assets/docker009_001.PNG)

有资料显示有可能系统上没有自带passwd命令，需要先安装之（ubuntu18.04上没有遇到此问题）。


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

