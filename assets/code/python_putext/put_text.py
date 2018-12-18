#coding=utf-8
#!/usr/bin/env python

import os
import cv2
import numpy
import PIL.Image
import PIL.ImageDraw
import PIL.ImageFont

def draw_en():
    """"""
    img = cv2.imread('123.jpg')
    # 照片/添加的文字/左上角坐标/字体/字体大小/颜色/字体粗细
    img_new = cv2.putText(img, "hello", (50, 150), cv2.FONT_HERSHEY_COMPLEX, 0.5, (0, 255, 0), 1)
    b = cv2.imwrite('123_en.jpg', img_new)
    assert b

def draw_ch():
    """"""
    img_cv2 = cv2.imread('123.jpg')
    # 图像从OpenCV格式转换成PIL格式
    img_pil = PIL.Image.fromarray(cv2.cvtColor(img_cv2, cv2.COLOR_BGR2RGB))
 
    # 字体  字体*.ttc的存放路径一般是： /usr/share/fonts/opentype/noto/ 查找指令locate *.ttc
    font = PIL.ImageFont.truetype('simhei.ttf', 20)
 
    draw = PIL.ImageDraw.Draw(img_pil)
    draw.text((100, 100), '在图片上输出中文', font=font, fill=(0, 255, 0))
    # 使用PIL中的save方法保存图片到本地
    # img_PIL.save('02.jpg', 'jpeg')
 
    # 转换回OpenCV格式
    img_cv2 = cv2.cvtColor(numpy.asarray(img_pil), cv2.COLOR_RGB2BGR)
    assert cv2.imwrite('123_ch.jpg', img_cv2)

if __name__ == '__main__':
    draw_ch()
