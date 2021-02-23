## 网络：Cookie详解

有代表性、比较具体且零散的技术问题的汇总备忘

### 问题：Eclipse查看某个方法被哪些类调用

答：[https://www.cnblogs.com/jpfss/p/9435911.html](https://www.cnblogs.com/jpfss/p/9435911.html)

* 方法一：打开该类，在类的定义上即类名上，右键-->References--->Project ,就可以查看该类是否被工程中的其他Java文件引用过；但是如果在JSP页面，这个方法查不出来

* 方法二：打开eclipse，使用快捷键，Ctrl+h,会弹出一个对话框，找到File Search这个页签，在Containing Text下的文本框中输入要查找的这个类名，然后点击Search按钮，就会在该项目下，全局查找该类名。这种查找方法，可以把所有文件（包括java文件，以及JSP文件）中引用该类的都能找出来

### 问题：框架或容器以何为依据判断session超时？

问：编程框架、服务容器一般都支持配置Session超时时间，所以一般的Session超时不需要开发者控制（除了设置这个超时时间），那么它们内部是以何为依据判定超时的呢？

答：底层框架、容器会自动记录Session的不活跃时间、以及SessionID最近一次请求，每次收到请求都会刷新不活跃时间，只有不活跃时间超过开发者设置的Session超时时间才会认为Session超时失效。

回答者：赵祥宇