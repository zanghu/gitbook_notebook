## Python: 操作excel表格——基本读写操作

参考资料: [python操作Excel的几种方式](https://www.cnblogs.com/lingwang3/p/6416023.html)


### 1.全局设置

* 编码设置

```python
wbk = xlwt.Workbook(encoding='utf-8') # 设置编码后, 当excel内有中文时，保存不会报错
```

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



### 3.边框设置

边框通过`xlwt.Borders`对象进行设置，一般方法如下：

```python
borders = xlwt.Borders()

... # 设置Borders对象

style = xlwt.XFStyle()
style.borders = borders 

```

* 设置实线边框

```python
borders.left = xlwt.Borders.THIN
borders.right = xlwt.Borders.THIN
borders.top = xlwt.Borders.THIN
borders.bottom = xlwt.Borders.THIN
```

### 4.对齐调整

对齐相关设置在`xlwt.Alignment`对象中进行设置，使用的一般方法如下：

```python
# 创建Alignment对象
alignment = xlwt.Alignment()

... # 设置Alignment对象

# Alignment对象加入Style
style = xlwt.XFStyle()
style.alignment = alignment # alignment加入style
```

* 对齐方式

```python
# 水平方向
alignment.horz = xlwt.Alignment.HORZ_CENTER # 水平居中 0x02
alignment.horz= xlwt.Alignment.HORZ_LEFT # 左端对齐 0x01
alignment.horz= xlwt.Alignment.HORZ_RIGHT # 右端对齐 0x03

# 垂直方向
alignment.vert = xlwt.Alignment.VERT_CENTER # 垂直居中 0x01
alignment.vert = xlwt.Alignment.VERT_TOP # 顶端对齐, 0x00
alignment.vert = xlwt.Alignment.VERT_BOTTOM # 低端对齐 0x02
```

* 自动换行

```python
alignment.wrap = xlwt.Alignment.WRAP_AT_RIGHT # 自动换行
```

### 5.字体调整

参考资料：[python xlwt写excel格式控制](https://blog.csdn.net/kk123a/article/details/49813559)
参考资料：[Python xlwt设置excel单元格字体及格式](https://blog.csdn.net/u013400654/article/details/50284983)

字体的所有设置应该通过`xlwt.Font`对象设置，一般步骤如下：

```python
# 创建excel文件对象
workbook = xlwt.Workbook(encoding = 'ascii')
worksheet = workbook.add_sheet('My Worksheet') # 创建sheet

# 创建Font对象
font = xlwt.Font() # Create the Font
... # 设置Font对象

# Font对象加入Style
style = xlwt.XFStyle() # Create the Style
style.font = font # Apply the Font to the Style

# 按照指定Style书写单元格内容
worksheet.write(1, 0, label = 'Formatted value', style) # Apply the Style to the Cell
```

* 字体加粗

```python
font.bold = True
```

* 文字加下划线

```python
font.underline = True
```

* 文字加斜体

```python
font.italic = True
```

* 设置字体

参考资料：[Python xlwt设置excel单元格字体及格式](https://blog.csdn.net/u013400654/article/details/50284983)

```python
font.name = 'Times New Roman'
fnt.name = u'微软雅黑' # 设置其字体为微软雅黑
```

* 字体颜色

```python
# 0 = Black, 1 = White, 2 = Red, 3 = Green, 4 = Blue, 5 = Yellow, 6 = Magenta,  the list goes on...
fnt.colour_index = i # 设置其字体颜色
```

```python
fnt.bold = True                                             
style.font = fnt                    #将赋值好的模式参数导入Style                                   
sheet.write_merge(i,i,3,5,Line_data,style)  #以合并单元格形式写入数据，即将数据写入以第4/5/6列合并德单元
```

#### 7.单元格模式设置

通过设置`xlwt.Pattern`对象实现， 一般过程如下

```python
pattern = xlwt.Pattern()

... 

style.pattern = pattern
```

* 单元格背景色

参考资料：[Python xlwt如何设置单元格的自定义背景颜色](https://cloud.tencent.com/developer/ask/34882)

```python
pattern.pattern = xlwt.Pattern.SOLID_PATTERN
pattern.pattern_fore_colour = xlwt.Style.colour_map['dark_purple']
```





