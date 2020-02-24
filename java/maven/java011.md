## Maven常用命令

参考资料：[cnblogs：maven中的groupId和artifactId到底指的是什么](https://www.cnblogs.com/zhaiyf/p/9077402.html)

### 1.概述

`groupid`和`artifactId`被统称为“坐标”是为了保证项目唯一性而提出的，如果你要把你项目弄到`maven`本地仓库去，你想要找到你的项目就必须根据这两个id去查找。

`groupId`一般分为多个段，这里我只说两段，第一段为域，第二段为公司名称。域又分为org、com、cn等等许多，其中org为非营利组织，com为商业组织。举个apache公司的tomcat项目例子：这个项目的`groupId`是`org.apache`，它的域是org（因为tomcat是非营利项目），公司名称是apache，`artigactId`是`tomcat`。

`artifactId`一般指项目名

### 2.举例

比如我创建一个项目，我一般会将`groupId`设置为`cn.zanghu`，cn表示域为中国，zanghu是我个人姓名缩写，`artifactId`设置为`testProj`，表示你这个项目的名称是testProj，依照这个设置，你的包结构最好是cn.snowin.testProj打头的，如果有个StudentDao，它的全路径就是cn.snowin.testProj.dao.StudentDao

