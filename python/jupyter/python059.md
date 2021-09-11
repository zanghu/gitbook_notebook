### Jupyter: 自定义配置

参考资料: [清华大学Pypi镜像](https://mirror.tuna.tsinghua.edu.cn/help/pypi/)

#### 1.cell内容写入文件

```python
%%file 目标文件名
import numpy as np
import pandas as pd
...
```

#### 2.notebook内部画图

```python
import matplotlib.pyplot as plt
import seaborn as sns
%matplotlib inline
```

#### 3.执行命令行命令

* ps命令

```python
!ps aux
```

* 几个文件放入一个目录，在对目录打包切分

```python
!mkdir tmp
!mv *.zip tmp/
!tar -cvf tmp.tar tmp
!split -b 20M tmp.tar 
```

