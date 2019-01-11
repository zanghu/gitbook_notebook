#coding=utf-8
#!/bin/env python

"""
批量将编译后的c++符号转为可理解的明文符号
"""

import os
import cxxfilt

def demangle(src_path, dst_path):
    """
    src_path: 使用 nm -S libxxx.so 导出的动态库符号表的文件路径
    dst_path: demangle后的符号表保存路径
    """
    os.path.isfile(src_path)
    lines = []
    with open(src_path, "rb") as f:
        lines = f.readlines()
    names = []
    for line in lines:
        #print(line)
        parts = str(line, 'utf-8').rstrip("\r\n").split(' ')
        if len(parts) == 4:
            name = cxxfilt.demangle(parts[3])
            names.append(name)
    with open("dst_path", "wb") as f:
        for name in names:
            f.write(bytes(name, 'utf-8'))
            f.write(bytes('\n', 'utf-8'))

if __name__ == '__main__':
    demangle('/home/zanghu/record', 'symbol.txt')