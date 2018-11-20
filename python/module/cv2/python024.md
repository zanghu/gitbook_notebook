## CV2: putText在图片写字


```python

#coding=utf-8
#!/usr/bin/env python
import os
import cv2

if __name__ == '__main__':
    img = cv2.imread('123.jpg')
    # 照片/添加的文字/左上角坐标/字体/字体大小/颜色/字体粗细（单位：像素）
    # 经过测试，字体大小可以取浮点数（比如下面是0.5）
    img_new = cv2.putText(img, "hello", (50, 150), cv2.FONT_HERSHEY_COMPLEX, 0.5, (0,255, 0), 1, False) 
    b = cv2.imwrite('123_new.jpg', img_new)
    assert b
```







