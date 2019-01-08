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

#include "test_debug_macros.h"

int cnt_header = 0;
//int cnt_body = 0;

static const char *m_rmc_req = 
"{"
	"\"esa\":	{"
		"\"appId\":	\"BDC201708_03\","
		"\"instanceId\":	\"e2f34972503244d4a8f940f57349d221\","
		"\"encryptedData\":	\"EB351BA378D4BC73122254A78157E5C5\","
		"\"provinceCode\":	\"99\","
		"\"channelCode\":	\"ATM\","
		"\"esaVersion\":	\"ACC_C_LIN_1.7.0\","
		"\"consumerIP\":	\"10.230.147.31\""
	"},"
	"\"version\":	0"
"}";

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
    

static size_t write_body(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    size_t total = size * nmemb;
    //memcpy(userdata, ptr, total);
    struct Buffer *buf = (struct Buffer *)userdata;
    CHK_ERR(writeBuffer(buf, ptr, total));

    return total;
}

static size_t write_header(char *buffer, size_t size, size_t nitems, void *userdata)
{
    size_t total = size * nitems;
    struct Buffer *buf = (struct Buffer *)userdata;
    CHK_ERR(writeBuffer(buf, buffer, total));

    char *tmp = calloc(total + 1, sizeof(char));
    if (tmp == NULL) {
        fprintf(stderr, "tmp calloc error\n");
        return -1;
    }
    memcpy(tmp, buffer, total);
    fprintf(stdout, "===========\nheader cnt = %d, data:\n%s\n==========\n", cnt_header, tmp);
    free(tmp);
    ++cnt_header;

    return total;
}
 
int main(void)
{
  CURL *curl;
  CURLcode res;

    struct Buffer *body = NULL;
    CHK_ERR(createBuffer(&body));
    struct Buffer *header = NULL;
    CHK_ERR(createBuffer(&header));
 
  /* In windows, this will init the winsock stuff */ 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* get a curl handle */ 
  curl = curl_easy_init();
  //fprintf(stdout, "111\n");
  if(curl) {
    //fprintf(stdout, "222\n");
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */ 
    //curl_easy_setopt(curl, CURLOPT_URL, "http://postit.example.com/moo.cgi");
    //curl_easy_setopt(curl, CURLOPT_URL, "http://itable.abc");
    curl_easy_setopt(curl, CURLOPT_URL, "http://10.230.135.55:8001/rmc/checkversion.action");
    //fprintf(stdout, "333\n");

    /* Now specify the POST data */ 
    //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=daniel&project=curl");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, m_rmc_req);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_body); // 将返回报文使用write_res处理
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, body); // 提供write_res的第四个参数

    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_header);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, header); // 提供write_res的第四个参数

    curl_easy_setopt(curl, CURLOPT_HEADER, 1L); // 屏幕输出时会输出header
    //fprintf(stdout, "444\n");
 
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    //fprintf(stdout, "555\n");

    /* Check for errors */ 
    //fprintf(stdout, "666\n");
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    //fprintf(stdout, "777\n");
    /* always cleanup */ 
    curl_easy_cleanup(curl);
    //fprintf(stdout, "888\n");
  }
  curl_global_cleanup();

  fprintf(stdout, "响应报文header:\n%s\n", header->buf);
  fprintf(stdout, "响应报文body:\n%s\n", body->buf);

  CHK_ERR(destroyBuffer(header));
  CHK_ERR(destroyBuffer(body));

  return 0;
}