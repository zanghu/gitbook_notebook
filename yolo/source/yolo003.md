## Yolo v3: 源码分析——数据配置文件与数据格式

参考资料（YOLO官网）：[https://pjreddie.com/darknet/yolo/](https://pjreddie.com/darknet/yolo/)

参考资料（CTW官网）：[https://ctwdataset.github.io/](https://ctwdataset.github.io/)

参考资料（ctw-baseline）: [https://github.com/yuantailing/ctw-baseline/blob/master/tutorial/3-detection.ipynb](https://github.com/yuantailing/ctw-baseline/blob/master/tutorial/3-detection.ipynb)

### 1.两个配置文件

YOLO（v2、v3）使用两种类型的配置文件：

* 数据配置文件：用于指明样本图片和真值信息文件的位置，以及模型参数保存路径和模型输出结果保存路径等。

* 网络结构配置文件: 指明网络的整体结构、优化算法和参数、每一层的结构。

#### 1.1.数据配置文件

##### 1.1.1.数据配置文件的特点

* 数据配置文件内容相对较少。

* 数据配置文件推荐以`.data`作为数据配置文件的后缀名，例如：YOLOv3的训练配置文件`voc.data`和ctw-baseline的`chinese.data`。

* 数据配置文件不包含以下内容：（1）预处理、样本增强、batch大小相关的信息；（2）不直接包含样本真值信息的路径（darknet默认样本图片路径与对应的真值信息文件的路径存在映射规则，训练时根据该规则有样本图片路径推断真值信息文件路径）；（3）不包含样本尺寸相关的信息（darknet的样本图片加载模块可以直接）

##### 1.1.2.数据配置文件示例

- 以下是`ctw-baseline`的数据配置文件：

```
backup = products/backup # 训练期间模型参数备份路径
classes = 1001 # 类标总数, ctw-baseline只考虑训练集中出现频率最高的前1000个汉字，其余全部归入额外一类. 训练阶段无用
names = products/chinese.names # 类标数字索引
results = products/results # 检测结果保存路径? 训练阶段无用
train = products/trainval.txt # 训练集样本图片路径信息文件
valid = products/test.txt # 验证集样本图片路径信息文件. 训练阶段无用
```

**注意**: 

（1）`darknet`的训练阶段`train_detector()`函数实际上只用到了数据配置文件中的`backup`和`train`两个配置项。其余配置项应该是用于其他阶段（例如测试）；

（2）`darknet`的训练过程中并未发现使用任何早停条件（early stopping），只是在训练主循环的每轮循环后会显示当前模型的loss值，训练结束条件是训练主循环达到预定次数。

（3）`darknet`的训练过程每一轮住循环计算loss的方法是在每次前向传递过程结尾收集所有cost属性不为空的layer的cost, 并将这些cost累加。因此本质上loss值是由本轮训练中所使用的全部训练样本所计算得到的。因此训练过程没有独立的验证集；

（4）`darknet`的训练过程每一次主循环中，先进行若干轮前向传递和反向传播，最后再进行一次网络参数更新，因此本轮主循环显示的loss值实际是由本轮所使用的训练数据在上一轮主循环更新参数后的模型上的结果。

- `chinese.names`文件内容示例（共1002行，包含1001个类标）

```
0
1
2
3
4
5
6
7
8
9
.
. # 省略部分内容
.
1000

```

- `trainval.txt`文件内容示例（共2407025行，包含2407024个样本）

```
products/trainval/0000172_6_0.jpg
products/trainval/0000172_6_1.jpg
products/trainval/0000172_6_2.jpg
products/trainval/0000172_6_13.jpg
products/trainval/0000172_7_0.jpg
products/trainval/0000174_6_0.jpg
products/trainval/0000174_6_1.jpg
products/trainval/0000174_6_2.jpg
products/trainval/0000174_6_3.jpg
.
. # 省略部分内容
.
products/trainval/3045638_4_11.jpg

```

- `test.txt`文件内容示例（共262837行，包含262836个样本）

```
products/test/0000238_0_0_0.jpg
products/test/0000238_0_0_1.jpg
products/test/0000238_0_0_2.jpg
products/test/0000238_0_0_3.jpg
products/test/0000238_0_0_4.jpg
products/test/0000238_0_0_5.jpg
products/test/0000238_0_0_6.jpg
products/test/0000238_0_0_7.jpg
products/test/0000238_0_1_0.jpg
.
. # 省略部分内容
.
products/test/3045448_2_1_1.jpg

```

**注意**：`ctw-baseline`中的每个样本图片都是从`ctw`样本集原始图中按照一定的方式（类似卷积过程的有交叠的水平+垂直扫描）“抠”出来的、经过筛选（是否包含有效bbox）的裁剪图，所以`trainval.txt`和`test.txt`中的训练样本数远高于`ctw`数据集对应子集的样本数。

### 2.数据加载模块和数据格式

机器学习模型的训练数据一般都要经过各种加工处理之后，才能制作成符合模型要求的输入数据格式。对于图片数据，各种预处理（缩放、像素值归一化等）和样本增强（平移、翻转、裁剪等等）也是必不可少的。

在`darknet`中，数据加工分为两部分：

* **STEP 1**: 由原始数据集生成符合`darknet`（数据加载器）要求格式的训练样本和真值信息，这步由`darknet`用户负责实现。

* **STEP 2**: 有输入数据生成内存中的模型输入数据，这步由`draknet`的数据加载器负责完成。

#### 2.1.从原始数据集到输入数据文件

* 生成`darknet`数据加载器要求的数据文需要的工作包括：  
  （1）将训练样本图片和真值信息文件按照指定目录格式和命名方式放置；  
  （2）确保样本图片的后缀名是`darknet`支持的格式；  
  （3）生成每个训练样本的真值信息文件；

* `darknet`数据加载器要求的数据格式：

##### 2.1.1.图片样本格式

图片分辨率不限，甚至不要求所有图片服从某个统一的分辨率；

图片后缀名必须是`.jpg`、`.png`、`.JPG`或`.JPEG`中之一；

如果编译`darknet`时打开了`OPENCV`选项，那么原则上应保证图片能够被OpenCV正确加载。

##### 2.1.2.真值信息文件格式

每张样本图片对应一个真值信息文件；

真值信息文件的名称应该与对应样本图片名称保持一致，但是后缀名换成`.txt`；

真值信息文件中每行是5个用空格分隔的数字，用来描述样本图的一个bbox；

每一行五元组含义是（id, bbox中心点x, bbox中心点y, bbox宽度, bbox高度）；

每一行五元组中后四元\(x, y, w, h\)的值并不是像素绝对值，而是相对于当前样本图片的边长的浮点数。即: \(\(x\_bbox\_center - bbox\_left\)/w\_img, \(y\_bbox\_ceter - bbox\_up\)/h\_img, w\_bbox/w\_img, h\_bbox/h\_img\)。这样做的好处是：将拟合值的取值范围压缩到\[0, 1\]。

每一行五元组中的id表示当前行所描述的bbox的类标编号，至少ctw-baseline在生成数据时是这么做的。

##### 2.1.3.实际例子

* **真值信息文件的例子**

ctw-baseline的真值信息文件的实例如下\(文件名称：0000172\_6\_0.txt， 对应样本名称：0000172\_6\_0.jpg）:

```
47 0.900858 0.287963 0.131950 0.228359
266 0.064299 0.246023 0.133742 0.228904
811 0.252052 0.246023 0.190325 0.228904
145 0.214759 0.962409 0.069443 0.081863
45 0.275200 0.962620 0.051439 0.080559
173 0.325353 0.962789 0.048867 0.079520
172 0.375506 0.962954 0.051439 0.078498
95 0.426945 0.963125 0.051439 0.077441
116 0.480957 0.963305 0.051439 0.076331
147 0.532396 0.963477 0.051439 0.075274
```

上述文件每一行内容是对样本图片的一个bbox的描述，（bbox类标编号, x, y, w, h）

* 样本和真值信息目录结构实例，仍然以ctw-baseline为例：

```
products/trainval/xxx.jpg
products/trainval/xxx.txt
```

darknet允许类标文件和样本文件不在同一个目录下，但是二者所在目录字符串相差的词只能是符合下表的对应关系的那些：

| 图片目录中的被替换词 | 类标目录中的替换词 |
| :---: | --- |
| images | labels |
| JPEGImages | labels |
| raw | labels |

例如下面的样本图片和真值文件布置路径也是`darknet`允许的：

```
products/trainval/images/xxx.jpg
products/trainval/labels/xxx.txt
```

##### 2.2.darknet数据加载模块

`darknet`的数据加载模块有多个数据加载函数（`src/data.c`中的`load_thread()`函数中进行选择），根据任务所需的数据类型（定义在`darknet.h`中的枚举类型`data_type`）来确定采用哪个函数加载数据。

根据所加载的数据类型的不同，又可以分为图片数据加载函数族和真值信息文件加载函数族。

究竟什么样的任务应该选择哪个数据类型值，是一件尚未研究清楚的事。根据对源码的阅读，训练时使用`DETECTION_DATA`，而使用训练好的模型检测单张图片时使用`LETTERBOX_DATA`。

##### 2.2.1.图片数据加载

这里只研究·src/detector.c·中的`train_detector()`函数的图片数据加载过程，并且假定用户编译时开启了`OPENCV`编译选项。

* 数据加载函数调用链

load\_data\_detection -&gt; load\_image\_color + jitter + random\_distort\_image + random\_flip

\(1\) load\_image\_color: 读取样本图片，在opencv模式下使用`cv::imread`读取图片，之后将读取到的数据转换为`darknet`的`image`对象。

转换过程：\*.jpg图片 -&gt; `cv::Mat`对象 -&gt; `cv::IplImage`对象 -&gt; `image`对象

\(2\) jitter: load\_data\_detection中的部分代码和`place_image()`函数共同实现了jitter，关于darknet中的jitter操作详情后续会专门论述；

\(3\) random\_distort\_image: 将图片表示变换到HSV颜色空间中，,s,v三通道上在添加噪声扰动；

\(4\) random\_flip: 50%概率对图片进行水平翻转。

* 最终加载到内存中的`image`对象的特性：

  -- 图片每次加载的图像增强操作是随机的

  -- 每张图片轴向是\(c, 0, 1\)

  -- 其中"c"轴的三个元的顺序是BGR

  -- 像素值是闭区间\[0, 1\]之间的浮点数（归一化用的是除以255）

##### 2.2.2.真值信息文件加载

真值信息文件加载过程: `src/data.c`中的函数：

```c
void fill_truth_detection(char *path, int num_boxes, float *truth, int classes, \
                          int flip, float dx, float dy, float sx, float sy);
```

加载真值信息时的处理中需要注意的包括以下几个：

（1）先加载样本，再加载对应的真值信息，因为真值信息需要按照样本加载时的样本增强操作进行相应的变换；  
（2）对每个样本的bbox随机乱序；  
（3）按照加载对应样本时的随机样本增强操作，对bbox信息进行必要的变换；  
（4）按照网络配置文件中的配置，限制每个样本的bbox个数。

