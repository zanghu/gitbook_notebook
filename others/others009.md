## 工具：清华大学IPV6资源

参考资料：[https://mirror.tuna.tsinghua.edu.cn/](https://mirror.tuna.tsinghua.edu.cn/)

参考资料：[清华大学IPV6 DNS](https://tuna.moe/help/dns/)

参考资料: [清华大学Pypi镜像](https://mirror.tuna.tsinghua.edu.cn/help/pypi/)


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










