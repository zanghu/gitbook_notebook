## 网络：Restful API 详解


参考资料

菜鸟教程：[RESTful 架构详解](https://www.runoob.com/w3cnote/restful-architecture.html)

### 1.要点总结

* URI、GET、POST、PUT、DELETE都不是HTTP特有的，实际上是Retful API的特性，只不过目前应用层协议只有HTTP遵循了Restful API的要求。

* GET、POST、PUT、DELETE的动词操作再HTTP中的原始定义，注意每一种操作的幂等性和安全性。

| 动词 | HTTP定义 | 幂等性 | 安全性 |
| --- | --- |--- |--- |
| GET | HTTP GET方法用于获取资源，不应有副作用，所以是幂等的。 | 是 | 安全 |
| POST | HTTP POST方法用于创建资源，所对应的URI并非创建的资源本身，而是去执行创建动作的操作者，有副作用，不满足幂等性。 | 否 | 不安全 |
| PUT | HTTP PUT方法用于创建或更新操作，所对应的URI是要创建或更新的资源本身，有副作用，它应该满足幂等性。 | 是 | 不安全 |
| DELETE | HTTP DELETE方法用于删除资源，有副作用，但它应该满足幂等性。 | 是 | 不安全 |

注意这里安全性的定义是动词是否会对服务端维护的资源进行修改。

* **客户端获取的只是资源的表述而已，而不是资源本身**。 例如文本资源可以采用html、xml、json等格式，图片可以使用PNG或JPG展现出来。那么客户端如何知道服务端提供哪种表述形式呢?答案是可以通过HTTP内容协商，客户端可以通过Accept头请求一种特定格式的表述，服务端则通过Content-Type告诉客户端资源的表述形式。

