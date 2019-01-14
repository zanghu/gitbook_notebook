#coding=utf-8
#!/usr/bin/env python

import httplib
import json
import hashlib
import copy
import os
import sys
import time
import traceback
import xlrd # 读excel
import xlwt # 写excel

# 解决非法key名称的问题, 否则会报错: 'ascii' codec can't encode characters in position 0-9: ordinal not in range(128)
default_encoding='utf-8'
if sys.getdefaultencoding() != default_encoding:
    reload(sys)
    sys.setdefaultencoding(default_encoding)

# 是否保存数据同步的.air文件
m_save_air = True
m_save_air_dir = 'air'

# md5加密密钥
m_md5_key = "b5729cf7dcf90df854d62b91d8ee62c1"

# rmc版本检查URL
m_rmc_vc_url = "/rmc/checkversion.action"

# rmc数据同步URL
m_rmc_ds_url = "/rmc/getdata.action"

# rmc的HTTP通讯报文头(不包含协议行)
m_rmc_header = {
        'content-type':'application/json',
        'Connection':'Keep-Alive',
        'User-Agent':'Apache-HttpClient/4.5 (Java/1.6.0)',
        'Accept-Encoding':'gzip,deflate'
}

# 版本检查报文体模板
m_vc_body_obj = {"esa":{"appId":None,"instanceId":"1b02048353e444c389102fc8c4c6d9aa","encryptedData":None,"provinceCode":"99","channelCode":"ATM","esaVersion":"ACC_C_1.6.0","consumerIP":"127.0.0.1|127.0.0.2|10.230.147.223"},"version":5103}

# 数据同步请求报文体模板
m_ds_body_obj = {"esa":{"appId":None,"instanceId":"69c86748aa46463dbaf5fe29c2b9e6c3","encryptedData":None,"provinceCode":"99","channelCode":"ATM","esaVersion":"ACC_C_1.6.0","consumerIP":"127.0.0.1|127.0.0.2|10.230.147.223"},"entityInfoSet":None}


def check_data_type(entity_dto_list, err_dump_path):
    """"""
    assert isinstance(entity_dto_list, list)

    # 1.解析schema数据, 每一行的数据类型, 和trans数据
    schema_info_dict = {} # {'id': [[line_path, dtype], ...], ...}
    trans_info_dict = {}  # {'id': [proto, pkg], ...}
    for entity in entity_dto_list:
        id = entity['id']
        entity_type = entity['type']
        if entity_type == 'I' or entity_type == 'PI':
            lines = entity["schemaItems"]
            assert isinstance(lines, list)
            schema_info_dict[id] = [lines, entity_type]
        elif entity_type == 'T':
            proto = entity["protocol"]
            pkg = entity["pkgType"]
            provider_id = entity["providerId"] 
            trans_info_dict[id] = [proto, pkg, provider_id]

    # 3.根据通讯协议和报文类型来校验schema数据类型
    err_infos = {} # {schema_id:[[line_id, err_dtp], ...], ...}
    for schema_id, (lines, type_str) in schema_info_dict.iteritems():
        # 查找报文类型和通讯协议
        proto = None
        pkg = None
        for trans_id in trans_info_dict:
            if schema_id.startswith(trans_id):
                proto, pkg, provider_id = trans_info_dict[trans_id]
                break
        #print 'schema: {0}'.format(schema_id)
        #print trans_info_dict
        assert proto is not None
        assert pkg is not None

        errs = []
        for  line in lines:
            #print lines
            #print line.__class__
            #print line
            #sys.exit(0)
            assert isinstance(line, dict)
            line_id = line['pth']
            #line_key = line['key'] # 也需要检查编码
            #seq = line['seq'] # 编码出错时，保存序号
            dtp = line['dtp']
            lev = line['lev']

            err = []
            #pth_cannot_use = 0 # 编码错误时, 不能在错误信息中打印pth, 否会导致excel无法保存
            # 1.检查数据类型错误

            # a.) TULIP/ATII XML
            if (pkg == 'ATII_XML' or pkg == 'XML') and proto == 'TCP':
                if dtp != 'OBJECT' and dtp != 'VALUE':
                    err.append("dtp_err: {0}".format(dtp))
            # b.) BoEing
            elif (pkg == 'B3' or pkg == 'B4') and proto == 'TCP':
                if dtp != 'MVIEW' and dtp != 'MSHORT' and dtp != 'MINTEGER' and dtp != 'MDECIMAL' and dtp != 'MCHAR' and dtp != 'MCHARAR' and dtp != 'MN_VARCHAR' and dtp != 'MMIXED' and dtp != 'MDATETIME' and dtp != 'MLONG':
                    err.append("dtp_err: {0}".format(dtp))
            # c.) json
            elif pkg == 'JSON' and proto == 'HTTP':
                if dtp != 'OBJECT' and dtp != 'BOOLEAN' and dtp != 'STRING' and dtp != 'NUMBER':
                    err.append("dtp_err: {0}".format(dtp))
            # d.) ln_xml
            elif pkg == 'LN_XML' and proto == 'TCP':
                if dtp != 'OBJECT' and dtp != 'VALUE':
                    err.append("dtp_err: {0}".format(dtp))
            # e.) soap
            elif pkg == 'SOAP' and proto == 'HTTP':
                if dtp != 'OBJECT' and dtp != 'VALUE':
                    err.append("dtp_err: {0}".format(dtp))
            # f.) unknow
            else:
                #raise NotImplementedError('Unknow pkg type: {0} or unknown protocol type: {1}'.format(pkg, proto)) # 2018.07.16, 不再抛出异常, 改为记录异常
                err.append("pkg_proto_err: \"{0}\" + \"{1}\"".format(pkg, proto))

            # 2.检查lev值错误
            #print 'lev = {0}'.format(lev)
            if int(lev) < 1:
                err.append("lev_err: {0}".format(lev))

            """
            # 3.检查pth编码错误
            print 'line_id: {0}'.format(line_id)
            try:
                line_id.decode('ascii') # 可以检查出现存的乱码问题, 对于乱码会抛出UnicodeDecodeError异常
            except Exception, e:
                print 'UnicodeDecodeError detected, schema line pth: {0}, seq: {1}'.format(line_id, seq)
                err.append("pth_codec_err: seq({0})".format(seq))
                pth_cannot_use = 1
            #line_id.encode('utf-8', 'strict')

            # 4.检查key编码错误
            #print 'line_id: {0}'.format(line_id)
            try:
                line_key.decode('ascii') # 可以检查出现存的乱码问题, 对于乱码会抛出UnicodeDecodeError异常
            except Exception, e:
                print 'UnicodeDecodeError detected, schema line key: {0}, seq: {1}'.format(line_key, seq)
                err.append("key_codec_err: seq({0})".format(seq))
            """

            # 汇总当前行错误: 如果当前行出现错误, 那么进行记录
            if len(err) > 0:
                #if pth_cannot_use:
                #    errs.append([seq, ', '.join(err)])
                #else:
                #    errs.append([line_id, ', '.join(err)])
                errs.append([line_id, ', '.join(err)])

        # 如果至少有一行出现错误, 记录当前schema
        if len(errs) > 0:
            err_infos[schema_id] = [errs, proto, pkg, provider_id, type_str]

    # 4.导出数据
    num_err_lines = 0
    if len(err_infos) > 0:
        with open(err_dump_path, 'wb') as f:
            for cnt, (key, val) in enumerate(err_infos.iteritems()):
                err_infos, proto, pkg, provider_id, type_str = val # type是用来标记是"I"还是"PI"
                f.write(str(cnt).zfill(3) + '.' + key + '|{0}|{1} + {2}|{3}:\n'.format(type_str, proto, pkg, provider_id))
                for err_info in err_infos:
                    line_id = err_info[0]
                    err_msg = err_info[1]
                    f.write('key: {0}, err: {1}\n'.format(line_id, err_msg))
                f.write('\n')
                num_err_lines += len(val)
    print 'err schema: {0}, err line: {1}'.format(len(err_infos), num_err_lines)


def invoke_http_post(ip, port, url, body, header):
    """"""
    # 发送请求获取响应
    conn = httplib.HTTPConnection(ip, port)
    conn.request('POST', url, body, header)
    res = conn.getresponse()
    http_res_body = res.read()

    return http_res_body


def rmc_vc(app_id, enc_data, data_ver=0, rmc_ip="10.230.135.55", rmc_port=8001):
    """向RMC发送版本检查请求并获得响应"""
    global m_rmc_header
    global m_vc_body_obj
    global m_md5_key
    global m_rmc_vc_url

    # 组装http请求报文体
    vc_body_obj = copy.deepcopy(m_vc_body_obj)
    vc_body_obj["esa"]["appId"] = app_id
    vc_body_obj["esa"]["encryptedData"] = enc_data
    vc_body_obj["version"] = data_ver

    # 发送请求获取响应
    #conn = httplib.HTTPConnection(rmc_ip, rmc_port)
    #conn.request('POST', m_rmc_vc_url, json.dumps(vc_body_obj), m_rmc_header)
    #res = conn.getresponse()
    #http_res_body = res.read()

    vc_body = json.dumps(vc_body_obj)
    http_res_body = invoke_http_post(rmc_ip, rmc_port, m_rmc_vc_url, vc_body, m_rmc_header)

    # 记录版本检查请求和返回报文
    vc_req_path = '_'.join([app_id, rmc_ip, 'vc_req_res']) + '.txt'
    with open(vc_req_path, 'wb') as f:
        f.write('==========vc req==========\n')
        f.write(vc_body)
        f.write('\n')
        f.write('==========vc res==========\n')
        f.write(http_res_body)
        f.write('\n')

    #print http_res_body

    # 取出返回json报文中有用的部分
    res_obj = json.loads(http_res_body)
    assert isinstance(res_obj, dict)
    entity_info_list = res_obj["entityInfoSet"]
    return entity_info_list


def rmc_ds(app_id, enc_data, entity_info_list, rmc_ip="10.230.135.55", rmc_port=8001):
    """进行数据同步"""
    global m_ds_body_obj
    global m_rmc_ds_url
    global m_rmc_header
    global m_save_air
    global m_save_air_dir
    assert isinstance(entity_info_list, list)

    # 组装http请求报文体
    ds_body_obj = copy.deepcopy(m_ds_body_obj)
    ds_body_obj["esa"]["appId"] = app_id
    ds_body_obj["esa"]["encryptedData"] = enc_data
    ds_body_obj["entityInfoSet"] = []

    cnt = 0
    batch = 0
    entity_dto_list = []
    #print entity_info_list
    #print '==========\n\n'
    #sys.exit()

    # 对entity_info_list中的全部对象进行请求, 注意单次最大请求实体数不能超过200
    num = 0
    http_msg_list = []
    for i, info in enumerate(entity_info_list):
        info_new = copy.deepcopy(info)
        del(info_new['version'])
        ds_body_obj["entityInfoSet"].append(info_new)
        cnt+= 1
        num += 1

        if cnt >= 200 or i == len(entity_info_list) - 1: # 注意要把最后不足200的部分也请求
            ds_body = json.dumps(ds_body_obj)
            #print 'batch = {0}, ds_body:\n{1}\n'.format(batch, ds_body)
            http_res_body = invoke_http_post(rmc_ip, rmc_port, m_rmc_ds_url, ds_body, m_rmc_header)
            #print http_res_body
            if m_save_air:
                http_msg_list.append(http_res_body)

            # 取出返回json报文中有用的部分
            res_obj = json.loads(http_res_body)
            assert isinstance(res_obj, dict)
            entity_dto_list += res_obj["entityDtoSet"]

            ds_body_obj["entityInfoSet"] = []
            cnt = 0

            batch += 1

    if m_save_air is True:
        if os.path.isdir(m_save_air_dir) is False:
            os.mkdir(m_save_air_dir)
        name = '_'.join([app_id, rmc_ip, str(rmc_port)]) + '.air'
        air_path = os.path.join(m_save_air_dir, name)
        with open(air_path, 'wb') as f:
            for line in http_msg_list:
                f.write(line)
                f.write('\n')

    print 'app_id: {0}, total batch = {1}, len(entity_dto_list) = {2}, num = {3}, len(entity_info_list) = {4}'.format(app_id, batch, len(entity_dto_list), num, len(entity_info_list))
    return entity_dto_list

def rmc_vc_ds(app_id, token=None, enc_data=None, data_ver=0, rmc_ip="10.230.135.55", rmc_port=8001):
    """输出app_id和token, 自动从指定RMC上获取数据并检查其数据正确性"""
    global m_md5_key
    assert (token is not None) or (enc_data is not None)

    # 计算加密数据
    if enc_data is None:
        data = m_md5_key + token
        m1 = hashlib.md5()
        m1.update(data)
        enc_data = m1.hexdigest().upper()
        #print enc_data

    # 进行版本检查
    entity_info_list = rmc_vc(app_id, enc_data, data_ver, rmc_ip, rmc_port)

    # 进行数据同步
    entity_dto_list = rmc_ds(app_id, enc_data, entity_info_list, rmc_ip, rmc_port)

    return entity_dto_list

def test_rmc_vc_ds():
    """"""
    err_dump_path = "err.txt"

    #entity_dto_list = rmc_vc_ds(app_id="SDC201108_01", enc_data="0F0B38A53C6EAE286011FAB70982ABE1")
    entity_dto_list = rmc_vc_ds(app_id="SDC201108_01", token="c0eb80f40d484eefa1dcec5782f2ebe9")

    # 检查数据类型错误
    check_data_type(entity_dto_list, err_dump_path)

def rmc_app_data_verify(app_id, token=None, enc_data=None, data_ver=0, rmc_ip="10.230.135.55", rmc_port=8001, dst_dir='err'):
    """测试rmc_vc_ds_verify()函数"""
    assert (token is not None) or (enc_data is not None)
    if os.path.isdir(dst_dir) is False:
        os.mkdir(dst_dir)

    err_file = '_'.join([app_id, rmc_ip, str(rmc_port), "err.txt"])
    err_dump_path = os.path.join(dst_dir, err_file)

    # 从RMC获取数据
    t0 = time.time()
    entity_dto_list = rmc_vc_ds(app_id, token, enc_data, data_ver, rmc_ip, rmc_port)
    print '{0} RMC: {1}:{2} DS + VC time elapsed: {3}s'.format(app_id, rmc_ip, rmc_port, time.time() - t0)

    # 检查数据类型错误
    t1 = time.time()
    check_data_type(entity_dto_list, err_dump_path)
    print '{0} RMC: {1}:{2} data check time elapsed: {3}s'.format(app_id, rmc_ip, rmc_port, time.time() - t1)

def rmc_data_verify(app_list_path, rmc_ip="10.230.135.55", rmc_port=8001, dst_dir='err'):
    """从指定文件中读取app_id和加密数据, 循环检查全部
       文件中每一行是用空格分隔的三个项, 按顺序是: app_id, token, enc_data
       token和enc_data之一可以为None, 如果都不为None那么优先选用token
    """
    assert os.path.isfile(app_list_path)
    #assert os.path.isdir(dst_dir)

    # 读取app_id列表所在文件
    with open(app_list_path, 'rb') as f:
        lines = f.readlines()

    #cnt = 0
    for line in lines:
        app_id, token, enc_data = line.split(' ')
        assert (token != 'None' or enc_data != 'None')

    # 注意检查数据
    print 'start verify apps in {0} from rmc {1}:{2}'.format(app_list_path, rmc_ip, rmc_port)
    t0 = time.time()
    cnt = 0
    err_mark = False
    num_err = 0
    records = []
    data_ver = 0; # 默认当前没有数据, 版本号是0
    for line in lines:
        t1 = time.time()
        app_id, token, enc_data = line.split(' ')
        try:
            if token == 'None':
                rmc_app_data_verify(app_id, None, enc_data, data_ver, rmc_ip, rmc_port, dst_dir)
            else:
                rmc_app_data_verify(app_id, token, None, data_ver, rmc_ip, rmc_port, dst_dir)
        except Exception, e:
            err_mark = True
            print Exception, ":", e
            traceback.print_exc()
            raise Exception

        if err_mark:
            num_err += 1
            record = 'cnt = {0}, app_id: {1} failed, time elapsed: {2}s'.format(cnt, app_id, time.time() - t1)
        else:
            record = 'cnt = {0}, app_id: {1} success, time elapsed: {2}s'.format(cnt, app_id, time.time() - t1)
        err_mark = False

        records.append(record)
        print record
        cnt += 1

    with open('verify_record.txt', 'wb') as f:
        for line in records:
            f.write(line)
            f.write('\n')

    print 'all finish, rmc: {0}:{1}, total app num: {2}, num_exception: {3}, time elapsed: {4}s'.format(rmc_ip, rmc_port, cnt, num_err, time.time() - t0)


def merge_err_info_new(err_dir='err'):
    """将指定目录下各个消费方应用系统的错误文件进行汇总，得到一个错误信息文件
       合并原则: 每个交易占一个段落; 交易中出错的内容分为五部分(5个字段)
                 第一子段: 交易信息错误
                 第二子段: schema_in信息错误
                 第三子段: schema_ou信息错误
                 第四子段: schema_in|PI信息错误
                 第五子段: schema_ou|PI信息错误
    """
    assert os.path.isdir(err_dir)
    file_names = os.listdir(err_dir)
    file_paths = [os.path.join(err_dir, name) for name in file_names if name.endswith('.txt')]
    #print 'len(file_paths) = {0}'.format(len(file_paths))

    # 错误交易字典
    trans_dict = {}

    num_trans_before = 0
    num_trans_after = 0
    for file_path in file_paths:
        with open(file_path, 'rb') as f:
            lines = f.readlines()

        is_start = True
        key = None
        para = []
        for line in lines:
            if is_start:
                elems = line.rstrip('\r\n').split('.')[1].split('|')
                key = '|'.join([elems[0], elems[1]]) # key = "交易码|版本号"
                para.append(line.split('.')[1]) # 段落首行也要加入, 但是要先去掉原序号
                is_start = False
                continue
            else:
                if line.rstrip('\r\n') == '':
                    assert key is not None
                    if not (key in trans_dict):
                        trans_dict[key] = []
                        num_trans_after += 1
                    trans_dict[key].append(''.join(para))
                    is_start = True
                    key = None
                    para = []
                    num_trans_before += 1
                    continue
                else:
                    para.append(line)
                    continue
        print 'file: {0} finish, num lines: {1}'.format(file_path, len(lines))

    print 'num_err_before: {0}, num_err_after: {1}'.format(num_trans_before, num_trans_after)

    #整理每个交易大段落中重复的子段
    trans_dict_new = {}
    for key, para in trans_dict.iteritems():
        new_para = []
        mark_set = set()
        for x, sub_para in enumerate(para):
            mark = '|'.join(sub_para.rstrip('\r\n').split('|')[:4])
            if not mark in mark_set:
                mark_set.add(mark)
                new_para.append(sub_para)
        trans_dict_new[key] = new_para
    trans_dict = trans_dict_new

    res_dir = os.path.join(err_dir, 'err_merge')
    if not os.path.isdir(res_dir):
        os.mkdir(res_dir)

    xls_path = os.path.join(res_dir, 'err_merge.xls')
    merge_to_excel(trans_dict, xls_path)

    #dst_path = os.path.join(res_dir, 'err_merged.txt')
    #with open(dst_path, 'wb') as f:
    #    for n, (key, val) in enumerate(trans_dict.iteritems()):
    #        f.write('.'.join([str(n).zfill(3), key]))
    #        f.write('\n')
    #        f.write('\n\n'.join(val))
    #        f.write('\n')


def merge_to_excel(trans_dict, xls_path):
    """"""
    #assert os.path.isfile(txt_path)
    if os.path.isfile(xls_path):
        os.remove(xls_path)

    wbk = xlwt.Workbook(encoding='utf-8') # 设置编码后, 当excel内有中文时，保存不会报错
    sheet = wbk.add_sheet('sheet1')

    # 单元格实体边框
    borders = xlwt.Borders()
    borders.left = 1
    borders.right = 1
    borders.top = 1
    borders.bottom = 1

    # 设置单元格对齐格式（本例中原则上同一列的单元格采用同一种风格）
    alignment_hv_center = xlwt.Alignment()
    alignment_hv_center.horz = xlwt.Alignment.HORZ_CENTER
    alignment_hv_center.vert = xlwt.Alignment.VERT_CENTER
    style0 = xlwt.XFStyle()
    style0.alignment = alignment_hv_center
    style0.borders = borders

    alignment_v_center = xlwt.Alignment()
    alignment_v_center.vert = xlwt.Alignment.VERT_CENTER
    style1 = xlwt.XFStyle()
    style1.alignment = alignment_v_center
    style1.borders = borders
    style2 = style1
    style3 = style1
    style4 = style1

    # 第0行: 列名称行

    # 设置字体
    font = xlwt.Font()
    font.bold = True # 字体加粗
    font.name = u'微软雅黑'

    # 设置单元格背景
    pattern = xlwt.Pattern
    pattern.pattern = xlwt.Pattern.SOLID_PATTERN
    pattern.pattern_fore_colour = 23
    pattern.pattern_back_colour = 1

    style_r0 = xlwt.XFStyle()
    style_r0.alignment = alignment_hv_center # 水平居中+垂直居中对齐
    style_r0.font = font
    style_r0.pattern = pattern
    style_r0.borders = borders

    sheet.write(0, 0, u'序号', style_r0)
    sheet.write(0, 1, u'交易名称', style_r0) # 交易码+版本号
    sheet.write(0, 2, u'协议报文', style_r0)
    sheet.write(0, 3, u'提供方', style_r0)
    sheet.write(0, 4, u'错误实体', style_r0)
    sheet.write(0, 5, u'错误详情', style_r0)

    for cnt, (key, val) in enumerate(trans_dict.iteritems()):
        #num_lines = len(val) # 交易错误信息要占用多少行([1, 5])

        rstart = cnt * 5 + 1

        # 写索引0的列(合并单元格): 序号
        content0 = "{0}".format(cnt + 1)
        #print 'rstart = {0}'.format(rstart)
        sheet.write_merge(rstart, rstart + 4, 0, 0, content0, style0) # 合并同一列的5行

        # 写索引1的列(合并单元格): 交易码
        #print 'key: {0}'.format(key)
        content1 = key.rstrip('\r\n') #.split('.')[1]
        sheet.write_merge(rstart, rstart + 4, 1, 1, content1, style1) # 合并同一列的5行
   
        # 写索引4、5的列: 错误类型 和错误详情
        content2 = None
        content3 = None
        style5 = xlwt.XFStyle()
        style5.alignment.wrap = 1
        style5.borders = borders
        num = 0 # 记录写了多少行, 剩余的行需要补空
        if len(val) > 5:
            print '===='
            for n, elem in enumerate(val):
                print n
                print elem
            print '====='
        for i, para in enumerate(val):
            # 第1-5行的第一列(合并单元格): 交易码
            first_line = para.rstrip('\r\n').split('\n')[0].rstrip(':')
            content4 = '|'.join(first_line.split('|')[:3])
            content5 = '\n'.join(para.rstrip('\r\n').split('\n')[1:])
            #print 'content4: {0}\ncontent5: {1}'.format(content4, content5)
            print 'r = {0}, cnt = {1}'.format(rstart+i, cnt)
            print 'content4: {0}\ncontent5: {1}'.format(content4, content5)
            sheet.write(rstart + i, 4, content4, style4)
            sheet.write(rstart + i, 5, content5, style5)
            if i == 0:
                content2 = first_line.split('|')[4]
                content3 = first_line.split('|')[5]
            num = i + 1

        for i in xrange(num, 5):
            sheet.write(rstart + i, 4, "", style4)
            sheet.write(rstart + i, 5, "", style5)

        # 写索引2和3的列(合并单元格)
        assert content2 is not None
        assert content3 is not None
        sheet.write_merge(rstart, rstart + 4, 2, 2, content2, style2) # 合并同一列的5行
        sheet.write_merge(rstart, rstart + 4, 3, 3, content3, style3) # 合并同一列的5行

    # 设置单元格宽度
    sheet.col(1).width = 256 * 4
    sheet.col(1).width = 256 * 20
    sheet.col(2).width = 256 * 16
    sheet.col(3).width = 256 * 16
    sheet.col(4).width = 256 * 24
    sheet.col(5).width = 256 * 36

    # 保存xls文件
    wbk.save(xls_path)

if __name__ == '__main__':
    #rmc_vc_ds(app_id="SDC201108_01", enc_data="0F0B38A53C6EAE286011FAB70982ABE1")
    #rmc_vc_ds(app_id="SDC201108_01", token="c0eb80f40d484eefa1dcec5782f2ebe9")
    #test_rmc_vc_ds()
    #rmc_data_verify(app_list_path='app_list.txt', rmc_ip="10.230.135.55", rmc_port=8001)

    rmc_data_verify(app_list_path='app_list_55.txt', rmc_ip="10.230.135.55", rmc_port=8001, dst_dir="err_55")
    merge_err_info_new("err_55") # 汇总信息

    #rmc_data_verify(app_list_path='app_list_58.txt', rmc_ip="10.230.135.58", rmc_port=8001, dst_dir="err")
    #merge_err_info("err") # 汇总信息

    #rmc_data_verify(app_list_path='app_list_test_complex.txt', rmc_ip="10.230.135.55", rmc_port=8001, dst_dir="err_test")
    #merge_err_info_new("err_test") # 汇总信息

    #rmc_data_verify(app_list_path='app_list_test_simple.txt', rmc_ip="10.230.135.55", rmc_port=8001, dst_dir="err_test")
    #merge_err_info_new("err_test") # 汇总信息

    #rmc_data_verify(app_list_path='app_list_55_unicode.txt', rmc_ip="10.230.135.55", rmc_port=8001, dst_dir="err_55_unicode")
    #merge_err_info_new("err_55_unicode") # 汇总信息

