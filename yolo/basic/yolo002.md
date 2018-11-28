## Yolo v3: 使用训练好的模型对单张图片进行检测

参考资料：[https://pjreddie.com/darknet/yolo/](https://pjreddie.com/darknet/yolo/)

### 1.所需材料

* 编译好的可执行文件：`darknet`；
* 网络结构的配置文件：`cfg/yolov3.cfg`；
* 预训练好的模型权值：`yolov3.weights`；
* 待测试的图片：      `data/dog.jpg`

其中唯一需要额外获取的就是预先训练好的模型，下载地址：[https://pjreddie.com/media/files/yolov3.weights](https://pjreddie.com/media/files/yolov3.weights)

### 2.执行检测

```shell
$ ./darknet detect cfg/yolov3.cfg yolov3.weights data/dog.jpg
layer     filters    size              input                output
    0 conv     32  3 x 3 / 1   608 x 608 x   3   ->   608 x 608 x  32  0.639 BFLOPs
    1 conv     64  3 x 3 / 2   608 x 608 x  32   ->   304 x 304 x  64  3.407 BFLOPs
    2 conv     32  1 x 1 / 1   304 x 304 x  64   ->   304 x 304 x  32  0.379 BFLOPs
    3 conv     64  3 x 3 / 1   304 x 304 x  32   ->   304 x 304 x  64  3.407 BFLOPs
...
  105 conv    255  1 x 1 / 1    76 x  76 x 256   ->    76 x  76 x 255  0.754 BFLOPs
  106 yolo
Loading weights from ../../model_zoo/darknet_models/yolo_v3/yolov3.weights...Done!
data/dog.jpg: Predicted in 0.035500 seconds.
dog: 100%
truck: 92%
bicycle: 99%

(predictions:16369): Gtk-WARNING **: cannot open display:
```

检测后的效果：

