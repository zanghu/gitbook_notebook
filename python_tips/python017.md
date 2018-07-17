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



xlrd主要是用来读取excel文件

```python
import xlrd

workbook = xlrd.open_workbook(u'有趣装逼每日数据及趋势.xls')
sheet_names= workbook.sheet_names()

for sheet_name in sheet_names:
　　 sheet2 = workbook.sheet_by_name(sheet_name)
　　 print sheet_name rows = sheet2.row_values(3) # 获取第四行内容
　　 cols = sheet2.col_values(1) # 获取第二列内容
　　 print rows
　　 print cols
```python
 

### 2.写excel文件

xlwt主要是用来写excel文件

```python
import xlwt

wbk = xlwt.Workbook()
sheet = wbk.add_sheet('sheet 1')
sheet.write(0,1,'test text')#第0行第一列写入内容
wbk.save('test.xls')
```
 

### 3.修改excel文件目的

xlutils结合xlrd可以达到修改excel文件目的

```python
import xlrd
from xlutils.copy import copy

workbook = xlrd.open_workbook(u'有趣装逼每日数据及趋势.xls')
workbooknew = copy(workbook)
ws = workbooknew.get_sheet(0)
ws.write(3, 0, 'changed!')
workbooknew.save(u'有趣装逼每日数据及趋势copy.xls')
```

4.对excel文件进行读写操作

openpyxl可以对excel文件进行读写操作

```python
from openpyxl import Workbook
from openpyxl import load_workbook
from openpyxl.writer.excel import ExcelWriter 

workbook_ = load_workbook(u"新歌检索失败1477881109469.xlsx")
sheetnames =workbook_.get_sheet_names() #获得表单名字
print sheetnames
sheet = workbook_.get_sheet_by_name(sheetnames[0])
print sheet.cell(row=3,column=3).value
sheet['A1'] = '47' 
workbook_.save(u"新歌检索失败1477881109469_new.xlsx")  
wb = Workbook()
ws = wb.active
ws['A1'] = 4
wb.save("新歌检索失败.xlsx") 
```
     
5.x写excel文件并加上图表

xlsxwriter可以写excel文件并加上图表

```python
import xlsxwriter

def get_chart(series):
    chart = workbook.add_chart({'type': 'line'})
    for ses in series:
        name = ses["name"]
        values = ses["values"]
        chart.add_series({ 
            'name': name,
            'categories': 'A2:A10',
            'values':values
        })  
    chart.set_size({'width': 700, 'height': 350}) 

    return chart

 if __name__ == '__main__':

    workbook = xlsxwriter.Workbook(u'H5应用中心关键数据及趋势.xlsx') 
    worksheet = workbook.add_worksheet(u"每日PV,UV")
    headings = ['日期', '平均值']
    worksheet.write_row('A1', headings)

    index=0
    for row in range(1,10):
        for com in [0,1]:
            worksheet.write(row,com,index)
            index+=1  

    series = [{"name":"平均值","values":"B2:B10"}]
    chart = get_chart(series)
    chart.set_title ({'name': '每日页面分享数据'})  
    worksheet.insert_chart('H7', chart)
    workbook.close() 
```