## Yolo v3: 使用训练好的模型对单张图片进行检测

参考资料：[https://pjreddie.com/darknet/yolo/](https://pjreddie.com/darknet/yolo/)

### 1.卷积层——convolutional

`darknet`的卷积层与普通卷积层基本相同，只需要注意以下几个特点：

- 只支持正方形的感受野. 这一点是由于layer只有一个size属性用来设置卷积核感受野的长和宽；

- 运算顺序是：conv -> batch normalize或+biase选择其一 -> 非线性激活

- 卷积层不包含池化过程。

事实上，`cfg/yolov3.cfg`配置文件中也未包含任何池化层，猜测可能的原因是对于池化（pooling），存在以下 4 点争论（源于Hinton的PPT）：
 
1.不合乎我们对形态感知的心理认知。 它无法解释为什么把固有坐标系分派给对象后，会有如此明显的效果。
 
2.它在解决的是错误的问题。我们想要的是 equivariance，不是 invariance。想要的是 Disentangling，而不是 discarding。

3.它无法使用基本的线性结构。它不能利用能够完美处理图像中大量variance的自然线性流形。

4.池化对于做动态routing也很差。我们需要route进入神经网络的输入的每一部分，好知道如何处理它。找到最好的 routing 相当于为图像做parsing。

### 2.汇集拼接层——route

支持将任意多个层的输出作为自身的输入，将全部输入张量concatenate成一个张量，作为当前层的输出。

- route_layer只有线性运算部分（concatenate）, 没有非线性运算部分；

- 拼接时直接将多个层输出的float数组首尾相接（每一个输入都是保存在float数组中的张量,）, 没有加入轴向的因素. 因此对输入张量的形状尺寸没有要求；

### 3.跨越传输层——shortcut


### 4.上/下采样层——upsample


### 5.yolo
