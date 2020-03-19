## HTTP：GET和POST的安全性比较

参考资料



### 1.从保密性或者说密码学的意义下考虑的安全性

**结论：POST的安全性强于GET**

按照RFC7231定义，GET, HEAD, OPTIONS 和 TRACE 这几个方法是安全的。


> Of the request methods defined by this specification, the GET, HEAD, OPTIONS, and TRACE methods are defined to be safe.


但是面试官如果这么问，那么应该是想你能答出 get 请求的url是在服务器上有日志记录，在浏览器也能查到历史记录，但是post请求的参数都在body里面，服务器日志记录不到，浏览器历史也记录不到

### 2.从HTTP动词对服务端资源的影响的意义下考虑的安全性

**结论：GET的安全性强于POST**