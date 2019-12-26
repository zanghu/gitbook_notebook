# coding=utf-8
#!/usr/bin/python env

import os
import xlrd

def read_xlsx(src_pth):
    """演示读取*.xlsx文档"""
    assert os.path.isfile(src_pth)
    # 打开xlsx文件
    with xlrd.open_workbook(src_pth) as table:
        # 随机访问sheet页
        sheets = table.sheets() # table.sheets返回包含所有sheet的列表

        sheet = sheets[0] # 选择第一个sheet

        #sheet页的行数和列数
        n_cols = sheet.ncols
        n_rows = sheet.nrows

        #逐行遍历sheet的每个元素
        for i in range(n_rows):
            for j in range(n_cols):
                cell_ij = sheet.cell(i, j) # 获得(i, j)位置的单元格对象
                print('elem[{}][{}] = {}'.format(i, j, cell_ij.value)) # 通过单元格对象的value属性获得单元格值

    print('finish')

if __name__ == '__main__':
    read_xlsx('xlrd_test.xlsx')