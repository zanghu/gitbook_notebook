# PYTHON: 计算函数运行时间的装饰器

下面演示对使用函数装饰器的函数调用自省/反射方法**name**和**doc**的效果

```python
import time

def count_time(func):
    def int_time(*args, **kwargs):
        t0 = time.time()  # 程序开始时间
        result = func(*args, **kwargs)
        print('{} finish, time elapsed: {}s'.format(func.__name__, time.time() - t0))
        return result
    return int_time

@count_time
def read_data(train_file_pth):
    """"""
    data = pd.read_csv(train_file_pth) # "../input/jane-street-market-prediction/train.csv"
    data['resp_weight']=data['resp']*data['weight']
    data["action"] = (data["resp_weight"] > 0).astype('int')
    data['action'].value_counts()
    features = [col for col in data.columns if "feature" in col]
    return data, features
```