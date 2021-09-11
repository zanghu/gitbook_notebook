### Jupyter: 下载服务端数据

**参考资料**

**medium**: [How to Upload/Download Files to/from Notebook in my Local machine](https://medium.com/@charles2588/how-to-upload-download-files-to-from-notebook-in-my-local-machine-6a4e65a15767)

#### 1.生成页面链接下载二进制文件

**代码如下**：

```python
from IPython.display import HTML
import base64

def create_download_link(src_pth, title="Download CSV file", filename="data.csv"):
    with open(src_pth, 'rb') as f:
        data = f.read()
    b64 = base64.b64encode(data)
    payload = b64.decode()
    html = '<a download="{filename}" href="data:text/csv;base64,{payload}" target="_blank">{title}</a>'
    html = html.format(payload=payload, title=title, filename=filename)
    return HTML(html)

create_download_link(src_pth=r'C:\Users\zanghu\Desktop\1.zip', title="Download ZIP file", filename="data.zip")
```

**运行效果**：

![](/assets/python058_02.png)

#### 2.生成页面链接下载DataFrame

**代码如下**：

```python
from IPython.display import HTML
import base64  
import pandas as pd  

def create_download_link(src_pth, title="Download CSV file", filename="data.csv"):  
    with open(src_pth, 'rb') as f:
        data = f.read()
    b64 = base64.b64encode(data.encode())
    payload = b64.decode()
    html = '<a download="{filename}" href="data:text/csv;base64,{payload}" target="_blank">{title}</a>'
    html = html.format(payload=payload,title=title,filename=filename)
    return HTML(html)

df = pd.DataFrame(data = [[1,2],[3,4]], columns=['Col 1', 'Col 2'])
create_download_link(df)
```

**运行效果**：

![](/assets/python058_01.png)



