## 谷歌日志工具——glog

本文出现的源码及源码中的英文注释全部来自于`glog`的头文件`glog/logging.h`，部分代码中有本人添加的中文注释，所用`glog`本本是0.3.5。

glog源码中doc录下官方文档并不丰富，但是`glog/logging.h`头文件中的注释有较高参考价值。

### 1.逻辑表达式检查宏

本小节分析以下检查宏函数的实现原理以及实际做了什么。

| 宏函数名称 | 功能说明 |
| :---: | :---: |
| CHECK\_EQ\(val1, val2\) |  |
| CHECK\_NE\(val1, val2\) |  |
| CHECK\_LE\(val1, val2\) |  |
| CHECK\_LT\(val1, val2\) |  |
| CHECK\_GE\(val1, val2\) |  |
| CHECK\_GT\(val1, val2\) |  |

### 1.1.检查宏源码跟踪

* 检查宏定义

```cpp
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

上面的源码可见日志记录的关键是`CHECK_OP`宏函数。

* 跟踪第一层定义：`CHECK_OP`

```cpp
#define CHECK_OP(name, op, val1, val2) \
  CHECK_OP_LOG(name, op, val1, val2, google::LogMessageFatal)
```

上面的代码可见`CHECK_OP`就是`CHECK_OP_LOG`，接下来需要跟踪两个分支：`CHECK_OP_LOG`宏函数和`google::LogMessageFatal`，先跟踪后者。

* 跟踪第二层定义中的google::LogMessageFatal：

```cpp
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

可见`google::LogMessageFatal`是一个类名，其派生自`LogMessage`类。

* 跟踪第二层定义中的`CHECK_OP_LOG`：

```cpp
// CHECK_OP_LOG有三个#if...#else分支的定义，这里选取最简单的一个
typedef std::string _Check_string;
#define CHECK_OP_LOG(name, op, val1, val2, log)                         \
  while (google::_Check_string* _result =                \
         google::Check##name##Impl(                      \ //  形如Check_EQImpl，生成某种关于比较内容的消息
             google::GetReferenceableValue(val1),        \ // 将全局变量变量等编程临时变量
             google::GetReferenceableValue(val2),        \
             #val1 " " #op " " #val2))                                  \
    log(__FILE__, __LINE__,                                             \
        google::CheckOpString(_result)).stream()
```

* 这里又产生了三个跟踪分支：`Check##name##Impl`、`GetReferenceableValue`和`log.stream()`，这里先来看最后一个：

logMessage类的stream方法的实际定义并未出现在`glog/logging.h`中，但大体可以猜出`log(__FILE__, __LINE__, google::CheckOpString(_result)).stream()`的含义是：  
（1）创建一个`google::LogMessageFatal`类对象；  
（2）将调用当前日志语句的原文件的文件名（`__FILE__`）、行号（`__LINE__`）和比较（EQ、NE、LE等等）结果（应该是由`google::CheckOpString(_result)`生成的）组成一个字符串输入一个流中；  
（3）将该流的句柄通过`stream()`方法返回，以便用户继续向该流中记录其他用来详细描述本次比较结果的信息。

* 接下来再跟踪倒数第二个的`GetReferenceableValue`：

```cpp
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

上面代码的英文注释中已经说明这些函数的用途，不再赘述。

* 最后剩下的只需要跟踪`Check##name##Impl`：

在头文件`glog/logging.h`中搜索会发现其实找不到形如`Check_EQImpl`的定义，其实他们是宏定义通过函数模板自动生成的实例化的模板函数，其模板如下面的代码所示：

```cpp
// 函数模板的宏，每次调用该宏函数实质上实例化了一个具体模板函数定义
// Helper functions for CHECK_OP macro.
// The (int, int) specialization works around the issue that the compiler
// will not instantiate the template version of the function on values of
// unnamed enum type - see comment below.
#define DEFINE_CHECK_OP_IMPL(name, op) \
  template <typename T1, typename T2> \
  // 如果name是Check_EQ那么就定义了名为Check_EQImpl的函数 \
  inline std::string* name##Impl(const T1& v1, const T2& v2, \
                            const char* exprtext) { \
    if (GOOGLE_PREDICT_TRUE(v1 op v2)) return NULL; \ // 如果逻辑表达式为真，则不进行任何操作
    else return MakeCheckOpString(v1, v2, exprtext); \ // 逻辑表达式为假，则返回一个说明错误信息的“字符串”
  } \
  inline std::string* name##Impl(int v1, int v2, const char* exprtext) { \
    return name##Impl<int, int>(v1, v2, exprtext); \
  }

// 基于上面的模板实例化出若干个具体的模板函数
// We use the full name Check_EQ, Check_NE, etc. in case the file including
// base/logging.h provides its own #defines for the simpler names EQ, NE, etc.
// This happens if, for example, those are used as token names in a
// yacc grammar.
DEFINE_CHECK_OP_IMPL(Check_EQ, ==)  // Compilation error with CHECK_EQ(NULL, x)?
DEFINE_CHECK_OP_IMPL(Check_NE, !=)  // Use CHECK(x == NULL) instead.
DEFINE_CHECK_OP_IMPL(Check_LE, <=)
DEFINE_CHECK_OP_IMPL(Check_LT, < )
DEFINE_CHECK_OP_IMPL(Check_GE, >=)
DEFINE_CHECK_OP_IMPL(Check_GT, > )
#undef DEFINE_CHECK_OP_IMPL
```

这里实际上还可以继续跟踪`MakeCheckOpString(v1, v2, exprtext)`，但是从理解基本日志记录逻辑的角度看已经没有必要了。

#### 1.2.结论

可知下面的代码

```cpp
CHECK_EQ(val1, val2) << "两个值不相等，错误！";
```

等价于

```cpp
if (GOOGLE_PREDICT_TRUE(val1 == val2)) {} // 如果用于比较的逻辑表达式为真，则不进行任何操作
else {
    google::LogMessageFatal(__FILE__, __LINE__, \
      <基于逻辑比较内容(val1 == val2的错误信息描述>).stream() << "两个值不相等，错误！";
}
```

### 2.日志记录宏

#### 2.1.日志级别

glog提供四个级别的日志，具体如下：

| 日志级别 | 功能说明 |
| :---: | :---: |
| INFO |  |
| WARNING |  |
| ERROR |  |
| FATAL |  |

#### 2.2.日志记录宏

`glog`最基本、最常用的日志宏是`LOG(日志级别)`，其返回一个流句柄，用户可以使用该流句柄记录信息。

例如：

```cpp
  LOG(INFO) << "Running for " << FLAGS_iterations << " iterations.";
```

**注意**：其中FATAL等级的日志会在记录以后终止程序运行，要谨慎使用。

#### 2.3.日志记录宏源码分析

其宏定义位于`glog/log_severity.h`，内容如下是：

```cpp
// Variables of type LogSeverity are widely taken to lie in the range
// [0, NUM_SEVERITIES-1].  Be careful to preserve this assumption if
// you ever need to change their values or add a new severity.
typedef int LogSeverity;

// 四个日志级别，这里没有使用enum，而是使用了整型
const int GLOG_INFO = 0, GLOG_WARNING = 1, GLOG_ERROR = 2, GLOG_FATAL = 3,
  NUM_SEVERITIES = 4;
#ifndef GLOG_NO_ABBREVIATED_SEVERITIES
# ifdef ERROR
#  error ERROR macro is defined. Define GLOG_NO_ABBREVIATED_SEVERITIES before including logging.h. 
#  See the document for detail.
# endif
const int INFO = GLOG_INFO, WARNING = GLOG_WARNING,
  ERROR = GLOG_ERROR, FATAL = GLOG_FATAL;
#endif
```

### 3.代码示例

* test.cpp

```cpp
#include <iostream>
#include <gflags/gflags.h> // 为了调用关闭gflags的方法
#include <glog/logging.h>

int main(int argc, char** argv)
{
    // 所有级别日志除了输出到日志文件之外也同时输出到stderr, 默认只有ERROR和FATAL也输出到stderr
    FLAGS_alsologtostderr = 1;

    // 日志初始化
    ::google::InitGoogleLogging(argv[0]); // 初始化参数一般是第一个命令行参数--即程序的名称

    // 日志配置
    ::google::SetLogDestination(::google::INFO, "./my_log/glog_"); //参数1为日志级别，参数2表示输出目录及日志文件名前缀
    ::google::InstallFailureSignalHandler(); // CoreDump发生时会通过stderr输出堆栈信息
    //::google::InstallFailureWriter(&FatalMessageDump);

    // 记录日志
    LOG(INFO) << "I am INFO!";
    LOG(WARNING) << "I am WARNING!";
    LOG(ERROR) << "I am ERROR!";
    //LOG(FATAL) << "I am FATAL!"; // 会主动引发段错误

    // 关闭日志
    ::google::ShutdownGoogleLogging();
    ::google::ShutDownCommandLineFlags(); // 不关闭此语句会发生内存泄漏，不知道是不是glog的bug

    return 0;
}
```

* build.sh

```shell
#!/bin/sh
set -ex
g++ -g -Wall test.cpp -lglog -lgflags -o Test
```

* 运行结果（不含fatal日志）

屏幕输出：

![](/assets/google002_001.PNG)

日志文件内容：

![](/assets/google002_002.PNG)


### 4.常用参数配置

```cpp
logtostderr (bool, default=false)    //是否将所有日志输出到 stderr，而非文件

alsologtostderr(bool,default=false)　　//是否同时将日志输出到文件和stderr

minloglevel (int, default=google::INFO)　　//限制输出到 stderr 的部分信息，包括此错误级别和更高错误级别的日志信息 
stderrthreshold (int, default=google::ERROR)　　//除了将日志输出到文件之外，还将此错误级别和更高错误级别的日志同时输出到 stderr，这个只能使用 -stderrthreshold=1 或代码中设置，而不能使用环境变量的形式。（这个参数可以替代上面两个参数）

colorlogtostderr(bool, default=false)　　//将输出到 stderr 上的错误日志显示相应的颜色 

v (int, default=0)　　//只记录此错误级别和更高错误级别的 VLOG 日志信息

log_dir (string, default="")　　//设置日志文件输出目录 

v (int, default=0)　　//只有当自定义日志（VLOG）级别值小于此值时，才进行输出，默认为0(注：自定义日志的优
先级与GLOG内置日志优级相反，值越小优先级越高！！！)。

vmodule (string, default="") // 分文件（不包括文件名后缀，支持通配符）设置自定义日志的可输出级别，如：GLOG_vmodule=server=2,client=3 表示文件名为server.* 的只输出小于 2 的日志，文件名为 client.* 的只输出小于 3 的日志。如果同时使用 GLOG_v 选项，将覆盖 GLOG_v 选项。

```

