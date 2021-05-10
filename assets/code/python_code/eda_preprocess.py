# coding=utf-8
#!/usr/bin/env python
import os
import sys

def show_tags(pth, sep=','):
    """读取文件的第一行，给出列标签和列数"""
    if not os.path.isfile(pth):
        print('Error: {} is not a file, plz check again.'.format(pth), file=sys.stderr)
        sys.exit()
    with open(pth, 'r', encoding='utf-8') as f:
        line = f.readline()
    tags = line.rstrip('\r\n').split(sep)
    for i, tag in enumerate(tags):
        print('{}: {}'.format(i, tag))
    print('total tags num: {}'.format(len(tags)))

if __name__ == '__main__':
    #print('{}'.format(len(sys.argv)))
    if len(sys.argv) < 2:
        print('Error: file name not found.', file=sys.stderr)
        sys.exit()
    elif len(sys.argv) == 2:
        show_tags(sys.argv[1])
    else:
        show_tags(sys.argv[1], sys.argv[2])