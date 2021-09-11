### PYTHON: 文件压缩

**参考资料**

简书: [Python压缩及解压文件](https://www.jianshu.com/p/271cb5c684e8)

#### 压缩为zip文件

```python
import zipfile
f = zipfile.ZipFile('压缩后zip文件的路径', 'w', zipfile.ZIP_DEFLATED, compresslevel=9)
f.write('被压缩的原始文件的路径')
f.close()
```





