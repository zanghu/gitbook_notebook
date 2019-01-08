#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <curl/curl.h>

#include <stdlib.h>
#include <string.h>

#include "test_debug_macros.h"


// 回调函数写入的缓冲区对象
struct Buffer
{
    char *buf;
    size_t len;
    size_t size;
};

#define BUFFER_INIT_SIZE (1024)
#define BUFFER_EXPAND_FACTOR (2)

int createBuffer(struct Buffer **buf)
{
    CHK_NIL(buf);

    struct Buffer *buf_tmp = NULL;
    CHK_NIL((buf_tmp = calloc(1, sizeof(struct Buffer))));
    buf_tmp->len = 0;
    buf_tmp->size = BUFFER_INIT_SIZE;
    CHK_NIL((buf_tmp->buf = calloc(BUFFER_INIT_SIZE, sizeof(char))));
    *buf = buf_tmp;
    return 0;
}

int writeBuffer(struct Buffer *buf, char *src, size_t len)
{
    CHK_NIL(buf);
    CHK_NIL(src);

    if (len == 0) {
        return 0;
    }

    if (len + buf->len >= buf->size) {
        CHK_ERR((buf->size > 0)? 0: 1);
        size_t size_new = buf->size * 2;
        while (size_new <= len + buf->len) {
            size_new *= BUFFER_EXPAND_FACTOR;
        }
        char *tmp = NULL;
        CHK_NIL((tmp = calloc(size_new, sizeof(char))));
        memcpy(tmp, buf->buf, buf->len);
        free(buf->buf);
        buf->buf = tmp;
        buf->size = size_new;
    }

    CHK_ERR((buf->size > len + buf->len)? 0: 1);
    memcpy(buf->buf + buf->len, src, len);
    buf->len += len;

    return 0;
}

int destroyBuffer(struct Buffer *buf)
{
    if (buf != NULL) {
        free(buf->buf);
        buf->len = 0;
        buf->size = 0;
    }
    free(buf);
    return 0;
}

size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
    size_t total = size * nmemb;
    struct Buffer *buf = (struct Buffer *)data;
    CHK_ERR(writeBuffer(buf, ptr, total));

    return total;
}

int test_http_get(const char *serv_ip, int serv_port, const char *serv_pth)
{
    CHK_NIL(serv_ip);
    CHK_NIL(serv_pth);

    CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
    if(CURLE_OK != res) {
        fprintf(stdout, "curl init failed\n");
        return 1;
    }

    CURL *pCurl = NULL;
    pCurl = curl_easy_init();
    if(NULL == pCurl) {
        fprintf(stdout, "Init CURL failed...\n");
        return -1;
    }

    CHK_ERR(curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 30L)); //请 求超时时长
    CHK_ERR(curl_easy_setopt(pCurl, CURLOPT_CONNECTTIMEOUT, 30L)); // 连接超时时长 
    CHK_ERR(curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1L)); // 允许重定向
    CHK_ERR(curl_easy_setopt(pCurl, CURLOPT_HEADER, 0L));  // 若启用，会将头文件的信息作为数据流输出
    CHK_ERR(curl_easy_setopt(pCurl, CURLOPT_NOSIGNAL, 1L)); // 关闭中断信号响应
    //CHK_ERR(curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 1L)); // 启用时, 即使设置了返回报文处理回调函数仍然会屏幕输出详细信息

    struct Buffer *buf = NULL;  //请求结果的保存格式
    CHK_ERR(createBuffer(&buf));
    CHK_ERR(curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback));  //得到请求结果后的回调函数
    CHK_ERR(curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, buf));

    struct curl_slist *pList = NULL;
    pList = curl_slist_append(pList,"Accept-Encoding:gzip, deflate, sdch"); 
    pList = curl_slist_append(pList,"Accept-Language:zh-CN,zh;q=0.8"); 
    pList = curl_slist_append(pList,"Connection:keep-alive");
    CHK_ERR(curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, pList)); 

    char url[512];
    snprintf(url, 512, "http://%s:%d/%s", serv_ip, serv_port, serv_pth);
    CHK_ERR(curl_easy_setopt(pCurl, CURLOPT_URL, url)); //需要获取的URL地址
    //执行请求
    if(curl_easy_perform(pCurl) == CURLE_OK) {
        // 判断响应是否正确
        long res_code=0;
        if (curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &res_code) == CURLE_OK) {
            fprintf(stderr, "curl_easy_getinfo failed\n");
        }
        if (res_code == 200 || res_code == 201) {
            fprintf(stdout, "HTTP success\n");
            fprintf(stdout, "HTTP Response:\n%s\n", buf->buf);
        }
        else {
            fprintf(stderr, "HTTP ret_code not 2xx\n");
        }
    }
    else {
        fprintf(stderr, "HTTP failed\n");
    }

    CHK_ERR(destroyBuffer(buf));

    curl_slist_free_all(pList); 
    curl_easy_cleanup(pCurl);
    curl_global_cleanup();

    return 0;
}

int main()
{
    CHK_ERR(test_http_get("10.230.135.31", 8008, "B302"));
    return 0;
}