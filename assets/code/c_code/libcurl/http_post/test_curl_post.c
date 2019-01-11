/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2015, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/ 
/* <DESC>
 * simple HTTP POST using the easy interface
 * </DESC>
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "buffer.h"
#include "test_debug_macros.h"

#define NUM_LOOP (1)

// 处理HTTP响应报文体的回调函数
// 在一次HTTP通讯(一次curl_easy_perform函数执行过程)中该函数可能被调用多次, 原因包括HTTP 100或者多次TCP通讯 
size_t body_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    size_t total = size * nmemb;
    struct Buffer *buf = (struct Buffer *)userdata;
    CHK_ERR(writeBuffer(buf, ptr, total));

    return total;
}

// 处理HTTP响应报文头的回调函数, 在一次HTTP通讯(一次curl_easy_perform函数执行过程)中该函数可能被调用多次
// 在一次HTTP通讯(一次curl_easy_perform函数执行过程)中该函数可能被调用多次, 原因包括HTTP 100或者多次TCP通讯 
size_t header_callback(char *ptr, size_t size, size_t nitems, void *userdata)
{
    size_t total = size * nitems;
    struct Buffer *buf = (struct Buffer *)userdata;
    CHK_ERR(writeBuffer(buf, ptr, total));

    return total;
}

int test_http_post(const char *url, const char *req_body)
{
    CHK_NIL(url);

    struct Buffer *body_buf = NULL;
    CHK_ERR(createBuffer(&body_buf));
    struct Buffer *header_buf = NULL;
    CHK_ERR(createBuffer(&header_buf));

    /* In windows, this will init the winsock stuff */ 
    CHK_ERR(curl_global_init(CURL_GLOBAL_ALL));

    // 循环执行 NUM_LOOP 次 HTTP POST
    int cnt = 0;
    while (cnt < NUM_LOOP) {
        /* get a curl handle */ 
        CURL *curl = NULL;;

        CHK_NIL((curl = curl_easy_init()));

        // 设置URL
        CHK_ERR(curl_easy_setopt(curl, CURLOPT_URL, url));

        // 设置header
        struct curl_slist *header_list = NULL;
        CHK_NIL((header_list = curl_slist_append(header_list, "Expect:"))); // 很重要，因为curl默认对超过1024B的报文采用Expect:100, 但并不是所有服务方都能正确处理
        CHK_ERR(curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list));

        // 设置body
        CHK_ERR(curl_easy_setopt(curl, CURLOPT_POSTFIELDS, req_body));

        CHK_ERR(curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, body_callback)); // 将返回报文使用write_res处理
        CHK_ERR(curl_easy_setopt(curl, CURLOPT_WRITEDATA, body_buf)); // 提供write_res的第四个参数

        CHK_ERR(curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback));
        CHK_ERR(curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buf)); // 提供write_res的第四个参数
        //curl_easy_setopt(curl, CURLOPT_HEADER, 1L); // 屏幕输出时会输出header

        // 执行HTTP通讯
        CHK_ERR(curl_easy_perform(curl));

        // 处理响应报文
        long res_code = 0;
        CHK_ERR(curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &res_code));
        if (res_code != 200 && res_code != 201) {
            fprintf(stderr, "HTTP ret_code not 2xx\n");
        }

        // 即使返回报文不是2xx任然可以查看返回报文头和报文体
        fprintf(stdout, "HTTP ret code: %ld\n", res_code);
        fprintf(stdout, "响应报文header:\n%s\n", header_buf->buf);
        fprintf(stdout, "响应报文body:\n%s\n", body_buf->buf);

        // 释放本次通讯相关资源
        curl_slist_free_all(header_list);
        curl_easy_cleanup(curl);
        ++cnt;
    }

    curl_global_cleanup();

    CHK_ERR(destroyBuffer(header_buf));
    CHK_ERR(destroyBuffer(body_buf));

    return 0;
}

int main()
{
    const char *req = "";
    CHK_ERR(test_http_post("http://www.baidu.com/", req));

    return 0;
}