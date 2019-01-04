#ifndef TEST_DEBUG_MACROS_H_
#define TEST_DEBUG_MACROS_H_

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

// 一、消息宏=========================

// 1.1.DEBUG_MSG=========================

// 最基本的声明
#if ((defined _DEBUG_MODE) && (defined _DEBUG_LOCAL))
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
    if (_chk_err_res != 0) { \
        ERROR_MSG_6("In file %s at line %d, %s() catch error: %d from %s, throw error: %d\n", __FILE__,  __LINE__, __FUNCTION__,  _chk_err_res, #val,  TEST_ERROR); \
        FUNC_ERROR(); \
        return TEST_ERROR; \
    } \
}


// 2.2.值为空报错
#define CHK_NIL(val) \
{ \
    if ((val) == NULL) { \
        ERROR_MSG_5("Null pointer Error in %s at line %d, var %s=NULL in %s(), throw error: %d\n", \
        __FILE__, __LINE__, #val, __FUNCTION__,  TEST_ERROR); \
        FUNC_ERROR(); \
        return  TEST_ERROR; \
    } \
} 


// 2.3.值为非空报错
#define CHK_NOT_NIL(val) \
{ \
    if ((!(val)) == 0) { \
        ERROR_MSG_5("Not Null pointer Error in %s at line %d, function: %s, var: %s, throw error: %d\n", \
        __FILE__, __LINE__, __FUNCTION__, #val,  TEST_ERROR); \
        FUNC_ERROR(); \
        return  TEST_ERROR; \
    } \
}


// 2.4.值非零报错, 返回NULL, 专门为返回指针的函数设计
#define CHK_SUITE(val) \
{ \
    int _chk_err_res = (val); \
    if (_chk_err_res != 0) { \
        ERROR_MSG_6("In file %s at line %d, %s() catch error: %d from %s, throw error: %d\n", __FILE__,  __LINE__, __FUNCTION__,  _chk_err_res, #val,  TEST_ERROR); \
        FUNC_ERROR(); \
        return NULL; \
    } \
}


// 2.5.线程函数内专用

#define CHK_NIL_PT(val) \
{ \
    if ((val) == NULL) { \
        ERROR_MSG_4("Null pointer Error in %s at line %d, var %s=NULL in %s(), ERR_MSG: ", \
                     __FILE__, __LINE__, #val, __FUNCTION__); \
                     FUNC_ERROR(); \
                     pthread_exit("1"); \
    } \
}


#define CHK_ERR_PT(val) \
{ \
    int check_error_res = (val); \
    if (check_error_res != 0) { \
        ERROR_MSG_5("In file %s at line %d, %s() catch error: %d from %s, pthread_exit(\"1\")\n",__FILE__,  __LINE__, __FUNCTION__,  check_error_res, #val);  \
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
    DEBUG_MSG_0("mutex准备加锁==========++++++++++\n"); \
    int _err = pthread_mutex_timedlock((p_lock), &timeout_tmp); \
    if (_err != 0) { \
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
        DEBUG_MSG_0("mutex完成加锁==========++++++++++\n"); \
    } \
}

// 4.2.解锁
#define MUTEX_UNLOCK(p_lock) \
    DEBUG_MSG_0("mutex准备解锁==========----------\n"); \
    CHK_ERR(pthread_mutex_unlock((p_lock)), TEST_ERROR); \
    DEBUG_MSG_0("mutex完成解锁==========----------\n")



// 五、其他=========================
#ifdef _SHOW_CALL_LEVEL
  #define RAISE_ERROR() _raise_error_with_call_level_test(TEST_ERROR, __FUNCTION__, __FILE__, __LINE__)
#else
  #define RAISE_ERROR() _raise_error_without_call_level_test(TEST_ERROR, __FUNCTION__, __FILE__, __LINE__)
#endif



// 主动引发异常, 有函数结尾标志行
static inline int _raise_error_with_call_level_test(int code, const char *func, const char *const file, int const line)
{
    ERROR_MSG_4("In file %s at line %d, %s(), throw error: %d\n", file, line, func, code);

    return code;
}


// 主动引发异常, 无函数结尾标志行
static inline int _raise_error_without_call_level_test(int code, const char *func, const char *const file, int const line)
{
    ERROR_MSG_4("In file %s at line %d, %s(), throw error: %d\n", file, line, func, code);

    return code;
}


// 六、简易日志模块

#ifdef _ENABLE_SIMPLE_LOG

#include "simple_wr.h"

#define WRITE_LOG(...) \
{ \
    char buf[512]; \
    if (snprintf(buf, 512, __VA_ARGS__) >= 512) { \
        fprintf(stderr, "LOG BUFFEROVERFLOW\n"); \
    } \
    if (writeStringToPath("log.txt", buf, "a") != 0) { \
        fprintf(stderr, "WRITE_LOG failed, error.\n"); \
    } \
}

#endif

#endif