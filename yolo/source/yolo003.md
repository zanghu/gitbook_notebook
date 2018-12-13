## Yolo v3: 源码分析——配置文件

参考资料：[https://pjreddie.com/darknet/yolo/](https://pjreddie.com/darknet/yolo/)

### 1.两个配置文件

YOLO各版本一直以来都使用两个（类）配置文件：

- 数据配置文件：用于指明样本和真值信息的位置，模型参数保存路径和模型输出结果保存路径等。

- 网络结构配置文件: 指明网络的整体结构、优化算法和每一层的结构

#### 1.1.数据配置文件

数据配置文件有以下特点：

- 数据配置文件内容相对较少。

- 习惯上数据配置文件以`.data`作为数据配置文件的后缀名，例如：YOLOv3的训练配置文件`voc.data`和ctw-baseline的`chinese.data`。

