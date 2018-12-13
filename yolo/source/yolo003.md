## Yolo v3: 源码分析——配置文件

参考资料：[https://pjreddie.com/darknet/yolo/](https://pjreddie.com/darknet/yolo/)

### 1.两个配置文件

YOLO各版本一直以来都使用两个（类）配置文件：

- 数据集配置文件：主要用于指明样本和真值信息的位置，模型参数保存路径和模型输出结果保存路径。

- 网络结构配置文件