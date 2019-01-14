#coding=utf-8
#!/bin/env python

import os
import base64
import socket
import numpy
import time

m_serv_ip = '10.230.147.31'
m_serv_port = 9999

def init_socket(serv_ip, serv_port):
    """"""
    ip_port = (serv_ip, serv_port)
    sk = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0) # TCP
    #sk = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0) # UDP
    sk.connect(ip_port)
    sk.settimeout(50)
    return sk

def send_socket(sk, b64, name):
    """"""
    len_content = len(b64) + len(name)
    sk.sendall(bytes(str(len_content).zfill(16), encoding='utf-8')) # 发送头部
    sk.sendall(bytes(name, encoding='utf-8'))
    #sk.sendall(str(len_content).zfill(16)) # 发送头部
    #sk.sendall(name)
    sk.sendall(b64) # 发送内容
    sk.close()


def img_to_b64(img_path):
    """显示一副图片"""
    assert os.path.isfile(img_path)
    with open(img_path, 'rb') as f:
        img = f.read()
    b64 = base64.b64encode(img)
    return b64

def get_img_names(img_dir):
    """"""
    assert os.path.isdir(img_dir)
    names_all = os.listdir(img_dir)
    names = [name for name in names_all if name.endswith('.jpg')]
    print('目录 {0} 下文件总数: {1}, 图片总数: {2}'.format(img_dir, len(names_all), len(names))) 
    return names
    
def send_batch(img_dir, img_names, start_idx, batch_num=10):
    """显示指定目录下的所有图片"""
    global m_serv_ip
    global m_serv_port
    t0 = time.clock()
    t1 = time.clock()
    for cnt, img_name in enumerate(img_names[start_idx: start_idx + batch_num]):
        img_path = os.path.join(img_dir, img_name)
        b64 = img_to_b64(img_path) # 获得b64编码
        sk = init_socket(m_serv_ip, m_serv_port)
        send_socket(sk, b64, img_name.rstrip('.jpg')) # 发送数据
        t2 = time.clock()
        print('cnt {0} finish, time elapsed: {1}, total elapsed: {2}'.format(cnt, t2 - t1, t2 - t0))
        t1 = t2
    
    print('all finished, num send: {0}, time elapsed: {1}'.format(len(img_names), time.clock() - t0))
    #sk.close()
    
def client(img_dir, batch_size, max_batch):
    """"""
    assert os.path.isdir(img_dir)
    t0 = time.clock()
    img_names = get_img_names(img_dir)
    num_img = len(img_names)
    num_finish = 0 # 已完成个数
    start_idx = 0 # batch起始索引号
    num_batch = 0
    while num_finish < num_img:
        max_num = 0
        num_left = num_img - num_finish
        if num_left < batch_size:
            max_num = num_left
        else:
            max_num = batch_size
        send_batch(img_dir, img_names, start_idx, max_num)
        start_idx += max_num
        num_finish += max_num
        num_batch += 1
        if num_batch >= max_batch:
            break
    print('client finish, time elapsed: {0}'.format(time.clock() - t0))

if __name__ == '__main__':
    client('../data/problem3/train', batch_size=20, max_batch=10000)