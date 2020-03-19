## 网络：Restful API 详解


参考资料

菜鸟教程：[RESTful 架构详解](https://www.runoob.com/w3cnote/restful-architecture.html)

### 1.要点总结

* URI、GET、POST、PUT、DELETE都不是HTTP特有的，实际上是Retful API的特性，只不过目前应用层协议只有HTTP遵循了Restful API的要求。

* GET、POST、PUT、DELETE的动词操作再HTTP中的原始定义，注意每一种操作的幂等性和安全性。

| 动词 | HTTP定义 | 幂等性 | 安全性 |
| :---: | --- |--- |--- |
| GET | HTTP GET方法用于获取资源，不应有副作用，所以是幂等的。| 是 | 安全 |
| POST | HTTP POST方法用于创建资源，所对应的URI并非创建的资源本身，而是去执行创建动作的操作者，有副作用，不满足幂等性。| 是 | 不安全 |
| PUT |
| DELETE |


