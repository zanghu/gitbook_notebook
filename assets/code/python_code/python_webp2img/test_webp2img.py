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

from PIL import Image
import os

def webp2img_dep(webp_pth, img_pth):
    """"""
    assert os.path.isfile(webp_pth)
    im = Image.open(webp_pth)
    if im.mode=="RGBA":
        im.load()  # required for png.split()
        background = Image.new("RGB", im.size, (255, 255, 255))
        background.paste(im, mask=im.split()[3])  # 3 is the alpha channel
        im = background

    #im.save('{}'.format(img_pth),'JPEG')
    im.save(fp='{}'.format(img_pth),format=None)

def webp2img(webp_pth, img_pth):
    """"""
    assert os.path.isfile(webp_pth)
    im = Image.open(webp_pth).convert("RGB")
    im.save(fp='{}'.format(img_pth), format=None)

def webp2img_dir(webp_dir, img_dir):
    """"""
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
                if len(parts) > 2:
                    if parts[-2] in ['jpg', 'png']:
                        img_pth = os.path.join(img_dir, '.'.join(parts[:-1]))
                elif name[:-5].endswith('png'):
                    img_pth = os.path.join(img_dir, ''.join([name[:-4], 'png']))
                else:
                    img_pth = os.path.join(img_dir, ''.join([name[:-4], 'jpg']))
                #print('img_pth: {}'.format(img_pth))
                webp2img(webp_pth, img_pth)
                cnt += 1
    print('finish, cnt = {}'.format(cnt))

if __name__ == '__main__':
    #webp2img('002.jpg.webp', '01.jpg')
    #webp2img('002.jpg.webp', '01.png')
    webp2img_dir('123', '123_jpg')