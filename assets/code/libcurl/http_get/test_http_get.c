#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <curl/curl.h>

#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "test_debug_macros.h"

/**
 * libcurl库函数返回成功一般是 CURLE_OK, man中明确表示该值等价于0
 */

#define NUM_LOOP (1)


// 响应HTTP报文体处理函数
size_t body_callback(void *ptr, size_t size, size_t nmemb, void *data)
{
    size_t total = size * nmemb;
    struct Buffer *buf = (struct Buffer *)data;
    CHK_ERR(writeBuffer(buf, ptr, total));

    return total;
}

// 响应HTTP报文头处理函数
size_t header_callback(char *buffer, size_t size, size_t nitems, void *userdata)
{
    size_t total = size * nitems;
    struct Buffer *buf = (struct Buffer *)userdata;
    CHK_ERR(writeBuffer(buf, buffer, total));

    return total;
}

int test_http_get(const char *serv_ip, int serv_port, const char *serv_pth)
{
    CHK_NIL(serv_ip);
    CHK_NIL(serv_pth);

    struct Buffer *header_buf = NULL;  //请求结果的保存格式
    CHK_ERR(createBuffer(&header_buf));
    struct Buffer *body_buf = NULL;  //请求结果的保存格式
    CHK_ERR(createBuffer(&body_buf));

    CHK_ERR(curl_global_init(CURL_GLOBAL_ALL));

    int cnt = 0;
    while (cnt < NUM_LOOP) {
        CURL *curl = NULL;
        curl = curl_easy_init();
        if(NULL == curl) {
            fprintf(stdout, "Init CURL failed...\n");
            return -1;
        }

        // 设置参数
        CHK_ERR(curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L)); //请 求超时时长
        CHK_ERR(curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30L)); // 连接超时时长 
        CHK_ERR(curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L)); // 允许重定向
        CHK_ERR(curl_easy_setopt(curl, CURLOPT_HEADER, 0L));  // 若启用，会将头文件的信息作为数据流输出
        CHK_ERR(curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L)); // 关闭中断信号响应
        //CHK_ERR(curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L)); // 启用时, 即使设置了返回报文处理回调函数仍然会屏幕输出详细信息

        // 处理响应报文体
        CHK_ERR(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, body_callback));  //得到请求结果后的回调函数
        CHK_ERR(curl_easy_setopt(curl, CURLOPT_WRITEDATA, body_buf));

        // 处理响应报文头
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buf); // 提供write_res的第四个参数

        struct curl_slist *pList = NULL;
        pList = curl_slist_append(pList,"Accept-Encoding:gzip, deflate, sdch"); 
        pList = curl_slist_append(pList,"Accept-Language:zh-CN,zh;q=0.8"); 
        pList = curl_slist_append(pList,"Connection:keep-alive");
        CHK_ERR(curl_easy_setopt(curl, CURLOPT_HTTPHEADER, pList)); 

        char url[512];
        snprintf(url, 512, "http://%s:%d/%s", serv_ip, serv_port, serv_pth);
        CHK_ERR(curl_easy_setopt(curl, CURLOPT_URL, url)); //需要获取的URL地址

        //执行请求
        CHK_ERR(curl_easy_perform(curl));

        // 判断响应是否正确
        long res_code = 0;
        CHK_ERR(curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &res_code));
        if (res_code != 200 && res_code != 201) {
            fprintf(stderr, "HTTP ret_code not 2xx\n");
        }

        // 即使返回报文不是2xx任然可以查看返回报文头和报文体
        fprintf(stdout, "HTTP ret code: %ld\n", res_code);
        fprintf(stdout, "HTTP Response header:\n%s\n", header_buf->buf);
        fprintf(stdout, "HTTP Response body:\n%s\n", body_buf->buf);

        // 释放本次通讯相关资源
        curl_slist_free_all(pList); 
        curl_easy_cleanup(curl);

        ++cnt;
    }

    CHK_ERR(destroyBuffer(header_buf));
    CHK_ERR(destroyBuffer(body_buf));
    curl_global_cleanup();

    return 0;
}

int main()
{
    //CHK_ERR(test_http_get("10.230.135.31", 8008, "B302"));
    CHK_ERR(test_http_get("www.baidu.com", 80, ""));
    return 0;
}