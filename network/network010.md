## telnet命令查看SMTP支持的身份认证协议

参考资料

[利用telnet进行SMTP的验证](http://www.cppblog.com/prayer/archive/2011/12/13/162021.html)

[SMTP的认证 - EHLO的使用](http://www.360doc.com/content/12/0218/15/3200886_187602886.shtml)



### 方法

以网易的SMTP邮件服务为例：

```
　　网易邮箱（163.com）:
　　POP3服务器地址:pop.163.com（端口：110）
　　SMTP服务器地址:smtp.163.com（端口：25）
　　
　　126邮箱:
　　POP3服务器地址：pop.live.com（端口：995）
　　SMTP服务器地址:smtp.126.com（端口：25）
```

命令行执行：
```shell
telnet smtp.163.com 25
```

