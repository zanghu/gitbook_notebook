#coding=utf-8
#!/usr/bin/env python

"""
from: https://qiita.com/hengsokvisal/items/e4610ec5c39a2f0045cc

First of all , we have to install webp by using pip

```shell
pip install webp
```

then , we have to install pillow. If you have pillow install please reinstall it because it is not going to work. you can reinstall pillow by

```shell
pip uninstall Pillow
pip install Pillow
```

"""

g_img_ext = ('jpg', 'png', 'jpeg')

from PIL import Image
import os
import time
import shutil
import zipfile

def zip_dir(dir_pth, zip_pth):
    """
    压缩指定文件夹
    :param dir_pth: 目标文件夹路径
    :param zip_pth: 压缩文件保存路径+xxxx.zip
    """
    assert os.path.isdir(dir_pth)
    zip = zipfile.ZipFile(zip_pth, "w", zipfile.ZIP_DEFLATED)
    for path, dirnames, filenames in os.walk(dir_pth):
        fpath = path.replace(dir_pth, '') # 去掉目标跟路径，只对目标文件夹下边的文件及文件夹进行压缩
        for filename in filenames:
            zip.write(os.path.join(path, filename), os.path.join(fpath, filename))
    zip.close()

def webp2img(webp_pth, img_pth):
    """"""
    assert os.path.isfile(webp_pth)
    im = Image.open(webp_pth).convert("RGB")
    im.save(fp='{}'.format(img_pth), format=None)

def webp2img_single_dir_mtime(webp_dir, img_dir, default_suffix='.png'):
    """
    1.按照文件最近修改时间确定文件名, 适用于图片顺序保存但文件名是特殊码的情况
    2.如果文件不是webp, 则只参加重命名不会进行转换
    """
    global g_img_ext
    assert os.path.isdir(webp_dir)
    if not os.path.exists(img_dir):
        os.mkdir(img_dir)
    else:
        assert os.path.isdir(img_dir)

    # 获取全部webp文件创建时间
    cnt = 0
    for dirpath, dirnames, filenames in os.walk(webp_dir):
        file_mtime = {} # {文件路径: 文件最后修改时间}
        for name in filenames:
            img_pth = os.path.join(dirpath, name)
            if name.endswith('.webp'):
                assert not (img_pth in file_mtime)
                file_mtime[img_pth] = [time.ctime(os.path.getmtime(img_pth)), True] # 文件最后修改时间
                cnt += 1
            elif name.lower().endswith(g_img_ext):
                assert not (img_pth in file_mtime)
                print('Warning: file {} not endswith ".webp" but is also img, will be rename and copy'.format(os.path.join(dirpath, name)))
                file_mtime[img_pth] = [time.ctime(os.path.getmtime(img_pth)), False] # 文件最后修改时间
                cnt += 1

    # 按照创建时间重命名
    a = sorted(file_mtime.items(), key=lambda x: x[1][0], reverse=False) # 排序
    n_zfill = 3
    if len(a) > 999:
        n_zfill = 4 # 不大可能出现单目录下文件数大于1W, 4应该足够
    b = [[os.path.join(img_dir, str(n).zfill(n_zfill) + default_suffix), webp_pth, need_trans] for n, [webp_pth, [_, need_trans]]in enumerate(a)]

    # 图片转换
    for [img_pth, webp_pth, need_trans] in b:
        if need_trans:
            webp2img(webp_pth, img_pth)
        else:
            shutil.copyfile(webp_pth, img_pth)

    print('finish dir {}, cnt = {}'.format(img_dir, cnt))
    return cnt

def webp2img_single_dir(webp_dir, img_dir):
    """"""
    global g_img_ext
    assert os.path.isdir(webp_dir)
    if not os.path.exists(img_dir):
        os.mkdir(img_dir)
    else:
        assert os.path.isdir(img_dir)

    cnt = 0
    for dirpath, dirnames, filenames in os.walk(webp_dir):
        for name in filenames:
            if name.endswith('.webp'):
                webp_pth = os.path.join(dirpath, name)
                parts = name.split('.');
                img_pth = None
                if len(parts) > 2:
                    #if parts[-2].lower() in ['jpg', 'png', 'jpeg', 'JPG', 'PNG', 'JPEG']: # 根据倒数第二段后缀名决定转化后的名称
                    if parts[-2].lower() in g_img_ext: # 根据倒数第二段后缀名决定转化后的名称
                        img_pth = os.path.join(img_dir, '.'.join(parts[:-1]))
                    else:
                        img_pth = os.path.join(img_dir, ''.join([name[:-4], 'jpg'])) # 如果文件不存在倒数第二段后缀名, 则使用jpgh
                #print('img_pth: {}'.format(img_pth))
                webp2img(webp_pth, img_pth)
                cnt += 1
            else:
                print('Warning: file {} not endswith ".webp" and will be ignored.'.format(os.path.join(dirpath, name)))
        break
    print('finish dir {}, cnt = {}'.format(img_dir, cnt))
    return cnt

def webp2img_multi_dirs(webp_dir, img_dir, zip_pth, need_rename):
    """
    @param
        zip_pth: 自动生成压缩包
        need_rename: 当文件名不是顺序数字时, 应设为True, 否则为False
    """
    assert os.path.isdir(webp_dir)
    if not os.path.exists(img_dir):
        os.mkdir(img_dir)
    else:
        assert os.path.isdir(img_dir)

    cnt_dir = 0 # 处理完成的目录总数
    cnt_img = 0 # 处理完成的图片总数
    for dirpath, dirnames, filenames in os.walk(webp_dir):
        for sub_name in dirnames:
            subdir_pth = os.path.join(dirpath, sub_name)
            assert os.path.isdir(subdir_pth)
            if not os.path.exists(img_dir):
                os.makedirs(img_dir)
            dstdir_pth = os.path.join(img_dir, sub_name)
            if need_rename:
                cnt_img += webp2img_single_dir_mtime(subdir_pth, dstdir_pth)
            else:
                cnt_img += webp2img_single_dir(subdir_pth, dstdir_pth)
            cnt_dir += 1
        break
    if zip_pth is not None:
        zip_dir(img_dir, zip_pth)
    print('all subdir finish, finish num_dirs = {}, finish num_imgs: {}'.format(cnt_dir, cnt_img))

if __name__ == '__main__':
    #webp2img('002.jpg.webp', '01.jpg')
    #webp2img('002.jpg.webp', '01.png')
    #webp2img_single_dir('123', '123_jpg')
    webp2img_multi_dirs('data/webp', 'data/img', "data/img.zip", True)