## 开源软件 Consul 简介

### 1.Consul概述

官方网站：[https://www.consul.io/](https://www.consul.io/)

![](/assets/linkerd002_001.PNG)

Consul是google开源的、Go语言编写的软件。  
主要用于（1）分布式集群动态配置;（2）自动服务发现. 除此之外还有节点健康检查等功能.

consul的基本架构  
agent-server  
api需要在consul上注册，注册方式有两种：（1）http接口协议注册;（2）json文件注册.  
agent用于节点健康检查，收集信息  
server用于收集和存储信息。  
多个server构成一个集群，集群通过选举选出一个leader，leader负责集群的各个server间数据同步.

当希望使用consul进行服务发现时，

### 2.进一步的资料

参考资料: [https://www.cnblogs.com/cuishuai/p/8194345.html](https://www.cnblogs.com/cuishuai/p/8194345.html)

consul是google开源的一个使用go语言开发的服务发现、配置管理中心服务。内置了服务注册与发现框 架、分布一致性协议实现、健康检查、Key/Value存储、多数据中心方案，不再需要依赖其他工具（比如ZooKeeper等）。服务部署简单，只有一个可运行的二进制的包。每个节点都需要运行agent，他有两种运行模式server和client。每个数据中心官方建议需要3或5个server节点以保证数据安全，同时保证server-leader的选举能够正确的进行。

@client

CLIENT表示consul的client模式，就是客户端模式。是consul节点的一种模式，这种模式下，所有注册到当前节点的服务会被转发到SERVER，本身是不持久化这些信息。

@server

SERVER表示consul的server模式，表明这个consul是个server，这种模式下，功能和CLIENT都一样，唯一不同的是，它会把所有的信息持久化的本地，这样遇到故障，信息是可以被保留的。

@server-leader

中间那个SERVER下面有LEADER的字眼，表明这个SERVER是它们的老大，它和其它SERVER不一样的一点是，它需要负责同步注册的信息给其它的SERVER，同时也要负责各个节点的健康监测。

@raft

server节点之间的数据一致性保证，一致性协议使用的是raft，而zookeeper用的paxos，etcd采用的也是taft。

@服务发现协议

consul采用http和dns协议，etcd只支持http

@服务注册

consul支持两种方式实现服务注册，一种是通过consul的服务注册http API，由服务自己调用API实现注册，另一种方式是通过json个是的配置文件实现注册，将需要注册的服务以json格式的配置文件给出。consul官方建议使用第二种方式。

@服务发现

consul支持两种方式实现服务发现，一种是通过http API来查询有哪些服务，另外一种是通过consul agent 自带的DNS（8600端口），域名是以NAME.service.consul的形式给出，NAME即在定义的服务配置文件中，服务的名称。DNS方式可以通过check的方式检查服务。

@服务间的通信协议

Consul使用gossip协议管理成员关系、广播消息到整个集群，他有两个gossip  pool（LAN pool和WAN pool），LAN pool是同一个数据中心内部通信的，WAN pool是多个数据中心通信的，LAN pool有多个，WAN pool只有一个。

