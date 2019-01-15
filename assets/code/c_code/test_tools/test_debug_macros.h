#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>


//#define _DEBUG_MODE
#define _SHOW_THREAD_ID

#define TEST_ERROR (1)


#ifdef _SHOW_THREAD_ID
  #define SHOW_THREAD_DEBUG() fprintf(stdout, "线程: %lu, ", pthread_self())
  #define SHOW_THREAD_ERROR() fprintf(stderr, "线程: %lu, ", pthread_self())
#else
  #define SHOW_THREAD_DEBUG() 
  #define SHOW_THREAD_ERROR()  
#endif

// GCC if optimizer
// c++编译器下最好用!!(x)将x转为其自身的布尔值
#ifdef __GNUC__
  #ifdef __cplusplus
    #define _LIKELY(x) __builtin_expect(!!(x), 1)
    #define _UNLIKELY(x) __builtin_expect(!!(x), 0)
  #else
    #define _LIKELY(x) __builtin_expect((x), 1)
    #define _UNLIKELY(x) __builtin_expect((x), 0)
  #endif
#else
  #define _LIKELY(x) (x)
  #define _UNLIKELY(x) (x)
#endif

// 一、消息宏=========================

// 1.1.DEBUG_MSG(内部宏, 目前只有当前文件的加解锁部分还在使用, 不建议外部用户调用)=========================

// 最基本的声明
#if ((defined _DEBUG_MODE) && (defined _DEBUG_LOCAL))
// __VA_ARGS__前面加上##后, 当...是空参数时, 会自动去掉前面的逗号, 使得具备兼容输入0个参数的能力
  #define DEBUG_MSG(fmt, ...) SHOW_THREAD_DEBUG();fprintf(stdout, fmt, ##__VA_ARGS__);fflush(stdout)
  #define DEBUG_MSG_0(msg) SHOW_THREAD_DEBUG();fprintf(stdout, msg);fflush(stdout)
  #define DEBUG_MSG_1(msg1, msg2) SHOW_THREAD_DEBUG();fprintf(stdout, msg1, msg2);fflush(stdout)
  #define DEBUG_MSG_2(msg1, msg2, msg3) SHOW_THREAD_DEBUG();fprintf(stdout, msg1, msg2, msg3);fflush(stdout)
  #define DEBUG_MSG_3(msg1, msg2, msg3, msg4) SHOW_THREAD_DEBUG();fprintf(stdout, msg1, msg2, msg3, msg4);fflush(stdout)
  #define DEBUG_MSG_4(msg1, msg2, msg3, msg4, msg5) SHOW_THREAD_DEBUG();fprintf(stdout, msg1, msg2, msg3, msg4, msg5);fflush(stdout)
  #define DEBUG_MSG_5(msg1, msg2, msg3, msg4, msg5, msg6) SHOW_THREAD_DEBUG();fprintf(stdout, msg1, msg2, msg3, msg4, msg5, msg6);fflush(stdout)
  #define DEBUG_MSG_6(msg1, msg2, msg3, msg4, msg5, msg6, msg7) SHOW_THREAD_DEBUG();fprintf(stdout, msg1, msg2, msg3, msg4, msg5, msg6, msg7);fflush(stdout)
  #define DEBUG_MSG_7(msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8) SHOW_THREAD_DEBUG();fprintf(stdout, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8);fflush(stdout)
#else
  #define DEBUG_MSG_0(msg) 
  #define DEBUG_MSG_1(msg1, msg2) 
  #define DEBUG_MSG_2(msg1, msg2, msg3)
  #define DEBUG_MSG_3(msg1, msg2, msg3, msg4)
  #define DEBUG_MSG_4(msg1, msg2, msg3, msg4, msg5)
  #define DEBUG_MSG_5(msg1, msg2, msg3, msg4, msg5, msg6)
  #define DEBUG_MSG_6(msg1, msg2, msg3, msg4, msg5, msg6, msg7)
  #define DEBUG_MSG_7(msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8)
#endif

// 1.2.error_msg=========================

// 测试时应该总是打印错误信息到屏幕, 所以不存在开关_ERR_MODE的问题
// __VA_ARGS__前面加上##后, 当...是空参数时, 会自动去掉前面的逗号, 使得具备兼容输入0个参数的能力
#define ERROR_MSG(fmt, ...) SHOW_THREAD_ERROR();fprintf(stderr, fmt, ##__VA_ARGS__);fflush(stderr)
#define ERROR_MSG_0(msg) SHOW_THREAD_ERROR();fprintf(stderr, msg);fflush(stderr)
#define ERROR_MSG_1(msg1, msg2) SHOW_THREAD_ERROR();fprintf(stderr, msg1, msg2);fflush(stderr)
#define ERROR_MSG_2(msg1, msg2, msg3) SHOW_THREAD_ERROR();fprintf(stderr, msg1, msg2, msg3);fflush(stderr)
#define ERROR_MSG_3(msg1, msg2, msg3, msg4) SHOW_THREAD_ERROR();fprintf(stderr, msg1, msg2, msg3, msg4);fflush(stderr)
#define ERROR_MSG_4(msg1, msg2, msg3, msg4, msg5) SHOW_THREAD_ERROR();fprintf(stderr, msg1, msg2, msg3, msg4, msg5);fflush(stderr)
#define ERROR_MSG_5(msg1, msg2, msg3, msg4, msg5, msg6) SHOW_THREAD_ERROR();fprintf(stderr, msg1, msg2, msg3, msg4, msg5, msg6);fflush(stderr)
#define ERROR_MSG_6(msg1, msg2, msg3, msg4, msg5, msg6, msg7) SHOW_THREAD_ERROR();fprintf(stderr, msg1, msg2, msg3, msg4, msg5, msg6, msg7);fflush(stderr)
#define ERROR_MSG_7(msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8) SHOW_THREAD_ERROR();fprintf(stderr, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8);fflush(stderr)



// 二、数值检查宏=========================

#ifdef _SHOW_CALL_LEVEL
  #define FUNC_START() DEBUG_MSG_1("Start Function %s++++++++++\n", __FUNCTION__)
  #define FUNC_FINISH() DEBUG_MSG_1("Finish Function %s----------\n", __FUNCTION__)
  #define FUNC_ERROR() DEBUG_MSG_1("Error finish Function %s----------\n", __FUNCTION__)
#else
  #define FUNC_START()
  #define FUNC_FINISH() 
  #define FUNC_ERROR() 
#endif


// 2.1.值非零报错
#define CHK_ERR(val) \
{ \
    int _chk_err_res = (val); \
    if (_UNLIKELY(_chk_err_res != 0)) { \
        ERROR_MSG("In file %s at line %d, %s() catch error: %d from %s, throw error: %d\n", __FILE__,  __LINE__, __FUNCTION__,  _chk_err_res, #val,  TEST_ERROR); \
        FUNC_ERROR(); \
        return TEST_ERROR; \
    } \
}


// 2.2.值为空报错
#define CHK_NIL(val) \
{ \
    if (_UNLIKELY((val) == NULL)) { \
        ERROR_MSG("NULL ptr Error in %s at line %d, var %s=NULL in %s(), throw error: %d\n", \
        __FILE__, __LINE__, #val, __FUNCTION__,  TEST_ERROR); \
        FUNC_ERROR(); \
        return  TEST_ERROR; \
    } \
} 


// 2.3.值为非空报错
#define CHK_NOT_NIL(val) \
{ \
    if (_UNLIKELY((!(val)) == 0)) { \
        ERROR_MSG("Not NULL ptr Error in %s at line %d, function: %s, var: %s, throw error: %d\n", \
        __FILE__, __LINE__, __FUNCTION__, #val,  TEST_ERROR); \
        FUNC_ERROR(); \
        return  TEST_ERROR; \
    } \
}


// 2.4.线程函数内专用

// 2.4.1
#define CHK_NIL_PT(val) \
{ \
    if (_UNLIKELY((val) == 0)) { \
        ERROR_MSG("NULL ptr Error in %s at line %d, var %s=NULL in %s(), ERR_MSG: ", \
                     __FILE__, __LINE__, #val, __FUNCTION__); \
                     FUNC_ERROR(); \
                     pthread_exit("1"); \
    } \
}


// 2.4.2
#define CHK_ERR_PT(val) \
{ \
    int check_error_res = (val); \
    if (_UNLIKELY(check_error_res != 0)) { \
        ERROR_MSG("In file %s at line %d, %s() catch error: %d from %s, pthread_exit(\"1\")\n",__FILE__,  __LINE__, __FUNCTION__,  check_error_res, #val);  \
        FUNC_ERROR(); \
        pthread_exit("1"); \
    } \
}

// 2.4.3
#define CHK_NOT_NIL_P(val) \
{ \
    if (_UNLIKELY((val) != 0)) { \
        ERROR_MSG("Not NULL ptr Error in %s at line %d, var %s=NULL in %s(), ERR_MSG: ", \
                     __FILE__, __LINE__, #val, __FUNCTION__); \
                     FUNC_ERROR(); \
                     pthread_exit("1"); \
    } \
}




// 四、锁相关=========================

// 4.1.加锁, 带超时功能
#define MUTEX_TIMEDLOCK(p_lock, timeout_sec) \
{ \
    struct timespec timeout_tmp; \
    CHK_ERR(clock_gettime(CLOCK_REALTIME, &timeout_tmp), ACC749_BASE_FUNC_FAILURE); \
    timeout_tmp.tv_sec += (timeout_sec); \
    DEBUG_MSG("mutex准备加锁==========++++++++++\n"); \
    int _err = pthread_mutex_timedlock((p_lock), &timeout_tmp); \
    if (_UNLIKELY(_err != 0)) { \
        if (_err == ETIMEDOUT) { \
            fprintf(stderr, "!!!线程: %lu, %s line %d in %s 获取锁超时, 跳过本次写日志, error.\n", pthread_self(), __FUNCTION__, __LINE__, __FILE__); \
        } \
        else { \
            fprintf(stderr, "线程: %lu, %s line %d in %s 获取超时锁失败, 跳过本次写日志, error.\n", pthread_self(), __FUNCTION__, __LINE__, __FILE__); \
        } \
        FUNC_ERROR(); \
        return _err; \
    } \
    else { \
        DEBUG_MSG("mutex完成加锁==========++++++++++\n"); \
    } \
}

// 4.2.解锁
#define MUTEX_UNLOCK(p_lock) \
    DEBUG_MSG("mutex准备解锁==========----------\n"); \
    CHK_ERR(pthread_mutex_unlock((p_lock)), TEST_ERROR); \
    DEBUG_MSG("mutex完成解锁==========----------\n")



// 五、其他=========================
#ifdef _SHOW_CALL_LEVEL
  #define RAISE_ERROR() _raise_error_with_call_level_test(TEST_ERROR, __FUNCTION__, __FILE__, __LINE__)
#else
  #define RAISE_ERROR() _raise_error_without_call_level_test(TEST_ERROR, __FUNCTION__, __FILE__, __LINE__)
#endif



// 主动引发异常, 有函数结尾标志行
static inline int _raise_error_with_call_level_test(int code, const char *func, const char *const file, int const line)
{
    ERROR_MSG("In file %s at line %d, %s(), throw error: %d\n", file, line, func, code);

    return code;
}


// 主动引发异常, 无函数结尾标志行
static inline int _raise_error_without_call_level_test(int code, const char *func, const char *const file, int const line)
{
    ERROR_MSG("In file %s at line %d, %s(), throw error: %d\n", file, line, func, code);

    return code;
}


// 六、简易日志模块

#ifdef _ENABLE_SIMPLE_LOG

#include "simple_wr.h"

#define WRITE_LOG(...) \
{ \
    char buf[512]; \
    if (_UNLIKELY(snprintf(buf, 512, __VA_ARGS__) >= 512)) { \
        fprintf(stderr, "LOG BUFFEROVERFLOW\n"); \
    } \
    if (_UNLIKELY(writeStringToPath("log.txt", buf, "a") != 0)) { \
        fprintf(stderr, "WRITE_LOG failed, error.\n"); \
    } \
}

#endif

// 共131个
static const char *_errno_detail[] = 
{
"NOT USED ERRNO 0",
"NOT USED ERRNO 1", 
"NOT USED ERRNO 2", 
"NOT USED ERRNO 3", 
"NOT USED ERRNO 4", 
"NOT USED ERRNO 5", 
"NOT USED ERRNO 6", 
"NOT USED ERRNO 7", 
"NOT USED ERRNO 8", 
"NOT USED ERRNO 9", 
"NOT USED ERRNO 10", 
"NOT USED ERRNO 11", 
"NOT USED ERRNO 12", 
"NOT USED ERRNO 13", 
"NOT USED ERRNO 14", 
"NOT USED ERRNO 15", 
"NOT USED ERRNO 16", 
"NOT USED ERRNO 17", 
"NOT USED ERRNO 18", 
"NOT USED ERRNO 19", 
"NOT USED ERRNO 20", 
"NOT USED ERRNO 22", 
"NOT USED ERRNO 22", 
"NOT USED ERRNO 23", 
"NOT USED ERRNO 24", 
"NOT USED ERRNO 25", 
"NOT USED ERRNO 26", 
"NOT USED ERRNO 27", 
"NOT USED ERRNO 28", 
"NOT USED ERRNO 29", 
"NOT USED ERRNO 30", 
"NOT USED ERRNO 33", 
"NOT USED ERRNO 33", 
"NOT USED ERRNO 33", 
"NOT USED ERRNO 34", 
"EDEADLK(35): Resource deadlock would occur ",
"ENAMETOOLONG(36): File name too long ",
"ENOLCK(37): No record locks available ",
"ENOSYS(38): Function not implemented ",
"ENOTEMPTY(39): Directory not empty ",
"NOT USED ERRNO 41",
"ELOOP(40): Too many symbolic links encountered ",
"ENOMSG(42): No message of desired type ",
"EIDRM(43): Identifier removed ",
"ECHRNG(44): Channel number out of range ",
"EL2NSYNC(45): Level 2 not synchronized ",
"EL3HLT(46): Level 3 halted ",
"EL3RST(47): Level 3 reset ",
"ELNRNG(48): Link number out of range ",
"EUNATCH(49): Protocol driver not attached ",
"ENOCSI(50): No CSI structure available ",
"EL2HLT(51): Level 2 halted ",
"EBADE(52): Invalid exchange ",
"EBADR(53): Invalid request descriptor ",
"EXFULL(54): Exchange full ",
"ENOANO(55): No anode ",
"EBADRQC(56): Invalid request code ",
"EBADSLT(57): Invalid slot ",
"NOT USED ERRNO 58",
"EBFONT(59): Bad font file format ",
"ENOSTR(60): Device not a stream ",
"ENODATA(61): No data available ",
"ETIME(62): Timer expired ",
"ENOSR(63): Out of streams resources ",
"ENONET(64): Machine is not on the network ",
"ENOPKG(65): Package not installed ",
"EREMOTE(66): Object is remote ",
"ENOLINK(67): Link has been severed ",
"EADV(68): Advertise error ",
"ESRMNT(69): Srmount error ",
"ECOMM(70): Communication error on send ",
"EPROTO(71): Protocol error ",
"EMULTIHOP(72): Multihop attempted ",
"EDOTDOT(73): RFS specific error ",
"EBADMSG(74): Not a data message ",
"EOVERFLOW(75): Value too large for defined data type ",
"ENOTUNIQ(76): Name not unique on network ",
"EBADFD(77): File descriptor in bad state ",
"EREMCHG(78): Remote address changed ",
"ELIBACC(79): Can not access a needed shared library ",
"ELIBBAD(80): Accessing a corrupted shared library ",
"ELIBSCN(81): .lib section in a.out corrupted ",
"ELIBMAX(82): Attempting to link in too many shared libraries ",
"ELIBEXEC(83): Cannot exec a shared library directly ",
"EILSEQ(84): Illegal byte sequence ",
"ERESTART(85): Interrupted system call should be restarted ",
"ESTRPIPE(86): Streams pipe error ",
"EUSERS(87): Too many users ",
"ENOTSOCK(88): Socket operation on non-socket ",
"EDESTADDRREQ(89): Destination address required ",
"EMSGSIZE(90): Message too long ",
"EPROTOTYPE(91): Protocol wrong type for socket ",
"ENOPROTOOPT(92): Protocol not available ",
"EPROTONOSUPPORT(93): Protocol not supported ",
"ESOCKTNOSUPPORT(94): Socket type not supported ",
"EOPNOTSUPP(95): Operation not supported on transport endpoint ",
"EPFNOSUPPORT(96): Protocol family not supported ",
"EAFNOSUPPORT(97): Address family not supported by protocol ",
"EADDRINUSE(98): Address already in use ",
"EADDRNOTAVAIL(99): Cannot assign requested address ",
"ENETDOWN(100): Network is down ",
"ENETUNREACH(101): Network is unreachable ",
"ENETRESET(102): Network dropped connection because of reset ",
"ECONNABORTED(103): Software caused connection abort ",
"ECONNRESET(104): Connection reset by peer ",
"ENOBUFS(105): No buffer space available ",
"EISCONN(106): Transport endpoint is already connected ",
"ENOTCONN(107): Transport endpoint is not connected ",
"ESHUTDOWN(108): Cannot send after transport endpoint shutdown ",
"ETOOMANYREFS(109): Too many references: cannot splice ",
"ETIMEDOUT(110): Connection timed out ",
"ECONNREFUSED(111): Connection refused ",
"EHOSTDOWN(112): Host is down ",
"EHOSTUNREACH(113): No route to host ",
"EALREADY(114): Operation already in progress ",
"EINPROGRESS(115): Operation now in progress ",
"ESTALE(116): Stale NFS file handle ",
"EUCLEAN(117): Structure needs cleaning ",
"ENOTNAM(118): Not a XENIX named type file ",
"ENAVAIL(119): No XENIX semaphores available ",
"EISNAM(120): Is a named type file ",
"EREMOTEIO(121): Remote I/O error ",
"EDQUOT(122): Quota exceeded ",
"ENOMEDIUM(123): No medium found ",
"EMEDIUMTYPE(124): Wrong medium type ",
"ECANCELED(125): Operation Canceled ",
"ENOKEY(126): Required key not available ",
"EKEYEXPIRED(127): Key has expired ",
"EKEYREVOKED(128): Key has been revoked ",
"EKEYREJECTED(129): Key was rejected by service ",
"EOWNERDEAD(130): Owner died ",
"ENOTRECOVERABLE(131): State not recoverable ",
"ERFKILL(132): Operation not possible due to RF-kill ",
"EHWPOISON(133): Memory page has hardware error ",
"NOT USED ERRNO > 133"
};

static inline const char *_getErrnoMsg(unsigned int err_idx)
{
    if (err_idx < 133) {
        return _errno_detail[err_idx];
    }
    return _errno_detail[err_idx];
}

#define ERRNO_DETAIL(x) _getErrnoMsg(x)