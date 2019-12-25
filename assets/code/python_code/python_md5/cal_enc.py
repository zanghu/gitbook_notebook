#coding=utf-8
#!/usr/bin/env python

"""
用法:
    python <当前脚本名> [校验算法类型] 
"""

import hashlib
import os
import time
import sys
 
def cal_enc_large(pth, enc_type='md5'):
    """计算大文件的校验值，支持md5和sha256"""
    assert os.path.isfile(pth)
    assert enc_type in ['md5', 'sha256']
    if enc_type == 'md5':
        print('enc_type is: MD5')
        enc = hashlib.md5()
    else: #enc_type == 'sha256':
        print('enc_type is: SHA256')
        enc = hashlib.sha256()
    #else:
    #    raise ValueError("Input param enc_type must be 'md5' or 'sha256', current value: '{}' is not valid, error.".format(enc_type))
    t0 = time.time()
    t1 = t0
    with open(pth,'rb') as f:
        cnt = 0
        while True:
            data = f.read(4 * 1024 * 1024)
            if not data:
                break
            enc.update(data) #update添加时会进行计算
            cnt += 1;
            if cnt % 100 == 0:
                t2 = time.time()
                print('finish {}, time delta: {}s, time elapsed: {}s'.format(cnt, t2 - t1, t2 - t0))
                t1 = t2
        print('all finished, cnt = {}'.format(cnt))
    print(enc.hexdigest()) #打印结果

if __name__ == '__main__':
    # 第一个参数时脚本文件路径
    if len(sys.argv) == 1:
        enc_type = 'md5'
        print('enc_type not defined, use default: MD5')
    else:
        _, enc_type = sys.argv
    #print('{}'.format(g_enc_type))
    cal_enc_large('../../soft_setup/ubuntu-18.04.3-desktop-amd64.iso', enc_type)