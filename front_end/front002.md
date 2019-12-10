### 为什么执行JS语句在控制台最后一个输出时undefined？

[CSDN: 为什么console.log()后会有一个undefined](https://blog.csdn.net/qiansuike/article/details/90765903)

控制台执行语句时，会先执行语句内容，最后在控制台打印语句返回值。因此，对于那些没有返回值的语句，最后的控制台输出就是`undefined`。

**实验一**

使用linux命令行的`node`

```js
> var p = {x: 1}; // 定义变量语句没有返回值，所以输出undefined
undefined
>x = 1; // 等号运算符有返回值，因此输出的时表达式返回值
1
> console.log("123"); // console.log没有返回值，所以执行结束后显示undefined
123
undefined
```

**实验二**

使用chrome的JS脚本调试功能

```js
function p(x) {
    console.log(x);
    return true;
}
p("123");
```

运行结果：

![](/assets/front002_01.PNG)