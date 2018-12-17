## Yolo v3: ctw-baseline

参考资料（CTW官网）：[https://ctwdataset.github.io/](https://ctwdataset.github.io/)

参考资料（ctw-baseline）: [https://github.com/yuantailing/ctw-baseline/blob/master/tutorial/3-detection.ipynb](https://github.com/yuantailing/ctw-baseline/blob/master/tutorial/3-detection.ipynb)

YOLO学习的一大困难就是缺少详细的说明，官网上虽然给出了一些基本的训练命令并提供了开源代码，但是对于训练数据的正例组织确语焉不详。

CTW数据集官方 提供的`ctw-baseline`代码使用YOLOv2训练检测器，提供了使用YOLO的一个很好的范例。尤其是其中使用python编写的数据和配置文件生成代码，非常有学习价值。

### 1.训练数据和测试数据生成




### 2.伪多线程（真多进程）的训练机制
