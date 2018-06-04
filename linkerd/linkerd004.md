## Linkerd官网宣称六大特性分析

参考资料: [https://linkerd.io/](https://linkerd.io/)

### 1.linkerd六大特性
* FAST, SCALABLE, AND PERFORMANT——快速、可伸缩和高性能
  
* RUNTIME TRAFFIC ROUTING——运行时交通路由

* ANY LANGUAGE, ANY ENVIRONMENT——任意语言、任意环境

* DROP-IN SERVICE DISCOVERY——附带（顺便）式服务发现

* LATENCY-AWARE LOAD-BALANCING——延迟感知 + 负载均衡

* PRODUCTION-TESTED AND PROVEN AT SCALE——已经过生产环境规模的测试和验证

上述六个特性的官网描述如下图所示：

![](/assets/linkerd004_001.PNG)

  **小知识：关于“at scale”的准确含义**
  
  在百度搜索词组“at scale”，翻译为：
  
  ![](/assets/linkerd004_002.PNG)
  
  但是原句"PRODUCTION-TESTED AND PROVEN AT SCALE"翻译成“经过按比例的生产测试和验证”显然有些别扭，于是Google之，得到如下答案：
  
  ![](/assets/linkerd004_003.PNG)
  
  答案大体上有两个有意思：
  
  含义一：at scale在上图中两个例子中表达的是“与真实一致的规模下”或“足够大的规模下”的意思。
  
  含义二：之所以使用 at scale 而不是直接用“on a large scale”是因为在例子的语境中所谓“大”的概念会随着时间退役而发生变化，at scale可以更准确的表达“始终与语境中描述的规模相一致”这一隐含意思，带有自适应的意味。