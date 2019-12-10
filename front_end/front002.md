### 为什么执行JS语句在控制台最后一个输出时undefined？

[CSDN: 为什么console.log()后会有一个undefined](https://blog.csdn.net/qiansuike/article/details/90765903)

控制台执行语句时，会先执行语句内容，最后在控制台打印语句返回值。因此，对于那些没有返回值的语句，最后的控制台输出就是`undefined`。