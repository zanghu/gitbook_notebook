## telnet命令查看SMTP支持的身份认证协议

参考资料

[利用telnet进行SMTP的验证](http://www.cppblog.com/prayer/archive/2011/12/13/162021.html)

[telnet收发邮件过程](https://www.cnblogs.com/h--d/p/6125993.html)

[SMTP的认证 - EHLO的使用](http://www.360doc.com/content/12/0218/15/3200886_187602886.shtml)



### 1.方法

以网易的SMTP邮件服务为例：

```
　　网易邮箱（163.com）:
　　POP3服务器地址:pop.163.com（端口：110）
　　SMTP服务器地址:smtp.163.com（端口：25）
　　
　　126邮箱:
　　POP3服务器地址：pop.live.com（端口：995）
　　SMTP服务器地址:smtp.126.com（端口：25）
```

* **STEP 1**: telnet邮件服务器地址

命令行执行：

```shell
telnet smtp.163.com 25
```

如下图：

![](/assets/network010_01.png)

* **STEP 2**: 使用EHLO命令查看邮件服务器支持的身份验证协议

```shell
EHLO localhost
```

或者

```shell
EHLO smtp.163.com
```

如下图：
![](/assets/network010_03.png)

2.知识

EHLO是extend helo，可以支持authorization，即用户认证。

EHLO是SMTP的必要步骤之一，由客户端发送，指示 ESMTP 会话开始。服务器可以在它对 EHLO 的响应中表明自己支持 ESMTP 命令。

HELO是普通SMTP，不带身份验证也可以继续MAIL FROM。。。下去，直到成功发送邮件，也就是可以伪造邮件啦！

EHLO是ESMTP，带有身份验证，所以没法伪造。

