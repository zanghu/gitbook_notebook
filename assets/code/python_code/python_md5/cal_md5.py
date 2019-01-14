import hashlib 


def md5_by_line(pth):
    """"""
    m = hashlib.md5()
    with open(file_path,'rb') as f:            #以二进制读的方式打开文件
        for line in f:                   #每次传入一"行"
            m.update(line)               #md5值更新
    md5_value = m.hexdigest()          #进制转化
    print(md5_value)

def md5_by_chunk(file):        
    md5_value=hashlib.md5()
    with open(file,'rb') as f:
        while True:
            data_flow=f.read(8096)       #每次读入8089kb进入内存
            if not data_flow:                 #读取完后返回空值，False
                break
            md5_value.update(data_flow)
    print(md5_value.hexdigest())

if __name__ == '__main__':
    #md5_by_chunk('/home/zanghu/123.jpg')
    md5_by_chunk("/home/zanghu/pylearn2_data/stl10_matlab/unlabeled.mat")