## Yolo v3: 使用训练好的模型对单张图片进行检测

参考资料（YOLO官网）：[https://pjreddie.com/darknet/yolo/](https://pjreddie.com/darknet/yolo/)

### 1.两个配置文件

YOLO（v2、v3）使用两种类型的配置文件：

* 数据配置文件：用于指明样本图片和真值信息文件的位置，以及模型参数保存路径和模型输出结果保存路径等。

* 网络结构配置文件: 指明网络的整体结构、优化算法和参数、每一层的结构。

#### 1.1.网络配置文件

网络配置文件远较数据配置文件大，内容也更复杂。

##### 1.1.1.网络配置文件的特点

* 内容较多. 以`yolov3.cfg`为例，其长度达到800+行；

* 采用“段落式”结构. 配置文件中每段一个用方括号括住的“标题行”开头，每一段包含若干行，每行描述该段的一个配置项。

* 配置文件中第一个“段落”必须是[net]或者[network]，用来对网络的整体配置项和优化算法进行描述。之后的每段是网络中一个层的配置项描述。述；

##### 1.1.2.网络配置文件示例

下面给出的是`cfg/yolov3.cfg`配置文件的部分内容，其中加入了部分注释。

```
[net]
# Testing
# batch=1
# subdivisions=1
# Training
batch=64
subdivisions=16
width=608
height=608
channels=3
momentum=0.9
decay=0.0005
angle=0
saturation = 1.5
exposure = 1.5
hue=.1

learning_rate=0.001
burn_in=1000
max_batches = 500200
policy=steps
steps=400000,450000
scales=.1,.1

[convolutional]
batch_normalize=1
filters=32
size=3
stride=1
pad=1
activation=leaky

# 省略后面的内容
...
```

经过人工统计，`cfg/yolov3.cfg`包含各类型层如下:

| 类型 | 出现次数 |
| :-: | :-: |
| yolo | 3 |
| convolutional | 75 |
| route | 4 |
| upsample | 2 |
| shortcut | 23 |

合计: 107



##### 1.1.3.网络配置文件解析机制

`darknet`将网络配置文件解析为一个"sections -- lines"结构的“二级嵌套链表”。

所谓“二级嵌套链表”，即配置文件解析后在内存中是一个链表对象，链表的每个元素是配置文件的一个段落，该元素本身又是一个链表对象（称为段落链表）。每个段落链表的元素是该段落的一个配置项。





