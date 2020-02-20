#coding=utf-8
#!/usr/bin/env python

"""
1.以下几项操作都有统一的（不区分操作物品）表中记录
评论
关注
收藏

2.以下几项操作只能在与物品相关的表中获得
赞
踩
回复
作者
"""

import os
import pickle
import numpy
import time
import pandas

# "用户-物品"矩阵的元素格式
# [赞, 踩, 关注, 收藏, 评论, 回复, 浏览, 作者, ]
g_element_labels = ['like', 'dislike', 'follow', 'collect', 'comment', 'reply', 'seen', 'author', 'participant']

def gen_single_col_csv(src_pth, dst_pth, src_key_col, dst_key_col, sort_type):
    """读取指定"""
    """读取pkl文件, 生成所有用户id组成的csv文件, 每个用户id占一行"""
    assert sort_type in ('str', 'int')
    with open(src_pth, 'rb') as f:
        dt_data = pickle.load(f)
    assert src_key_col in dt_data['cols']
    idx = dt_data['cols'].index(src_key_col)

    ids = []
    for line in dt_data['data']:
        id = line[idx]
        ids.append(id)

    if sort_type == 'int':
        tmp = [int(x) for x in ids]
        tmp.sort()
        ids = [str(x) for x in tmp]
    else:
        ids.sort()

    # 保存为csv
    #obj = pandas.DataFrame(columns=[dst_key_col], data=ids)
    #obj.to_csv(dst_pth, encoding='utf-8')
    with open(dst_pth, 'w') as f:
        f.write('\n'.join(ids))
        f.write('\n')

def gen_ids_csv():
    """"""
    gen_single_col_csv('data_pkl/USER/3.1_DT_USERS.pkl', 'data_csv/user_ids.csv', 'ID_USER', 'USER_ID', 'str') # id_users
    gen_single_col_csv('data_pkl/QA/3.13_DT_QUESTIONS.pkl', 'data_csv/question_ids.csv', 'QUESTION_ID', 'QUESTION_ID', 'int') # id_questions
    gen_single_col_csv('data_pkl/BLOG/3.49_DT_BLOG_META.pkl', 'data_csv/blog_ids.csv', 'ID_BLOG_META', 'BLOG_ID', 'int') # id_blogs
    gen_single_col_csv('data_pkl/IDEA/3.84_DT_IDEA.pkl', 'data_csv/idea_ids.csv', 'ID_IDEA', 'IDEA_ID', 'int') # id_ideas

def gen_user_item_csv(users, items, u2i, csv_pth):
    """用户物品矩阵csv"""
    with open(csv_pth, 'wb') as f:
        # 写入首行
        f.write(b'USER_ID,')
        for item_id in items:
            f.write(bytes(item_id, encoding='utf-8'))
            f.write(b',')
        f.write(b'\n')

        # 循环写入每一行
        for user_id in users:
            line = []
            line.append(bytes(user_id, encoding='utf-8'))
            if not (user_id in u2i):
               for _ in items:
                   line.append(b'')
            else:
                for item_id in items:
                    if item_id in u2i[user_id]:
                        elem = '|'.join([str(x) for x in u2i[user_id][item_id]]) # u2i矩阵每个元素是一个n维列表, 把列表中的元素
                        line.append(bytes(elem, encoding='utf-8'))
                    else:
                        line.append(b'')
            f.write(b','.join(line))
            f.write(b'\n')

def read_ids_csv(csv_pth):
    """"""
    assert os.path.isfile(csv_pth)
    with open(csv_pth, 'r') as f:
        lines = f.readlines()
    ids = [line.strip('\n') for line in lines]

    # 如果最后csv一行只有一个回车符
    if ids[-1] == '':
        ids = ids[:-1]
    return ids

def gen_user_item_csv_all(id_csv_dir, u2i_pkl_dir, dst_csv_dir):
    """"""
    assert os.path.isdir(id_csv_dir)
    assert os.path.isdir(u2i_pkl_dir)
    if os.path.exists(dst_csv_dir):
        assert os.path.isdir(dst_csv_dir)
    else:
        os.makedirs(dst_csv_dir)

    user_ids_csv = os.path.join(id_csv_dir, 'user_ids.csv')
    question_ids_csv = os.path.join(id_csv_dir, 'question_ids.csv')
    blog_ids_csv = os.path.join(id_csv_dir, 'blog_ids.csv')
    idea_ids_csv = os.path.join(id_csv_dir, 'idea_ids.csv')

    question_u2i_pkl = os.path.join(u2i_pkl_dir, 'QA_u2i.pkl')
    blog_u2i_pkl = os.path.join(u2i_pkl_dir, 'BLOG_u2i.pkl')
    idea_u2i_pkl = os.path.join(u2i_pkl_dir, 'IDEA_u2i.pkl')

    # user_ids
    user_ids = read_ids_csv(user_ids_csv)

    # question_ids 
    question_ids = read_ids_csv(question_ids_csv)

    # blog_ids
    blog_ids = read_ids_csv(blog_ids_csv)

    # idea_ids
    idea_ids = read_ids_csv(idea_ids_csv)

    # question u2i
    with open(question_u2i_pkl, 'rb') as f:
        u2i = pickle.load(f)
    gen_user_item_csv(user_ids, question_ids, u2i, os.path.join(dst_csv_dir, 'user_question.csv'))

    # blog u2i
    with open(blog_u2i_pkl, 'rb') as f:
        u2i = pickle.load(f)
    gen_user_item_csv(user_ids, blog_ids, u2i, os.path.join(dst_csv_dir, 'user_blog.csv'))

    # idea u2i
    with open(idea_u2i_pkl, 'rb') as f:
        u2i = pickle.load(f)
    gen_user_item_csv(user_ids, idea_ids, u2i, os.path.join(dst_csv_dir, 'user_idea.csv'))

if __name__ == '__main__':
    gen_ids_csv()
    gen_user_item_csv_all("data_csv", "data_mat", "u2i_csv")