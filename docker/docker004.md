## DockerFile的常用命令

参考资料：[https://www.cnblogs.com/dazhoushuoceshi/p/7066041.html](https://www.cnblogs.com/dazhoushuoceshi/p/7066041.html)

FROM

功能为指定基础镜像，并且必须是第一条指令。

如果不以任何镜像为基础，那么写法为：FROM scratch。

同时意味着接下来所写的指令将作为镜像的第一层开始

语法：

FROM &lt;image&gt;  
FROM &lt;image&gt;:&lt;tag&gt;  
FROM &lt;image&gt;:&lt;digest&gt;  
三种写法，其中&lt;tag&gt;和&lt;digest&gt; 是可选项，如果没有选择，那么默认值为latest

