## HTTP的标准文件传输方式：multipart/form-data

参考资料

[从 HTTP 角度看 Go 如何实现文件提交](https://juejin.cn/post/6844904017466753032)

[RFC 1867](https://tools.ietf.org/html/rfc1867)

### 1.场景

对于HTTP POST的body部分包含多种类型混杂，特别是包含上传文件类型时（例如某全文检索平台的某接口的请求报文body中同时包含json字符串和二进制串形式的文件）；

此外，HTTP POST并未提供上传多文件的支持（个人理解是如果body中同时包含不止一个文件，那么如何区分文件间的边界，是一件困难的事情）。因此如果需要上传多文件则需自定义上传协议，而且客户端和服务端都要提供相应的支持。

### 2.介绍

需要说明，`multipart/form-data`并不是一个新鲜事物，其协议草案`RFC 1867`制订于1995年11月

首先，HTTP HEADER中的`Content-Type`字段的取值如下：

`Content-Type: multipart/form-data; boundary=285fa365bd76e6378f91f09f4eae20877246bbba4d31370d3c87b752d350`

其次，请求报文中的body形如：

```
upload file1
--285fa365bd76e6378f91f09f4eae20877246bbba4d31370d3c87b752d350
Content-Disposition: form-data; name="uploadFile1"; filename="uploadfile2.txt"
Content-Type: application/octet-stream

upload file2
--285fa365bd76e6378f91f09f4eae20877246bbba4d31370d3c87b752d350
Content-Disposition: form-data; name="words"

123
--285fa365bd76e6378f91f09f4eae20877246bbba4d31370d3c87b752d350--
```

注意：报文中的内容被之前`Content-Type`中出现的boundary分割，这样就实现了清晰标记body中各部分内容的边界（这也就是为什么boundry字符串如此之长，应该是为了尽可能避免与文件内容中某一部分相同）。

### 3.支持

目前主流HTTP包、工具都支持`multipart/form-data`模式，并提供了简化的API。

* POSTMAN
![](/assets/network009_01.png)

* Golang
![](/assets/network009_02.png)



