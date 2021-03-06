### xlrd读取文件的缺陷

#### 1.问题描述

`xlrd.open_workbook()`默认情况下再返回前会解析excel文档的内部结构，且无法通过输入参数调整。
因此在读取大文档时，就不可避免的遇到打开时间过长的问题。

目前遇到一个15MB，包含40W+行的excel文档，`xlrd.open_workbook()`打开该文档的执行时间达到25s，通过输入参数设置为`on_demand=True`，没有取得明显的改善。

这就导致一个很糟糕的后果——无法通过多进程或其他并发策略实现文档读取加速，因为解析文档结构的操作实际上被封装在`xlrd.open_workbook()`中，没法拆分成子任务。

#### 2.解决方案

从stackoverfkow上找到的问题看，目前所有的处理excel文件的模块都是采用打开即加载的模式。

一种可能可行的的解决方案是：使用`sxl`模块

[https://github.com/ktr/sxl](https://github.com/ktr/sxl)