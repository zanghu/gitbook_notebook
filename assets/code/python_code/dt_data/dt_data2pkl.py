#coding=utf-8
#!/usr/bin/env python

"""
读取数据库导出数据，保存为*.pkl文件
"""

import os
import pickle
import xlrd
import time
import multiprocessing
import sys
import itertools

# 主键字典
g_main_key = {
              r"3.1_DT_USERS": "ID_USER",
              r"3.35_DT_FOLLOW": "ID",
              r"3.64_DT_USER_COUNT": "ID_USER",
              r"3.65_DT_USER_COLLECT": "ID_COLLECT",
              r"3.83_DT_USER_OPR_RECD": "ID_OPR_RECD",
              r"3.62_DT_USER_NEWS": "ID_USER_NEWS",
              r"3.13_DT_QUESTIONS": "QUESTION_ID",
              r"3.14_DT_QUESTIONS_COUNT": "QUESTION_ID",
              r"3.15_DT_QUESTIONS_INTERACT": "QUESTION_ID", # 有2主键，另一个是"USER_ID"
              r"3.20_DT_ANSWERS": "ANSWER_ID",
              r"3.21_DT_ANSWERS_COUNT": "ANSWER_ID",
              r"3.22_DT_ANSWERS_INTERACT": "ANSWER_ID", # 有2主键，另一个是"USER_ID"
              r"3.23_DT_COMMENTS": "COMMENT_ID",
              r"3.49_DT_BLOG_META": "ID_BLOG_META",
              r"3.56_DT_BLOG_COMMENT": "ID_BLOG_COMMENT",
              r"3.57_DT_BLOG_REPLY": "ID_BLOG_REPLY",
              r"3.58_DT_BLOG_COMMENT_INTERACT": "ID_BLOG_COMMENT", # 有2主键，另一个是"ID_FROM_USER"
              r"3.59_DT_BLOG_COMMENT_COUNT": "ID_BLOG_COMMENT",
              r"3.61_DT_BLOG_INTERACT": "ID_BLOG_META", # 有2主键，另一个是"ID_USERR" 
              r"3.84_DT_IDEA": "ID_IDEA",
              r"3.86_DT_LIKE": "ID_CONTENT", # 有3主键，还有"ID_USER"和"TYPE"
              r"3.87_DT_IDEA_COMMENT": "ID_IDEA_COMMENT",
              r"3.89_DT_IDEA_REPLY": "ID_IDEA_REPLY"
}

def xls2pkl(src_pth, dst_pth):
    """读取数据库导出的excel数据文件, 保存为python对象的序列化文件"""
    global g_main_key
    print('src_pth: {}'.format(src_pth))
    try:
        assert os.path.isfile(src_pth)
    except:
        print('exp src_path: {}'.format(src_pth))
        sys.stdout.flush()
        sys.exit()
        raise AssertionError()
        
    assert src_pth.endswith('.xlsx')
    table_name = os.path.splitext(os.path.basename(src_pth))[0]
    assert table_name in g_main_key
    key = g_main_key[table_name]
    parent_pth = os.path.dirname(dst_pth)
    if parent_pth != '':
        if not os.path.exists(parent_pth):
            os.makedirs(parent_pth)

    # 读取全部数据，保存为列表
    lines = []
    col_names = []
    key_idx = None
    with xlrd.open_workbook(src_pth) as table:
        sheet = table.sheets()[0]
        n_rows = sheet.nrows
        n_cols = sheet.ncols

        # 第一行的列名加入col_names
        assert n_rows > 0
        for j in range(n_cols):
            #print('j = {}, type: {}, value: {}'.format(j, type(sheet.cell(0, j).value), sheet.cell(0, j).value))
            col_names.append(sheet.cell(0, j).value)
        print(col_names)
        key_idx = col_names.index(key) # 确认主键确实是列名之一

        # 读取数据
        cnt = 0
        for i in range(1, n_rows):
            item = []
            for j in range(n_cols):
                item.append(sheet.cell(i, j).value)
                cnt += 1
            lines.append(item)
        print('{} read finish, num lines: {}'.format(src_pth, cnt))

    # 将全量数据保存为本地pkl
    dst = {}
    dst['cols'] = tuple(name for name in col_names)
    dst['data'] = lines
    assert not (key_idx is None)
    dst['key_idx'] = key_idx # 保存主键
    with open(dst_pth, 'wb') as f:
        pickle.dump(dst, f)
    print('{} dump finish'.format(dst_pth))

def txt2pkl(src_pth, dst_pth):
    """读取数据库导出的text数据文件, 保存为python对象的序列化文件"""
    print('src_pth: {}'.format(src_pth))
    assert os.path.isfile(src_pth)
    parent_pth = os.path.dirname(dst_pth)
    if parent_pth != '':
        if not os.path.exists(parent_pth):
            os.makedirs(parent_pth)

    # 读取全部数据，保存为列表
    lines = []
    col_names = None
    with open(src_pth, 'rb') as f:
        # 保存列名称
        col_names = f.readline().rstrip(b'\r\n').split(b'\t')
        for n, name in enumerate(col_names):
            print('[{}]: {}'.format(n, name))

        num_items_per_line = len(col_names) # 每行有多少个元素, 用来在读取时校验
        cnt = 1 # 计算开头的列名称行
        for n, line in enumerate(f):
            items = line.rstrip(b'\r\n').split(b'\t')
            try:
                assert len(items) == num_items_per_line
            except AssertionError as e:
                print('line: {}, content:\n{}'.format(n, line))
                print('len(items) = {}, num_items_per_line = {}'.format(len(items), num_items_per_line))
                raise AssertionError(e)
            lines.append(items)
            cnt += 1
        print('{} read finish, num lines: {}'.format(src_pth, cnt))

    # 将数据列表保存为本地pkl
    dst = {}
    dst['cols'] = tuple(name for name in col_names)
    dst['data'] = lines
    with open(dst_pth, 'wb') as f:
        pickle.dump(dst, f)
    print('{} dump finish'.format(dst_pth))

def data2pkl_dir(src_dir, dst_dir, src_suffix='.xlsx'):
    """递归的将src_dir下的所有数据文件转成pkl文件保存到dst_dir"""
    assert os.path.isdir(src_dir)
    assert src_suffix in [r'.tsv', r'.xlsx']
    data2pkl = None
    if src_suffix == r'.tsv':
        data2pkl = txt2pkl
    elif src_suffix == r'.xlsx':
        data2pkl = xls2pkl
    else:
        raise ValueError('')
    cnt = 0
    t0 = time.time()
    for root, dirs, files in os.walk(src_dir):
        # 递归深度优先遍历当前目录下的子目录
        for dir_name in dirs:
            src_dir_new = os.path.join(src_dir, dir_name)
            dst_dir_new = os.path.join(dst_dir, dir_name)
            cnt += data2pkl_dir(src_dir_new, dst_dir_new, src_suffix) # 递归

        # 处理当前目录下的普通文件
        for file_name in files:
            if file_name.endswith(src_suffix):
                src_pth = os.path.join(src_dir, file_name)
                dst_pth = os.path.join(dst_dir, os.path.splitext(file_name)[0] + '.pkl')
                data2pkl(src_pth, dst_pth)
                cnt += 1
        break
    print('finish dir: {}, num_files: {}, time_elapsed: {}s'.format(src_dir, cnt, time.time() - t0))
    return cnt

def get_all_paths_recur(src_dir, dst_dir, target_suffix, src_paths, dst_paths):
    """get_all_paths的递归核"""
    assert os.path.isdir(src_dir)
    assert target_suffix in [r'.tsv', r'.xlsx']
    assert isinstance(src_paths, list)
    assert isinstance(dst_paths, list)
    for root, dirs, files in os.walk(src_dir):
        # 递归深度优先遍历当前目录下的子目录
        for dir_name in dirs:
            get_all_paths_recur(os.path.join(src_dir, dir_name), os.path.join(dst_dir, dir_name), target_suffix, src_paths, dst_paths) # 递归

        # 处理当前目录下的普通文件
        for file_name in files:
            if file_name.endswith(target_suffix):
                src_paths.append(os.path.join(src_dir, file_name))
                dst_paths.append(os.path.join(dst_dir, os.path.splitext(file_name)[0] + '.pkl'))
        break

def get_all_paths(src_dir, dst_dir, target_suffix):
    """返回指定目录下指定后缀名的全部文件的路径"""
    src_paths = []
    dst_paths = []
    get_all_paths_recur(src_dir, dst_dir, target_suffix, src_paths, dst_paths)
    return src_paths, dst_paths

def data2pkl_dir_pool(src_dir, dst_dir, src_suffix='.xlsx', num_proc=4):
    """递归的将src_dir下的所有数据文件转成pkl文件保存到dst_dir, 使用进程池"""
    assert os.path.isdir(src_dir)
    assert src_suffix in [r'.tsv', r'.xlsx']
    data2pkl = None
    if src_suffix == r'.tsv':
        data2pkl = txt2pkl
    elif src_suffix == r'.xlsx':
        data2pkl = xls2pkl
    else:
        raise ValueError('')

    # 获取所有源文件和目标文件的路径
    src_paths, dst_paths = get_all_paths(src_dir, dst_dir, src_suffix) # 获取所有源文件路径
    #dst_paths = [os.path.join(os.path.dirname(path), os.path.splitext(os.path.basename(path))[0] + '.pkl') for path in src_paths]
    print('src_paths:\n{}'.format(src_paths))
    print('dst_paths:\n{}'.format(dst_paths))

    t0 = time.time()
    with multiprocessing.Pool(num_proc) as p:
        p.starmap(data2pkl, zip(src_paths, dst_paths))

    print('finish dir: {}, num_files: {}, time_elapsed: {}s'.format(src_dir, len(src_paths), time.time() - t0))

def check_pkl(pth):
    """校验pkl数据是否正常"""
    assert os.path.isfile(pth)
    with open(pth, 'rb') as f:
        data = pickle.load(f)
    print(type(data))
    print(data['cols'])
    print(data['cols'][data['key_idx']])
    print(data['data'][:5])

if __name__ == '__main__':
    #txt2pkl('data_orin/dt_answers_count', 'data_pickled/xx')
    #check_pkl('data_pickled/xx')
    #txt2pkl_dir('data_orin', 'data_pkl')

    #xls2pkl('data_orin/QA/3.13_DT_QUESTIONS.xlsx', 'xx')
    #check_pkl('xx')

    # 数据转换，顺序执行
    data2pkl_dir('data_orin', 'data_pkl', r'.xlsx')

    # 数据转换，进程池并发执行
    #data2pkl_dir_pool('data_orin', 'data_pkl', r'.xlsx')

    #xls2pkl_pool('data_orin/USER/3.83_DT_USER_OPR_RECD.xlsx', 'data_pkl/USER/3.83_DT_USER_OPR_RECD.pkl')