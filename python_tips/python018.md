## Python: 操作excel表格——基本读写操作

参考资料: [python-pptx模块官方文档](http://python-pptx.readthedocs.io/en/latest/index.html)
参考资料：[使用python生成pptx](https://blog.csdn.net/huuinn/article/details/78965725)

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


### 2.官方支持

[python-pptx模块官方文档](http://python-pptx.readthedocs.io/en/latest/index.html)

[项目的github主页](https://github.com/scanny/python-pptx)

