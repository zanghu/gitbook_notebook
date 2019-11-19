## 网络：Cookie详解

参考资料：[cnblogs: HTTP报文结构及Cookie、session区别](https://www.cnblogs.com/plf-Jack/p/11105228.html)

参考资料：[掘金：一文搞懂Session和Cookie的用法及区别](https://juejin.im/post/5d8330996fb9a06b1d217bf5#heading-6)


### 1.Cookie

### 1.1.场景实例

在网上购物时，一个顾客要购买很多种物品。当他把选好的一件物品放入“购物车”后，他还有继续浏览和选购其他的物品。因此，服务器需要记住用户的身份，使他再接着选购的一些物品能够放入同一个“购物车”中，这样就便于集中结账。有时某些万维网站点也可能想限制某些用的访问。要做到这点，可以在HTTP中使用Cookie.

在RFC 2109中对Cookie进行了定义，规定万维网站点可以使用Cookie来跟踪用户。Cookie原意是“小甜饼”（广东人用方言音译为“曲奇”），目前尚无标准译名，在这里Cookie表示在HTTP服务器和客户之间传递的状态信息。现在很多网站都已广泛使用Cookie

### 1.2.工作原理：

当用户张三浏览某个使用Cookie的网站时，该网站的服务器就为张三产生一个唯一的识别码，并以此作为索引在服务器的后端数据库中产生一个项目。接着给张三的HTTP响应报文中添加一个叫做Set-cookie的首部行。这里的“首部字段名”就是"Set-cookie"，而后面的“值”就是赋予该用户的“识别码”。例如这个首部行时这样的：
Set-cookie: 12345678
张三收到响应后，其浏览器会在管理的特定Cookie文件中添加一行，包括这个服务器的主机名和Set-cookie后面给出的识别码。张三每发一个HTTP请求报文，浏览器就会从其Cookie文件中取出这个识别码，放到HTTP请求报文的Cookie首部行中：
Cookie: 12345678
于是，这个网站就能够跟踪用户1234567（张三）在该网站的活动。服务器不需要知道这个用户的姓名和其它信息，但能够知道它在什么时间访问了什么页面，访问的顺序等。如果是网上购物，服务器可以为张三维护一个所购物品列表，一起付费。
如果张三几天后访问这个网站，那么他的浏览器会在其HTTP请求报文中继续使用首部行Cookie: 12345678，而这个网站服务器根据张三过去的访问记录可以向他推荐商品

### 1.3.cookie中的主要内容：

cookie的内容主要包括：名字，值，过期时间，路径和域。 
其中域可以指定某一个域比如.google.com，相当于总店招牌，比如宝洁公司，也可以指定一个域下的具体某台机器比如www.google.com或者froogle.google.com，可以用飘柔来做比。 
路径就是跟在域名后面的URL路径，比如/或者/foo等等，可以用某飘柔专柜做比。 
路径与域合在一起就构成了cookie的作用范围。 
如果不设置过期时间，则表示这个cookie的生命期为浏览器会话期间，只要关闭浏览器窗口，cookie就消失了。这种生命期为浏览器会话期的cookie被称为会话cookie。会话cookie一般不存储在硬盘上而是保存在内存里，当然这种行为并不是规范规定的。如果设置了过期时间，浏览器就会把cookie保存到硬盘上，关闭后再次打开浏览器，这些cookie仍然有效直到超过设定的过期时间。 

存储在硬盘上的cookie可以在不同的浏览器进程间共享，比如两个IE窗口。而对于保存在内存里的cookie，不同的浏览器有不同的处理方式。对于IE，在一个打开的窗口上按Ctrl-N（或者从文件菜单）打开的窗口可以与原窗口共享，而使用其他方式新开的IE进程则不能共享已经打开的窗口的内存cookie；对于Mozilla Firefox0.8，所有的进程和标签页都可以共享同样的cookie。一般来说是用javascript的window.open打开的窗口会与原窗口共享内存cookie。浏览器对于会话cookie的这种只认cookie不认人的处理方式经常给采用session机制的web应用程序开发者造成很大的困扰。 

下面就是一个goolge设置cookie的响应头的例子 
HTTP/1.1 302 Found 
Location: http://www.google.com/intl/zh-CN/ 
Set-Cookie: PREF=ID=0565f77e132de138:NW=1:TM=1098082649:LM=1098082649:S=KaeaCFPo49RiA_d8; expires=Sun, 17-Jan-2038 19:14:07 GMT; path=/; domain=.google.com 
Content-Type: text/html 

### 1.4.Cookie的作用：

记住密码，下次自动登录
购物车功能
记录用户浏览数据，进行商品（广告）推荐

### 1.5.Cookie的缺陷：

Cookie会被附加在每个HTTP请求中，所以无形中增加了流量。
由于在HTTP请求中的Cookie是明文传递的，所以安全性成问题。（除非用HTTPS）
Cookie的大小限制在4KB左右。对于复杂的存储需求来说是不够用的。

### 2.Session详解

#### 2.1.简介

Session代表服务器与浏览器的一次会话过程，这个过程是连续的，也可以时断时续的。Session是一种服务器端的机制，Session 对象用来存储特定用户会话所需的信息

#### 2.2.工作原理

当用户访问到一个服务器，如果服务器启用Session，服务器就要为该用户创建一个SESSION，在创建这个SESSION的时候，服务器首先检查这个用户发来的请求里是否包含了一个SESSION ID，如果包含了一个SESSION ID则说明之前该用户已经登陆过并为此用户创建过SESSION，那服务器就按照这个SESSION ID把这个SESSION在服务器的内存中查找出来（如果查找不到，就有可能为他新创建一个），如果客户端请求里不包含有SESSION ID，则为该客户端创建一个SESSION并生成一个与此SESSION相关的SESSION ID。这个SESSION ID是唯一的、不重复的、不容易找到规律的字符串，这个SESSION ID将被在本次响应中返回到客户端保存，而保存这个SESSION ID的正是COOKIE，这样在交互过程中浏览器可以自动的按照规则把这个标识发送给服务器。

#### 2.3.作用

判断用户是否登录
购物车功能

### 3.Cookie和session的区别

1.存放位置不同
Cookie保存在客户端，Session保存在服务端。

2.存取方式的不同

Cookie中只能保管ASCII字符串，假如需求存取Unicode字符或者二进制数据，需求先进行编码。Cookie中也不能直接存取Java对象。若要存储略微复杂的信息，运用Cookie是比拟艰难的。 

而Session中能够存取任何类型的数据，包括而不限于String、Integer、List、Map等。Session中也能够直接保管Java Bean乃至任何Java类，对象等，运用起来十分便当。能够把Session看做是一个Java容器类

3.安全性（隐私策略）的不同

Cookie存储在浏览器中，对客户端是可见的，客户端的一些程序可能会窥探、复制以至修正Cookie中的内容。而Session存储在服务器上，对客户端是透明的，不存在敏感信息泄露的风险。 假如选用Cookie，比较好的方法是，敏感的信息如账号密码等尽量不要写到Cookie中。最好是像Google、Baidu那样将Cookie信息加密，提交到服务器后再进行解密，保证Cookie中的信息只要本人能读得懂。而假如选择Session就省事多了，反正是放在服务器上，Session里任何隐私都能够有效的保护

4.有效期的不同

只需要设置Cookie的过期时间属性为一个很大很大的数字，Cookie就可以在浏览器保存很长时间。 由于Session依赖于名为JSESSIONID的Cookie，而Cookie JSESSIONID的过期时间默许为–1，只需关闭了浏览器（一次会话结束），该Session就会失效。

5.对服务器造成的压力不同

Session是保管在服务器端的，每个用户都会产生一个Session。假如并发访问的用户十分多，会产生十分多的Session，耗费大量的内存。而Cookie保管在客户端，不占用服务器资源。假如并发阅读的用户十分多，Cookie是很好的选择

6.跨域支持上的不同

Cookie支持跨域名访问，例如将domain属性设置为“.baidu.com”，则以“.baidu.com”为后缀的一切域名均能够访问该Cookie。跨域名Cookie如今被普遍用在网络中。而Session则不会支持跨域名访问。Session仅在他所在的域名内有效。 