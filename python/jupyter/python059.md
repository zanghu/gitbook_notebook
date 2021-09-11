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


