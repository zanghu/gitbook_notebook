# 关于Python函数定义形参表的形参前面的'\*'号和'\*\*'号

## 1.函数定义中的'\*'

允许用户调用函数时输入不定个参数, 例如: func\(a, b, c\), 函数自动将它们全部包装成一个列表args = \[a, b, c\], 供函数内使用

\`\`\` python

`def func1(*args):`

```
assert isinstance\(args, tuple\)

print 'args: {0}'.format\(args\)

for i, elem in enumerate\(args\):

    print 'args\[{0}\] = {1}'.format\(i, args\[i\]\)
```

\`\`\`

### 调用方法1

func1\(1, 2, '123'\)

### 调用方法2

c = \[1, 2, '123'\]

func1\(\*c\) \# 把list解包成不定参数

## 2.函数定义中的'\*\*'

允许用户调用函数时输入不定个键值对形式的参数, 例如: func\(a=1, b=2, c='123'\)函数自动将它们全部包装成一个字典args = {'a': 1, 'b: 2, 'c': '123'}, 供函数内使用

def func2\(\*\*args\):

```
assert isinstance\(args, dict\)

print 'args: {0}'.format\(args\)

for key, val in args.iteritems\(\):

    print 'args\[{0}\] = {1}'.format\(key, val\)
```

### 调用方法1

func2\(a=1,b= 2, c='123'\)

### 调用方法2

c = {a=1,b= 2, c='123'}

func2\(\*\*c\) \# 把dict解包成含有参数名的不定参数

## 3.非函数定义形参表中的'\*'号和'\*\*'号

不能直接把列表作为形参作为上述func1的参数，也不能直接把字典作为上述func的参数

如果希望用list作为形参作为上述func1的参数，或者用dict作为形参作为上述func2的参数, 必须先用\*和\*\*分别解包list和dict

a = \[1,2,3\]

func1\(\*a\) \# 把列表对象展开成不定参数

c = {a=1,b= 2, c='123'}

func2\(\*\*c\) \# 把dict解包成含有参数名的不定参数

