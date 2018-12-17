## Yolo v3: 使用训练好的模型对单张图片进行检测

参考资料：[https://pjreddie.com/darknet/yolo/](https://pjreddie.com/darknet/yolo/)

### 1.卷积层——convolutional

`darknet`的卷积层与普通卷积层基本相同，只需要注意以下几个特点：

- 只支持正方形的感受野. 这一点是由于layer只有一个size属性用来设置卷积核感受野的长和宽；

- 运算顺序是：

### 2.汇集层——route


### 3.捷径层——shortcut


### 4.上/下采样层——upsample


### 5.yolo
