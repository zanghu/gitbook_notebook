## 工具：清华大学IPV6资源

参考资料：[https://mirror.tuna.tsinghua.edu.cn/](https://mirror.tuna.tsinghua.edu.cn/)

参考资料：[清华大学IPV6 DNS](https://tuna.moe/help/dns/)

参考资料: [清华大学Pypi镜像](https://mirror.tuna.tsinghua.edu.cn/help/pypi/)

参考资料：[清华大学Ubuntu软件源](https://mirror.tuna.tsinghua.edu.cn/help/ubuntu/)


经测试，在校园网不需要登陆外网账号的情况下即可访问清华IPV6资源。

### 1.DNS

* 打开配置文件
```shell
sudo cp  /etc/resolv.conf   /etc/resolv.con.fbak    # 备份原有dns配置文件
sudo vim  /etc/resolv.conf   # 编辑配置文件,添加以下内容
```

* 添加以下内容
```shell
nameserver 101.6.6.6   # 设置首选dns
nameserver 2001:da8::666 # 设置备用dns
# 保存配置并退出
```
### 2.pip的Pipy资源库

* 从指定源安装
```python
# 以下命令指定使用清华提供的ipv6镜像源（临时使用指定源）
$ pip install -i https://pypi.tuna.tsinghua.edu.cn/simple some-package
```

* 将指定源设为pip的默认源
```python
$ pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple
```

* 更新本地`pip`版本
```python
# 从python2.7.9开始自带pip
$ pip install --upgrade pip
```

* 安装软件包(如果本地已有吗，不会尝试更新版版本)
```python
$ pip3 install numpy
```

### 3.配置Ubuntu软件源

Ubuntu 的软件源配置文件是 `/etc/apt/sources.list`。将系统自带的该文件做个备份，将该文件替换为下面内容，即可使用 TUNA 的软件源镜像。

注意选择适合自己系统版本的软件源。

以下是Ubuntu16.04LTS的源
```shell
# 默认注释了源码镜像以提高 apt update 速度，如有需要可自行取消注释
deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ xenial main restricted universe multiverse
# deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ xenial main restricted universe multiverse
deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ xenial-updates main restricted universe multiverse
# deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ xenial-updates main restricted universe multiverse
deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ xenial-backports main restricted universe multiverse
# deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ xenial-backports main restricted universe multiverse
deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ xenial-security main restricted universe multiverse
# deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ xenial-security main restricted universe multiverse

# 预发布软件源，不建议启用
# deb https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ xenial-proposed main restricted universe multiverse
# deb-src https://mirrors.tuna.tsinghua.edu.cn/ubuntu/ xenial-proposed main restricted universe multiverse
```










