## 谷歌日志工具——glog

### 1.源码分析

#### 1.1.日志宏

* 检查宏定义

```c
// Equality/Inequality checks - compare two values, and log a FATAL message
// including the two values when the result is not as expected.  The values
// must have operator<<(ostream, ...) defined.
//
// You may append to the error message like so:
//   CHECK_NE(1, 2) << ": The world must be ending!";
//
// We are very careful to ensure that each argument is evaluated exactly
// once, and that anything which is legal to pass as a function argument is
// legal here.  In particular, the arguments may be temporary expressions
// which will end up being destroyed at the end of the apparent statement,
// for example:
//   CHECK_EQ(string("abc")[1], 'b');
//
// WARNING: These don't compile correctly if one of the arguments is a pointer
// and the other is NULL. To work around this, simply static_cast NULL to the
// type of the desired pointer.

#define CHECK_EQ(val1, val2) CHECK_OP(_EQ, ==, val1, val2)
#define CHECK_NE(val1, val2) CHECK_OP(_NE, !=, val1, val2)
#define CHECK_LE(val1, val2) CHECK_OP(_LE, <=, val1, val2)
#define CHECK_LT(val1, val2) CHECK_OP(_LT, < , val1, val2)
#define CHECK_GE(val1, val2) CHECK_OP(_GE, >=, val1, val2)
#define CHECK_GT(val1, val2) CHECK_OP(_GT, > , val1, val2)
```

上面的源码可见日志记录的关键是CHECK_OP宏函数。

* 跟踪第一层定义：CHECK_OP

```c
CHECK_OP_LOG(name, op, val1, val2, google::LogMessageFatal) // 实际被调用的
```

上面的代码可见接下来需要跟踪两个分支：CHECK_OP_LOG宏函数和google::LogMessageFatal

* 跟踪第二层定义：

```c
// CHECK_OP_LOG有三个#if...#else分支的定义，这里选取最简单的一个
typedef std::string _Check_string;
#define CHECK_OP_LOG(name, op, val1, val2, log)                         \
  while (google::_Check_string* _result =                \
         google::Check##name##Impl(                      \ //  形如Check_EQImpl
             google::GetReferenceableValue(val1),        \
             google::GetReferenceableValue(val2),        \
             #val1 " " #op " " #val2))                                  \
    log(__FILE__, __LINE__,                                             \
        google::CheckOpString(_result)).stream()
```

#### 1.2.