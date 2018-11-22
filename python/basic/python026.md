## Python: 逻辑运算符and和or的返回值

参考资料：[深入分析python的and or 返回值](https://www.jianshu.com/p/d4059a76a7a3)

最近在参考CTW数据集基准方案代码`ctw-baseline`时，看到了以下源码：

```python
    ...
    a.append({'bbox': bbox, 'text': text or '■', 'color': colormap[taken]})
    ...
    ```



