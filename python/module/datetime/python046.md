## datetime: datetime与距离1970年1月1日的秒数相互转换

参考资料

CSDB：[python 的1970年秒数和datetime互相转换](https://blog.csdn.net/nini3816/article/details/81053994)

### datetime转1970年秒数

```python
import time,datetime
timeDateStr="2014-07-29 00:00:00"
time1=datetime.datetime.strptime(timeDateStr,"%Y-%m-%d %H:%M:%S")
secondsFrom1970=time.mktime(time1.timetuple())
```

### 1970年秒数转datetime

```python
import time,datetime
timeArray = time.localtime(1462482700)#1970秒数
print(timeArray)
otherStyleTime = time.strftime("%Y-%m-%d %H:%M:%S", timeArray)
datetime1=datetime.datetime.strptime(otherStyleTime, "%Y-%m-%d %H:%M:%S")
```