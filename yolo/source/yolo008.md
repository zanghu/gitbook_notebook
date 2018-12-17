## Yolo v3: ctw-baseline

参考资料（CTW官网）：[https://ctwdataset.github.io/](https://ctwdataset.github.io/)

参考资料（ctw-baseline）: [https://github.com/yuantailing/ctw-baseline/blob/master/tutorial/3-detection.ipynb](https://github.com/yuantailing/ctw-baseline/blob/master/tutorial/3-detection.ipynb)

YOLO学习的一大困难就是缺少详细的说明，官网上虽然给出了一些基本的训练命令并提供了开源代码，但是对于训练数据的正例组织确语焉不详。

CTW数据集官方 提供的`ctw-baseline`代码使用YOLOv2训练检测器，提供了使用YOLO的一个很好的范例。尤其是其中使用python编写的数据和配置文件生成代码，非常有学习价值。

### 1.训练数据和测试数据生成

训练数据生成方式: `detection/prepare_train_data.py`，以下是注释版的源码。

```python
# -*- coding: utf-8 -*-

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
from __future__ import unicode_literals

import cv2
import darknet_tools
import json
import numpy as np
import os
import settings

from jinja2 import Template
from pythonapi import anno_tools, common_tools
from six.moves import queue


def write_darknet_data():
    """生成darknet训练用数据配置文件"""
    if not os.path.exists(settings.DARKNET_BACKUP_DIR):
        os.makedirs(settings.DARKNET_BACKUP_DIR)
    if not os.path.exists(settings.DARKNET_RESULTS_DIR):
        os.makedirs(settings.DARKNET_RESULTS_DIR)
    data = {
        'classes': settings.NUM_CHAR_CATES + 1,
        'train': settings.DARKNET_TRAIN_LIST,
        'valid': settings.DARKNET_VALID_LIST,
        'names': settings.DARKNET_NAMES,
        'backup': settings.DARKNET_BACKUP_DIR,
        'results': settings.DARKNET_RESULTS_DIR,
    }
    with open(settings.DARKNET_DATA, 'w') as f:
        for k, v in sorted(data.items()):
            f.write('{} = {}\n'.format(k, v))


def write_darknet_cfg():
    """生成darknet训练用网络配置文件"""
    # 脱离flask单独使用jinja2的模板功能
    with open('yolo-chinese.template.cfg') as f:
        template = Template(f.read())
    with open(settings.DARKNET_CFG, 'w') as f:
        # 注意: image_size填写的是width=672, height=672(代表网络模型输入要求是672x672), 但是实际上生成的训练样本是168x168
        # 这是因为darknet在加载图片时会自动进行放缩和其他样本增强变换, 使得输入图片尺寸和网络配置文件中规定的图片尺寸无关(详见darknet的data.c)
        f.write(template.render({
            'testing': False,
            'image_size': settings.TRAIN_IMAGE_SIZE, # TRAIN_IMAGE_SIZE = 672
            'classes': settings.NUM_CHAR_CATES + 1, # 1001类
            'filters': 25 + 5 * (settings.NUM_CHAR_CATES + 1), # 可能是5 *(5 + (settings.NUM_CHAR_CATES + 1)), 第一个5代表每点处预测5个标定框, 第二个5表示每个标定框4个坐标加上一个置信度, settings.NUM_CHAR_CATES + 1表示每个类的概率
        }))
        f.write('\n')


def write_darknet_names():
    """生成darknet类标描述文件, 每一类占一行"""
    with open(settings.DARKNET_NAMES, 'w') as f:
        for i in range(settings.NUM_CHAR_CATES + 1): # NUM_CHAR_CATES = 1000
            f.write('{}\n'.format(i))


def crop_train_images():
    """从原始样本集图片和真值信息文件train.jsonl、val.jsonl, 生成darknet训练用样本(裁剪图)和真值信息文件"""
    imshape = (2048, 2048, 3) # 原始图形状尺寸

    # 裁剪图形状尺寸
    # python3双斜线除法表示地板除, 向下取整
    # (672//4, 672 // 4) = (168, 168)
    cropshape = (settings.TRAIN_IMAGE_SIZE // 4, settings.TRAIN_IMAGE_SIZE // 4) # TRAIN_IMAGE_SIZE = 672

    # 原始图上的相邻裁剪图之间的重叠部分长度
    cropoverlap = (16, 16)

    # cates.json文件中保存一个列表，内容如下 ：
    # [
    #  {
    #    "cate_id": 0, # 数字类标, 等于该类标在样本集中的出现次数
    #    "text": "中", # 类标汉字
    #    "trainval": 13924 # 类标在数据集中出现的次数
    #  },
    #  ... # 剩余元素
    #  ]
    with open(settings.CATES) as f: # CATES = 'products/cates.json'
        cates = json.load(f)
    text2cate = {c['text']: c['cate_id'] for c in cates} # "汉子类标 -> 数字类标"的映射关系表

    # 创建保存训练样本和真值信息文件的目录
    if not os.path.isdir(settings.TRAINVAL_CROPPED_DIR): # TRAINVAL_CROPPED_DIR = 'products/trainval'
        os.makedirs(settings.TRAINVAL_CROPPED_DIR)

    # train.jsonl和val.jsonl中每一行是一张样本图片的全部信息(包含全部真值bbox的信息)
    with open(settings.TRAIN) as f: # TRAIN = '../data/annotations/train.jsonl'
        lines = f.read().splitlines()
    with open(settings.VAL) as f: # VAL = '../data/annotations/val.jsonl'
        lines += f.read().splitlines()

    def in_image_ratio(bbox):  # bbox is in darknet bbox representation
        """
        @brief 计算bbox与裁剪图相交部分面积与bbox总面积的比值
        @param bbox 真值bbox的坐标, 采用"中心点+宽长"方法表示, 且坐标和变长相对于裁剪图的边长做了"归一化"
        """
        xmid, ymid, w, h = bbox

        def cutto01(x):
            return max(0, min(1, x))
        # 计算bbox与裁剪图相交部分的面积. Acut = bbox落在裁剪图内的宽度 * bbox落在裁剪图内的高度
        Acut = (cutto01(xmid + w / 2) - cutto01(xmid - w / 2)) * (cutto01(ymid + h / 2) - cutto01(ymid - h / 2))
        return Acut / (w * h)

    # line是train.jsonl或val.jsonl中的一行
    def crop_once(line, write_images):
        """
        @brief 按照一定的尺寸和步长从原始图中裁剪出一系列子图, 从其中中选出有效的子图作为训练输入样本, 并将选中的子图及其真值信息保存到硬盘.
        @param line train.jsonl或val.jsonl中的一行, 对应于样本集中一张图片的全部真值信息的描述
        @param write_images bool类型, 如果为False则不会将生成的样本和真值信息写入到硬盘
        """
        anno = json.loads(line.strip())
        image_id = anno['image_id'] # 不带后缀名.jpg的文件名
        all = []
        for char in anno_tools.each_char(anno):
            if not char['is_chinese']:
                continue
            cate_id = text2cate[char['text']]
            if cate_id >= settings.NUM_CHAR_CATES: # NUM_CHAR_CATES = 1000
                cate_id = settings.NUM_CHAR_CATES
            all.append((char['adjusted_bbox'], cate_id)) # bbox在原始样本图中的坐标, 格式是[左上横坐标, 左上纵坐标, 宽度, 高度]
        if write_images:
            image = cv2.imread(os.path.join(settings.TRAINVAL_IMAGE_DIR, anno['file_name']))
            assert image.shape == imshape
            for o in anno['ignore']:
                poly = (np.array(o['polygon'])).astype(np.int32)
                # cv2.fillConvexPoly()函数可以实现填充任意凸多边形
                # 第二个参数是填充的多边形的顶点坐标的序列
                cv2.fillConvexPoly(image, poly, (128, 128, 128)) # 在image上填充多边形poly, 填充颜色是(128, 128, 128)
        cropped_list = list()

        # 计算每个裁剪图的真值bbox, 生成真值信息文件
        # 真值判定原则: 当且仅当原始图上的一个真值bbox完全落入某裁剪图内时, 才能将该bbox计入裁剪图的真值bbox
        for o in darknet_tools.get_crop_bboxes(imshape, cropshape, cropoverlap):
            xlo = o['xlo'] # 裁剪图的左侧横坐标?
            xhi = xlo + cropshape[1] # 裁剪图的右侧横坐标?
            ylo = o['ylo']
            yhi = ylo + cropshape[0]
            labels = []
            for bbox, cate_id in all:
                x, y, w, h = bbox # bbox在原始样本图中的坐标, 格式是[左上横坐标, 左上纵坐标, 宽度, 高度]

                # 如果真值bbox与当前裁剪图无交集, 则该bbox一定不会计入当前裁剪图的真值集合
                if x > xhi or x + w < xlo or y > yhi or y + h < ylo: # 根据真值bbox左上角点坐标判断其余裁剪图是否有交集
                    continue

                # 真值bbox表示进行转换, 具体包括几个方面: 
                # (1)原始图坐标 -> 裁剪图坐标
                # (2)"左上角+宽长"表示 -> "中心点 + 宽长"表示(因为darknet要求)
                # (3)绝对坐标转化为相对(边长的)坐标(因为darknet要求)
                bbox = ((x + w / 2 - xlo) / cropshape[1], (y + h / 2 - ylo) / cropshape[0], w / cropshape[1], h / cropshape[0])

                # 计算真值bbox与当前裁剪图相交部分面积占bbox的比例, 如果比例大于等于50%则将bbox计入当前裁剪图的真值
                if 0.5 < in_image_ratio(bbox):
                    labels.append((bbox, cate_id))

            # 将裁剪图和它的真值信息保存到硬盘. 注意只保存那些至少包含一个真值bbox的裁剪图
            if 0 < len(labels):
                basename = '{}_{}'.format(image_id, o['name']) # o['name'] = "{}_{}".format(i, j), 其中i和j取值都是闭区间[0, 13]
                cropped_file_name = os.path.join(settings.TRAINVAL_CROPPED_DIR, '{}.jpg'.format(basename)) # TRAINVAL_CROPPED_DIR = 'products/trainval'
                cropped_list.append(cropped_file_name)
                if write_images:
                    cropped = image[ylo:yhi, xlo:xhi] # 从原始图上"抠出"目标裁剪图 
                    cv2.imwrite(cropped_file_name, cropped)
                    with open(os.path.join(settings.TRAINVAL_CROPPED_DIR, '{}.txt'.format(basename)), 'w') as f:
                        for bbox, cate_id in labels:
                            f.write('%d %f %f %f %f\n' % ((cate_id, ) + bbox))

        # 返回所有有效(至少包含一个真值bbox)裁剪图的名字列表
        return cropped_list

    q_i = queue.Queue()
    q_i.put(0)

    def foo(*args):
        """线程函数"""
        # 从队列中取出一个任务索引号, 之后将该任务索引号的下一个索引号加入队列
        # 该索引号的值只有计数意义, 并没有业务意义
        i = q_i.get()
        if i % 100 == 0:
            print('crop trainval', i, '/', len(lines))
        q_i.put(i + 1)

        # 根据原始样本信息生成darknet训练样本和真值信息
        crop_once(*args)

    # 生成训练数据集(从原始图上生成裁剪图和真值信息)
    # 采用多线程并发, 考虑到python中多线程效率低下, 这里为什么不使用多进程?
    common_tools.multithreaded(foo, [(line, True) for line in lines], num_thread=4)

    # 生成全部训练样本(图片)路径的集合. 注意: darknet会根据训练训练样本路径自动推断出其真值信息文件路径
    trainset = []
    for i, line in enumerate(lines):
        if i % 1000 == 0:
            print('list trainval', i, '/', len(lines))
        trainset += crop_once(line, False) # 第二个参数置为False, 这时不会读取或写入图片和真值信息, 只是返回生成的训练样本路径集合

    with open(settings.DARKNET_TRAIN_LIST, 'w') as f: # DARKNET_TRAIN_LIST = 'products/trainval.txt'
        for file_name in trainset:
            f.write(file_name)
            f.write('\n')


def main():
    write_darknet_data()  # 生成数据配置文件
    write_darknet_cfg()   # 生成网络配置文件
    write_darknet_names() # 生成类标文件
    assert os.path.isfile(settings.DARKNET_PRETRAIN) and 79327120 == os.path.getsize(settings.DARKNET_PRETRAIN), \
            'please download {} to {}'.format('https://pjreddie.com/media/files/darknet19_448.conv.23', settings.DARKNET_PRETRAIN)
    crop_train_images()   # 生成训练样本集和真值信息文件


if __name__ == '__main__':
    main()
```

### 2.多进程训练机制
