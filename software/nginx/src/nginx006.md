## Nginx源码学习：配置文件相关常量的定义

参考资料: [https://github.com/vislee/leevis.com/issues/99](https://github.com/vislee/leevis.com/issues/99)

### 概述
ngx配置文件指令解析是相关模块通过定义一个ngx_command_s数组实现的。这个结构体ngx_command_s有一个成员变量type，类型是ngx_uint_t。

### 类型定义
先说下这个整形的定义：ngx是这么定义的，typedef uintptr_t ngx_uint_t;
而uintptr_t是定义在<stdint.h>这个头文件中，也是通过typedef定义的，实际上是在64位机器上是unsigned long int，在32位机器上是unsigned int。 也即是说64位系统是8个字节64bit，32位系统是4个字节32bit。

### 类型取值
在ngx中这个type定义了这个指令可以配置的范围（NGX_MAIN_CONF，NGX_HTTP_MAIN_CONF等），指令的类型（NGX_DIRECT_CONF，NGX_CONF_BLOCK等），指令参数个数（NGX_CONF_NOARGS，NGX_CONF_TAKE12，NGX_CONF_1MORE等。）并且它们之间通过与操作同时起作用。

### 定义
每一个flag都是通过define定义了一个32bit的数字，用16进制表示的话，每一位仅允许1、2、4、8。
```c
#define NGX_CONF_NOARGS      0x00000001
#define NGX_CONF_TAKE1       0x00000002
#define NGX_CONF_TAKE2       0x00000004
#define NGX_CONF_TAKE12      (NGX_CONF_TAKE1|NGX_CONF_TAKE2)

#define NGX_CONF_BLOCK       0x00000100
#define NGX_CONF_FLAG        0x00000200
#define NGX_CONF_ANY         0x00000400
#define NGX_CONF_1MORE       0x00000800
#define NGX_MAIN_CONF        0x01000000
#define NGX_ANY_CONF         0x1F000000


#define NGX_HTTP_MAIN_CONF        0x02000000
#define NGX_HTTP_SRV_CONF         0x04000000
#define NGX_HTTP_LOC_CONF         0x08000000
#define NGX_HTTP_UPS_CONF         0x10000000
```

### 其他常量的定义
模块定义： 该flag没有与操作，就是对应模块的字母倒序的ascii码，如：核心模块0x45524F43就是EROC的ascii码。
```c
#define NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#define NGX_CONF_MODULE      0x464E4F43  /* "CONF" */
#define NGX_EVENT_MODULE      0x544E5645  /* "EVNT" */
#define NGX_HTTP_MODULE           0x50545448   /* "HTTP" */
#define NGX_MAIL_MODULE         0x4C49414D     /* "MAIL" */
#define NGX_STREAM_MODULE       0x4d525453     /* "STRM" */
```