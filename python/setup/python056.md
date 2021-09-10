## pip修改镜像源



参考资料: [anaconda修改镜像源](https://www.cnblogs.com/wzz2500/p/11427507.html)


### 1.国内源：
新版ubuntu要求使用https源，要注意。
清华：https://pypi.tuna.tsinghua.edu.cn/simple
阿里云：http://mirrors.aliyun.com/pypi/simple/
中国科技大学 https://pypi.mirrors.ustc.edu.cn/simple/
华中科技大学：http://pypi.hustunique.com/
山东理工大学：http://pypi.sdutlinux.org/
豆瓣：http://pypi.douban.com/simple/
清华的镜像源是最常用的，我试过几个镜像源之后，发现中科大的比较快质量不错。

### 2.修改方法

#### 2.1 对于Linux系统：

```shell
mkdir ~/.pip
cd ~/.pip
vi pip.conf
```

按i进入编辑模式，添加如下内容（来自制品库）：

```shell
[global]
index-url = https://pypi.mirrors.ustc.edu.cn/simple/
``

按ESC,再输入wq保存即可。

#### 2.2 对于windows系统：

```bat
cd C:\Users\(你的用户名)
mkdir pip
cd pip
cd.>pip.ini
```

然后打开`C:\Users(用户名)\pip\pip.ini`，在里面黏贴和linux系统一样的内容：

```bat
[global]
index-url = https://pypi.mirrors.ustc.edu.cn/simple/
```


