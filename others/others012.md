## 网络：Cookie详解

参考资料：[cnblogs: HTTP报文结构及Cookie、session区别](https://www.cnblogs.com/plf-Jack/p/11105228.html)

100: 同意大文件传输

200: 成功

300: 重定位、跳转引发的访问错误

400：传输层协议相关运行错误（拒绝链接、连接超时等等），一般就是TCP/IP层的socket接口调用错误

500：应用层协议相关运行错误，例如：SOAP通讯wsdl、xmlns错误，HTTP报文headercontent-type与body内容不符等，此类错误一般是因为服务端底层通讯框架（webservice框架、HTTP框架）逻辑错误直接返回的。