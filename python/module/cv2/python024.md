## CV2: putText在图片写字

### 1.代码示例

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

由于OpenCV原生函数putText是不支持中文字体，所以这里无法写入中文。
关于文字字体名称标识符，参见 Hershey 字体集 ，可供字体类型如下：
FONT_HERSHEY_SIMPLEX 正常大小无衬线字体
FONT_HERSHEY_PLAIN 小号无衬线字体
FONT_HERSHEY_DUPLEX 正常大小无衬线字体，比FONT_HERSHEY_SIMPLEX更复杂
FONT_HERSHEY_COMPLEX 正常大小有衬线字体
FONT_HERSHEY_TRIPLEX 正常大小有衬线字体，比FONT_HERSHEY_COMPLEX更复杂
FONT_HERSHEY_COMPLEX_SMALL 同FONT_HERSHEY_COMPLEX
FONT_HERSHEY_SCRIPT_SIMPLEX 手写风格字体
FONT_HERSHEY_SCRIPT_COMPLEX 比FONT_HERSHEY_SCRIPT_SIMPLEX 更复杂
以上只是针对sanserif字体进行的设置。

作者：刑素素
链接：https://www.jianshu.com/p/e99ede5103ed
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。







