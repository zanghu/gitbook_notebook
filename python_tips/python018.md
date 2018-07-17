## Python: 操作excel表格——基本读写操作

参考资料: [python-pptx模块官方文档](http://python-pptx.readthedocs.io/en/latest/index.html)


### 1.简单例子

* **文字输出到pptx**

```python
from pptx import Presentation
 
# 创建幻灯片 ------
prs = Presentation()
title_slide_layout = prs.slide_layouts[0]
slide = prs.slides.add_slide(title_slide_layout)
 
title = slide.shapes.title
subtitle = slide.placeholders[1]
 
# 设置标题和副标题
title.text = "Hello, World!"
subtitle.text = "pip install python-pptx"
 
prs.save("test.pptx")
```

效果图：

![](/assets/python018_001.PNG)

* ***绘制柱状图**

```python
from pptx import Presentation
from pptx.chart.data import ChartData
from pptx.enum.chart import XL_CHART_TYPE
from pptx.util import Inches
 
# 创建幻灯片 ------
prs = Presentation()
slide = prs.slides.add_slide(prs.slide_layouts[5])
 
# 定义图表数据 ---------------------
chart_data = ChartData()
chart_data.categories = ['East', 'West', 'Midwest']
chart_data.add_series('Series 1', (19.2, 21.4, 16.7))
 
# 将图表添加到幻灯片 --------------------
x, y, cx, cy = Inches(2), Inches(2), Inches(6), Inches(4.5)
slide.shapes.add_chart(
    XL_CHART_TYPE.COLUMN_CLUSTERED, x, y, cx, cy, chart_data
)
 
prs.save('chart-01.pptx')
```

效果图：

![](/assets/python018_002.PNG)


### 2.单元格调整

* 合并单元格

```python
sheet = wbk.add_sheet('sheet1')
sheet.write_merge(row_start, row_end, col_start, col_end, content, [style]) # 向圈出的区域内写入内容，实际效果等同于合并圈出区域的单元格
```

* 文字对齐

参考资料：[python导出excel常用格式样式设置](https://blog.csdn.net/qq_32502511/article/details/53463743)

```python
# 创建Alignment对象
alignment = xlwt.Alignment()
alignment.horz = xlwt.Alignment.HORZ_CENTER # 水平对齐
alignment.vert = xlwt.Alignment.VERT_CENTER # 垂直对齐

# 创建Style对象
style = xlwt.XFStyle()
style.alignment = alignment # alignment加入style

sheet.write(..., style=style) # 写入时使用指定对齐模式
```

* 单元格尺寸

sheet.col(1).width = 256 * 4
sheet.col(1).width = 256 * 20

* 单元格边框

```python
borders = xlwt.Borders()
borders.left = 1
borders.right = 1
borders.top = 1
borders.bottom = 1
borders.bottom_colour=0x3A    
 
style = xlwt.XFStyle()
style.borders = borders 
 
sheet.write(0, 0, 'Firstname',style)
```

* 单元格背景色

参考资料：[Python xlwt如何设置单元格的自定义背景颜色](https://cloud.tencent.com/developer/ask/34882)

### 3.字体调整

参考资料：[python xlwt写excel格式控制](https://blog.csdn.net/kk123a/article/details/49813559)

* 字体加粗

```python
import xlwt
workbook = xlwt.Workbook(encoding = 'ascii')
worksheet = workbook.add_sheet('My Worksheet')
font = xlwt.Font() # Create the Font
font.name = 'Times New Roman'
font.bold = True
font.underline = True
font.italic = True
style = xlwt.XFStyle() # Create the Style
style.font = font # Apply the Font to the Style
worksheet.write(0, 0, label = 'Unformatted value')
worksheet.write(1, 0, label = 'Formatted value', style) # Apply the Style to the Cell
workbook.save('Excel_Workbook.xls')
```

* 字体类型

参考资料：[Python xlwt设置excel单元格字体及格式](https://blog.csdn.net/u013400654/article/details/50284983)

```python
fnt = xlwt.Font()                        # 创建一个文本格式，包括字体、字号和颜色样式特性                              
fnt.name = u'微软雅黑'                # 设置其字体为微软雅黑                                 
fnt.colour_index = i                # 设置其字体颜色                                    
fnt.bold = True                                             
style.font = fnt                    #将赋值好的模式参数导入Style                                   
sheet.write_merge(i,i,3,5,Line_data,style)  #以合并单元格形式写入数据，即将数据写入以第4/5/6列合并德单元
```

