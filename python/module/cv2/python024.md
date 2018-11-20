## CV2: putText在图片写字

参考资料：[opencv 绘制不同图形并添加文字](https://www.jianshu.com/p/e99ede5103ed)

### 1.函数原型和代码示例

#### 1.1 函数原型

```python
void cv::putText(
    InputOutputArray    img,
    const String &  text,
    Point   org,
    int     fontFace,
    double  fontScale,
    Scalar  color,
    int     thickness = 1,
    int     lineType = LINE_8,
    bool    bottomLeftOrigin = false 
)       
Python:
img =cv.putText(img, text, org, fontFace,fontScale, color[, thickness[, lineType[, bottomLeftOrigin]]])
```

#### 1.2 代码示例

图片显示英文文字

```python
#coding=utf-8
#!/usr/bin/env python
import os
import cv2

if __name__ == '__main__':
    img = cv2.imread('123.jpg')
    # 照片/添加的文字/左上角坐标/字体/字体大小/颜色/字体粗细（单位：像素）/是否改变默认起点
    # 最后一个布尔参数为False时起点坐标是左上角，为True时起点坐标是左下角
    # 经过测试，字体大小可以取浮点数（比如下面是0.5）
    img_new = cv2.putText(img, "hello", (50, 150), cv2.FONT_HERSHEY_COMPLEX, 0.5, (0,255, 0), 1, False) 
    b = cv2.imwrite('123_new.jpg', img_new)
    assert b
```

#### 1.3 关于字体

由于OpenCV原生函数putText是不支持中文字体，所以这里无法写入中文。

关于文字字体名称标识符，参见 Hershey 字体集 ，可供字体类型如下：

| 字体类型 | 说明 |
| :---: | --- |
| FONT_HERSHEY_SIMPLEX | 正常大小无衬线字体 |
| FONT_HERSHEY_PLAIN | 小号无衬线字体 |
| FONT_HERSHEY_DUPLEX | 正常大小无衬线字体，比FONT_HERSHEY_SIMPLEX更复杂 |
| FONT_HERSHEY_COMPLEX | 正常大小有衬线字体 |
| FONT_HERSHEY_TRIPLEX | 正常大小有衬线字体，比FONT_HERSHEY_COMPLEX更复杂
| FONT_HERSHEY_COMPLEX_SMALL | 同FONT_HERSHEY_COMPLEX |
| FONT_HERSHEY_SCRIPT_SIMPLEX | 手写风格字体 |
| FONT_HERSHEY_SCRIPT_COMPLEX | 比FONT_HERSHEY_SCRIPT_SIMPLEX 更复杂 |

以上只是针对sanserif字体进行的设置。

### 2.写中文

由于OpenCV原生函数putText是不支持中文字体，所以这里无法写入中文。

实现使用opencv在图片写中文有两个思路：`freetype`或者`PIL`。

#### 2.1 opencv + freetype

* **安装`freetype`模块**

```shell
$ pip install freetype-py
```

* **代码示例**

略

#### 2.2 opencv + PIL

从目前查到的资料看，这种方法代码写起来比基于`freetype`的方案更简单。

* **代码示例**

```python
# -*- coding: utf-8 -*-
import cv2
import numpy
from PIL import Image, ImageDraw, ImageFont
 
if __name__ == '__main__':
 
    img_OpenCV = cv2.imread('01.jpg')
    # 图像从OpenCV格式转换成PIL格式
    img_PIL = Image.fromarray(cv2.cvtColor(img_OpenCV, cv2.COLOR_BGR2RGB))
 
    # 字体  字体*.ttc的存放路径一般是： /usr/share/fonts/opentype/noto/ 查找指令locate *.ttc
    font = ImageFont.truetype('NotoSansCJK-Black.ttc', 40)
    # 字体颜色
    fillColor = (255,0,0)
    # 文字输出位置
    position = (100,100)
    # 输出内容
    str = '在图片上输出中文'
 
    # 需要先把输出的中文字符转换成Unicode编码形式
    if not isinstance(str, unicode):
        str = str.decode('utf8')
 
    draw = ImageDraw.Draw(img_PIL)
    draw.text(position, str, font=font, fill=fillColor)
    # 使用PIL中的save方法保存图片到本地
    # img_PIL.save('02.jpg', 'jpeg')
 
    # 转换回OpenCV格式
    img_OpenCV = cv2.cvtColor(numpy.asarray(img_PIL),cv2.COLOR_RGB2BGR)
    cv2.imshow("print chinese to image",img_OpenCV)
    cv2.waitKey()
    cv2.imwrite('03.jpg',img_OpenCV)
```









