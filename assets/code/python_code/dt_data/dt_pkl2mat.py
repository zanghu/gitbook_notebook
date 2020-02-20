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

# "用户-物品"矩阵的元素格式
# [赞, 踩, 关注, 收藏, 评论, 回复, 浏览, 作者]
g_element_labels = ['like', 'dislike', 'follow', 'collect', 'comment', 'reply', 'seen', 'author', 'participant']

def gen_main_keys(dt_data, key):
    """根据读取到的pkl对象, 返回主键组成的集合"""
    key_idx = dt_data['cols'].index(key)
    keys = set()
    for line in dt_data['data']:
        assert (line[key_idx] in keys) is False
        keys.add(line[key_idx])
    return keys

def gen_col_vals(dt_data, col_name):
    """根据读取到的pkl对象, 返回该列值组成的列表"""
    col_idx = dt_data['cols'].index(col_name)
    vals = []
    for line in dt_data['data']:
        vals.append(line[col_idx])
    return vals

def gen_kv_dict(dt_data, key, col_names):
    """根据读取到的pkl对象, 返回主键到目标列值组成的字典. 可以同时指定多个列"""
    assert isinstance(col_names, (list, tuple, str))

    res = {}
    if isinstance(col_names, str): # 值只有一列
        key_idx = dt_data['cols'].index(key)
        col_idx =  dt_data['cols'].index(col_names)

        for line in dt_data['data']:
            key = line[key_idx]
            assert (key in res) is False
            res[key] = line[col_idx]
    else: # 值有多列
        assert len(col_names) > 0
        key_idx = dt_data['cols'].index(key)
        col_idxs = []
        for col_name in col_names:
            col_idxs.append(dt_data['cols'].index(col_name))

        for line in dt_data['data']:
            key = line[key_idx]
            assert (key in res) is False
            res[key] = []
            for col_idx in col_idxs:
                res[key] = [line[col_idx]]

    return res

def none2val():
    """"""
    return 1

def intchar2intnum(c):
    """整数字符0-9转换为整数值"""
    return ord(c) - ord('0')

def insert_kv_into_mat(mat, dt_data, name_row, name_col, name_val, elem_type, val2elem):
    """
    将pkl对象dt_data['data']中的某列数据插入"用户物品矩阵"mat中
    mat格式: {row: {col:[val]}}
    elem_type: 用户-物品间关系的类型: 赞, 踩, 关注, 收藏, 评论, 回复, 浏览, 作者，决定了插入到元素向量中的位置
    val2elem: 将dt_data中的值转换为mat元素向量对应位置的值的转换函数
    """
    global g_element_labels
    assert name_row in dt_data['cols']
    assert name_col in dt_data['cols']
    if name_val is not None:
        assert name_val in dt_data['cols']
    idx_row = dt_data['cols'].index(name_row)
    idx_col = dt_data['cols'].index(name_col)
    # 考虑name_val为None是因为某些情况下只需要在表中出现内容，不需要对应值, 例如作者身份
    idx_val = None
    if name_val is not None:
        idx_val = dt_data['cols'].index(name_val)
    idx_elem = g_element_labels.index(elem_type)

    num_elem = 0
    for line in dt_data['data']:
        key_row = line[idx_row]
        key_col = line[idx_col]
        if idx_val is None:
            key_val = None
        else:
            key_val = line[idx_val]

        # 数据插入
        if key_val is not None:
            delta = val2elem(key_val)
        else:
            delta = val2elem()
        if delta == 0: # 不会产生影响, 直接结束
            continue

        if key_row in mat:
            if not (key_col in mat[key_row]):
                num_elem += 1
                mat[key_row][key_col] = [0 for i in range(len(g_element_labels))]
            mat[key_row][key_col][idx_elem] += delta
        else:
            num_elem += 1
            elem = [0 for i in range(len(g_element_labels))]
            elem[idx_elem] += delta
            mat[key_row] = {key_col: elem}
    return num_elem
        

def gen_blog_mat_from_obj(dt_users, dt_blogs, dt_blog_comment, dt_blog_reply, dt_blog_inter, dt_blog_comment_inter):
    """从相关pkl文件中抽取信息，返回Blog类文档的'用户-文档矩阵'，为CF做准备"""
    # 1.dt_q_inter: 解析用户问题评价
    b2u = {} # question被user评价过的记录
    u2b = {} # user评价question的记录

    n_elems_b2u = 0
    n_elems_u2b = 0

    # 2.1 作者计数
    n_elems_b2u += insert_kv_into_mat(b2u, dt_blogs, 'ID_BLOG_META', 'ID_USER', None, 'author', none2val)
    n_elems_u2b += insert_kv_into_mat(u2b, dt_blogs, 'ID_USER', 'ID_BLOG_META', None, 'author', none2val)
   # print('2.1 n_elems_b2u = {}, n_elems_u2b = {}'.format(n_elems_b2u, n_elems_u2b))

    # 2.2 评论计数
    n_elems_b2u += insert_kv_into_mat(b2u, dt_blog_comment, "ID_BLOG_META", "ID_FROM_USER", None, "comment", none2val)
    n_elems_u2b += insert_kv_into_mat(u2b, dt_blog_comment, "ID_FROM_USER", "ID_BLOG_META", None, "comment", none2val)
    #print('2.2 n_elems_b2u = {}, n_elems_u2b = {}'.format(n_elems_b2u, n_elems_u2b))

    # 2.3 回复计数
    n_elems_b2u += insert_kv_into_mat(b2u, dt_blog_reply, "ID_BLOG_META", "ID_FROM_USER", None, "reply", none2val)
    n_elems_u2b += insert_kv_into_mat(u2b, dt_blog_reply, "ID_FROM_USER", "ID_BLOG_META", None, "reply", none2val)
    #print('2.3 n_elems_b2u = {}, n_elems_u2b = {}'.format(n_elems_b2u, n_elems_u2b))

    # 2.4 博文赞、踩、收藏计数
    n_elems_b2u += insert_kv_into_mat(b2u, dt_blog_inter, "ID_BLOG_META", "ID_USER", "LIKE_STATE", "like", intchar2intnum)
    n_elems_b2u += insert_kv_into_mat(b2u, dt_blog_inter, "ID_BLOG_META", "ID_USER", "DISLIKE_STATE", "dislike", intchar2intnum)
    n_elems_b2u += insert_kv_into_mat(b2u, dt_blog_inter, "ID_BLOG_META", "ID_USER", "COLLECT_STATE", "collect", intchar2intnum)
    n_elems_u2b += insert_kv_into_mat(u2b, dt_blog_inter, "ID_USER", "ID_BLOG_META", "LIKE_STATE", "like", intchar2intnum)
    n_elems_u2b += insert_kv_into_mat(u2b, dt_blog_inter, "ID_USER", "ID_BLOG_META", "DISLIKE_STATE", "dislike", intchar2intnum)
    n_elems_u2b += insert_kv_into_mat(u2b, dt_blog_inter, "ID_USER", "ID_BLOG_META", "COLLECT_STATE", "collect", intchar2intnum)
    #print('2.4 n_elems_b2u = {}, n_elems_u2b = {}'.format(n_elems_b2u, n_elems_u2b))

    # 2.5 博文评论赞、踩计数
    n_elems_b2u += insert_kv_into_mat(b2u, dt_blog_comment_inter, "ID_BLOG_META", "ID_FROM_USER", "LIKE_STATE", "like", intchar2intnum)
    n_elems_b2u += insert_kv_into_mat(b2u, dt_blog_comment_inter, "ID_BLOG_META", "ID_FROM_USER", "DISLIKE_STATE", "dislike", intchar2intnum)
    n_elems_u2b += insert_kv_into_mat(u2b, dt_blog_comment_inter, "ID_FROM_USER", "ID_BLOG_META", "LIKE_STATE", "like", intchar2intnum)
    n_elems_u2b += insert_kv_into_mat(u2b, dt_blog_comment_inter, "ID_FROM_USER", "ID_BLOG_META", "DISLIKE_STATE", "dislike", intchar2intnum)

    print('2.5, n_elems_b2u = {}, n_elems_u2b = {}'.format(n_elems_b2u, n_elems_u2b))
    assert n_elems_b2u == n_elems_u2b

    n_users = len(u2b)
    n_blogs = len(b2u)
    print('finish BLOG, n_users: {}, n_blogs: {}'.format(n_users, n_blogs))
    print('BLOG sparsity: {}'.format(n_elems_b2u / (n_users * n_blogs))) # 只考虑参与过blog互动的用户的矩阵稀疏度

    n_users_all = len(dt_users['data']) - 1 # 目前谛听所有用户
    print('BLOG sparsity all: {}'.format(n_elems_b2u / (n_users_all * n_blogs))) # 考虑全部用户的矩阵稀疏度
    return [b2u, u2b]

def gen_qa_mat_from_obj(dt_users, dt_questions, dt_answers, dt_q_inter, dt_a_inter, dt_comments):
    """
    从相关pkl文件中抽取信息，返回QA类文档的'用户-文档矩阵'，为CF做准备
    q2u和u2q矩阵中每个元素都是一个N元组[like, dislike, follow, collect, comment, auth]
    """
    # 1.dt_q_inter: 解析用户问题评价
    q2u = {} # question被user评价过的记录
    u2q = {} # user评价question的记录

    n_elems_q2u = 0
    n_elems_u2q = 0

    # 2.1 问题作者计数
    n_elems_q2u += insert_kv_into_mat(q2u, dt_questions, "QUESTION_ID", "USER_ID", None, 'author', none2val)
    n_elems_u2q += insert_kv_into_mat(u2q, dt_questions, "USER_ID", "QUESTION_ID", None, 'author', none2val)

    # 2.2 回答作者计数
    n_elems_q2u += insert_kv_into_mat(q2u, dt_answers, "QUESTION_ID", "USER_ID", None, 'participant', none2val)
    n_elems_u2q += insert_kv_into_mat(u2q, dt_answers, "USER_ID", "QUESTION_ID", None, 'participant', none2val)

    # 2.3 问题赞、踩、关注计数
    n_elems_q2u += insert_kv_into_mat(q2u, dt_q_inter, "QUESTION_ID", "USER_ID", "LIKE_STATE", "like", intchar2intnum)
    n_elems_q2u += insert_kv_into_mat(q2u, dt_q_inter, "QUESTION_ID", "USER_ID", "DISLIKE_STATE", "dislike", intchar2intnum)
    n_elems_q2u += insert_kv_into_mat(q2u, dt_q_inter, "QUESTION_ID", "USER_ID", "FOLLOW_STATE", "follow", intchar2intnum)
    n_elems_u2q += insert_kv_into_mat(u2q, dt_q_inter, "USER_ID", "QUESTION_ID", "LIKE_STATE", "like", intchar2intnum)
    n_elems_u2q += insert_kv_into_mat(u2q, dt_q_inter, "USER_ID", "QUESTION_ID", "DISLIKE_STATE", "dislike", intchar2intnum)
    n_elems_u2q += insert_kv_into_mat(u2q, dt_q_inter, "USER_ID", "QUESTION_ID", "FOLLOW_STATE", "follow", intchar2intnum)

    # 2.4 回答赞、踩计数
    n_elems_q2u += insert_kv_into_mat(q2u, dt_a_inter, "ANSWER_ID", "USER_ID", "LIKE_STATE", "like", intchar2intnum)
    n_elems_q2u += insert_kv_into_mat(q2u, dt_a_inter, "ANSWER_ID", "USER_ID", "DISLIKE_STATE", "dislike", intchar2intnum)
    n_elems_u2q += insert_kv_into_mat(u2q, dt_a_inter, "USER_ID", "ANSWER_ID", "LIKE_STATE", "like", intchar2intnum)
    n_elems_u2q += insert_kv_into_mat(u2q, dt_a_inter, "USER_ID", "ANSWER_ID", "DISLIKE_STATE", "dislike", intchar2intnum)

    # 2.5 评论计数
    n_elems_q2u += insert_kv_into_mat(q2u, dt_comments, "QUESTION_ID", "FROM_USER_ID", None, "comment", none2val)
    n_elems_u2q += insert_kv_into_mat(u2q, dt_comments, "FROM_USER_ID", "QUESTION_ID", None, "comment", none2val)

    print('2.5, n_elems_q2u = {}, n_elems_u2q = {}'.format(n_elems_q2u, n_elems_u2q))
    assert n_elems_q2u == n_elems_u2q

    n_users = len(u2q)
    n_questions = len(q2u)
    print('finish QUESTION, n_users: {}, n_questions: {}'.format(n_users, n_questions))
    print('QA sparsity: {}'.format(n_elems_q2u / (n_users * n_questions))) # 只考虑参与过blog互动的用户的矩阵稀疏度

    n_users_all = len(dt_users['data']) - 1 # 目前谛听所有用户
    print('QA sparsity all: {}'.format(n_elems_q2u / (n_users_all * n_questions))) # 考虑全部用户的矩阵稀疏度

    return [q2u, u2q]

def gen_idea_mat_from_obj(dt_users, dt_idea, dt_like, dt_idea_comment, dt_idea_reply):
    """
    从相关pkl文件中抽取信息，返回QA类文档的'用户-文档矩阵'，为CF做准备
    q2u和u2q矩阵中每个元素都是一个N元组[like, dislike, follow, collect, comment, auth]
    """
    # 1.dt_q_inter: 解析用户问题评价
    i2u = {} # question被user评价过的记录
    u2i = {} # user评价question的记录

    n_elems_i2u = 0
    n_elems_u2i = 0

    # 2.1 想法作者计数
    n_elems_i2u += insert_kv_into_mat(i2u, dt_idea, "ID_IDEA", "ID_USER", None, 'author', none2val)
    n_elems_u2i += insert_kv_into_mat(u2i, dt_idea, "ID_USER", "ID_IDEA", None, 'author', none2val)

    # 2.2 想法赞计数. 注意：想法没有踩
    n_elems_i2u += insert_kv_into_mat(i2u, dt_like, "ID_CONTENT", "ID_USER", "LIKE_STATE", 'like', intchar2intnum)
    n_elems_u2i += insert_kv_into_mat(u2i, dt_like, "ID_USER", "ID_CONTENT", "LIKE_STATE", 'like', intchar2intnum)

    # 2.3 想法评论计数
    n_elems_i2u += insert_kv_into_mat(i2u, dt_idea_comment, "ID_IDEA", "ID_FROM_USER", None, 'comment', none2val)
    n_elems_u2i += insert_kv_into_mat(u2i, dt_idea_comment, "ID_FROM_USER", "ID_IDEA", None, 'comment', none2val)

    # 2.4 想法回复计数
    n_elems_i2u += insert_kv_into_mat(i2u, dt_idea_reply, "ID_IDEA", "ID_FROM_USER", None, 'comment', none2val)
    n_elems_u2i += insert_kv_into_mat(u2i, dt_idea_reply, "ID_FROM_USER", "ID_IDEA", None, 'comment', none2val)

    print('2.5, n_elems_i2u = {}, n_elems_u2i = {}'.format(n_elems_i2u, n_elems_u2i))
    assert n_elems_i2u == n_elems_u2i

    n_users = len(u2i)
    n_ideas = len(i2u)
    print('finish IDEA, n_users: {}, n_ideas: {}'.format(n_users, n_ideas))
    print('IDEA sparsity: {}'.format(n_elems_i2u / (n_users * n_ideas))) # 只考虑参与过blog互动的用户的矩阵稀疏度

    n_users_all = len(dt_users['data']) - 1 # 目前谛听所有用户
    print('IDEA sparsity all: {}'.format(n_elems_i2u / (n_users_all * n_ideas))) # 考虑全部用户的矩阵稀疏度

    return [i2u, u2i]

def gen_mat_from_pkl(func, *args):
    """读取pkl文件，返回'用户-问题'矩阵的稀疏表示q2u和u2q"""
    pkl_objs = []
    for pkl_pth in args:
        with open(pkl_pth, 'rb') as f:
            pkl_objs.append(pickle.load(f))
    pkl_objs = tuple(pkl_objs)

    # 生成矩阵 
    t0 = time.time()
    q2u, u2q = func(*pkl_objs)
    print('time_elapsed: {}s'.format(time.time() - t0))

    # 检查返回结果
    check_mat_empty_elem(q2u)
    check_mat_empty_elem(u2q)

    return [q2u, u2q]

def check_mat_empty_elem(mat):
    """检查二级字典矩阵中是否存在全0的元素"""
    cnt = 0
    for key0, val0 in mat.items():
        for key1, val1 in val0.items():
            if any(val1) is False:
                print('key0: {}, key1: {}, val: {}'.format(key0, key1, val1))
                ++cnt
    #print('cnt = {}'.format(cnt))
    assert cnt == 0

def main():
    """QA BLOG IDEA"""
    # QA
    q2u, u2q = gen_mat_from_pkl(gen_qa_mat_from_obj, 'data_pkl/USER/3.1_DT_USERS.pkl', 'data_pkl/QA/3.13_DT_QUESTIONS.pkl', 'data_pkl/QA/3.20_DT_ANSWERS.pkl', 'data_pkl/QA/3.15_DT_QUESTIONS_INTERACT.pkl', 'data_pkl/QA/3.22_DT_ANSWERS_INTERACT.pkl', 'data_pkl/QA/3.23_DT_COMMENTS.pkl')
    with open('data_mat/QA_i2u.pkl', 'wb') as f:
        pickle.dump(q2u, f)
    with open('data_mat/QA_u2i.pkl', 'wb') as f:
        pickle.dump(u2q, f)

    # BLOG
    q2u, u2q = gen_mat_from_pkl(gen_blog_mat_from_obj, 'data_pkl/USER/3.1_DT_USERS.pkl', 'data_pkl/BLOG/3.49_DT_BLOG_META.pkl', 'data_pkl/BLOG/3.56_DT_BLOG_COMMENT.pkl', 'data_pkl/BLOG/3.57_DT_BLOG_REPLY.pkl', 'data_pkl/BLOG/3.61_DT_BLOG_INTERACT.pkl', 'data_pkl/BLOG/3.58_DT_BLOG_COMMENT_INTERACT.pkl')
    with open('data_mat/BLOG_i2u.pkl', 'wb') as f:
        pickle.dump(q2u, f)
    with open('data_mat/BLOG_u2i.pkl', 'wb') as f:
        pickle.dump(u2q, f)

    # IDEA
    q2u, u2q = gen_mat_from_pkl(gen_idea_mat_from_obj, 'data_pkl/USER/3.1_DT_USERS.pkl', 'data_pkl/IDEA/3.84_DT_IDEA.pkl', 'data_pkl/IDEA/3.86_DT_LIKE.pkl', 'data_pkl/IDEA/3.87_DT_IDEA_COMMENT.pkl', 'data_pkl/IDEA/3.89_DT_IDEA_REPLY.pkl')
    with open('data_mat/IDEA_i2u.pkl', 'wb') as f:
        pickle.dump(q2u, f)
    with open('data_mat/IDEA_u2i.pkl', 'wb') as f:
        pickle.dump(u2q, f)

if __name__ == '__main__':
    #gen_blog_mat_from_pkl('data_pkl', 'xx')
    #gen_qa_mat_from_pkl('data_pkl', 'xx')
    main()




