## datetime: 查看指定日期属于一年中的哪一周

### 一、datetime生成

```python
from datetime import datetime

# 当前时间
now=datetime.now()

# 指定时间
test=datetime(2020,1,26,11,11,11)
```

### 二、datetime转字符串

* str 强转类型
```python
str(datetime(2020,1,3))
strftime：根据传入格式
datetime.now().strftime('%Y-%m-%d')
```

三、字符串转datetime
strptime：解析已知格式的时间
datetime.strptime('1/26/2021','%m/%d/%Y')
dateutil.parse 可以解析几乎所有人类能够理解的日期表示形式
from dateutil.parser import parse
parse('2020-01-03')
parse('Jan 31, 2021 10:45 PM')
四、datetime时间差计算
timedelta：表示两个datetime之间的差（日、秒、毫秒）

# datetime相减
datetime(2019,1,7) - datetime(2021,6,24,8,15)
# datetime加timedelta
datetime(2021,1,7)+timedelta(12)
