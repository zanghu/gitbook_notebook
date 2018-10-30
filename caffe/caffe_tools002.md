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

上面的源码可见日志记录的关键是CHECK\_OP宏函数。

* 跟踪第一层定义：CHECK\_OP

```c
CHECK_OP_LOG(name, op, val1, val2, google::LogMessageFatal) // 实际被调用的
```

上面的代码可见接下来需要跟踪两个分支：CHECK\_OP\_LOG宏函数和google::LogMessageFatal，先跟踪后者。

* 跟踪第二层定义中的google::LogMessageFatal：

```c
// This class happens to be thread-hostile because all instances share
// a single data buffer, but since it can only be created just before
// the process dies, we don't worry so much.
class GOOGLE_GLOG_DLL_DECL LogMessageFatal : public LogMessage {
 public:
  LogMessageFatal(const char* file, int line);
  LogMessageFatal(const char* file, int line, const CheckOpString& result);
  __attribute__ ((noreturn)) ~LogMessageFatal();
};
```

可见google::LogMessageFatal是一个类名，其派生自LogMessage类。

* 跟踪第二层定义中的CHECK\_OP\_LOG：

```c
// CHECK_OP_LOG有三个#if...#else分支的定义，这里选取最简单的一个
typedef std::string _Check_string;
#define CHECK_OP_LOG(name, op, val1, val2, log)                         \
  while (google::_Check_string* _result =                \
         google::Check##name##Impl(                      \ //  形如Check_EQImpl
             google::GetReferenceableValue(val1),        \ // 将全局变量变量等编程临时变量
             google::GetReferenceableValue(val2),        \
             #val1 " " #op " " #val2))                                  \
    log(__FILE__, __LINE__,                                             \
        google::CheckOpString(_result)).stream()
```

* 这里又产生了三个跟踪分支：Check\#\#\#name\#\#Impl、GetReferenceableValue和log.stream\(\)

logMessage类的stream方法的实际定义并未出现在`glog/logging.h`中，但大体可以猜出`log(__FILE__, __LINE__, google::CheckOpString(_result)).stream()`的含义是：  
（1）创建一个`google::LogMessageFatal`类对象；  
（2）将调用当前日志语句的原文件的文件名（`__FILE__`）、行号（`__LINE__`）和比较（EQ、NE、LE等等）结果（应该是由`google::CheckOpString(_result)`生成的）组成一个字符串输入一个流中；  
（3）将该流的句柄通过stream\(\)方法返回，以便用户继续向该流中记录其他用来详细描述本次比较结果的信息。

接下来再跟踪第三层GetReferenceableValue：

```c
// Function is overloaded for integral types to allow static const
// integrals declared in classes and not defined to be used as arguments to
// CHECK* macros. It's not encouraged though.
template <class T>
inline const T&       GetReferenceableValue(const T&           t) { return t; }
inline char           GetReferenceableValue(char               t) { return t; }
inline unsigned char  GetReferenceableValue(unsigned char      t) { return t; }
inline signed char    GetReferenceableValue(signed char        t) { return t; }
inline short          GetReferenceableValue(short              t) { return t; }
inline unsigned short GetReferenceableValue(unsigned short     t) { return t; }
inline int            GetReferenceableValue(int                t) { return t; }
inline unsigned int   GetReferenceableValue(unsigned int       t) { return t; }
inline long           GetReferenceableValue(long               t) { return t; }
inline unsigned long  GetReferenceableValue(unsigned long      t) { return t; }
inline long long      GetReferenceableValue(long long          t) { return t; }
inline unsigned long long GetReferenceableValue(unsigned long long t) {
  return t;
}
```

#### 1.2.



