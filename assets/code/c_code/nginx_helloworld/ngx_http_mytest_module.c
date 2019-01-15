#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


// 静态函数声明====================

static char *ngx_http_mytest(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_int_t ngx_http_mytest_handler(ngx_http_request_t *r);

/*
// 在nginx.conf中加入
server {
    ...
    location /test {
        mytest;
    }
    ...
}

// 访问网站时，浏览器输入: http://10.230.168.28:8010/test
// nginx检查到http://10.230.168.28:8010后面的uri是/test, 就会调用ngx_command_t对象ngx_http_mytest_commands来处理请求
// 显示网页上有nginx
*/

// 处理配置项的数组, 数组的每个元素对应于对一类配置项的处理
static ngx_command_t ngx_http_mytest_commands[] = 
{
    {
        ngx_string("mytest"), // 在编译好nginx后, 运行期配置文件nginx.conf中需要为某个url指向该名字
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LMT_CONF | NGX_CONF_NOARGS,
        ngx_http_mytest, // 使用该方法来处理配置项中的参数
        NGX_HTTP_LOC_CONF_OFFSET,
        0,
        NULL
    },

    ngx_null_command
};


// 模块上下文
static ngx_http_module_t ngx_http_mytest_module_ctx = 
{
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};


// 新模块定义
ngx_module_t ngx_http_mytest_module = 
{
    NGX_MODULE_V1,
    &ngx_http_mytest_module_ctx,
    ngx_http_mytest_commands,
    NGX_HTTP_MODULE,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NGX_MODULE_V1_PADDING
};


// 配置项对应的回调函数, 配置项参数处理句柄
static char *ngx_http_mytest(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf;

    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);

    clcf->handler = ngx_http_mytest_handler;

    return NGX_CONF_OK;
}



// 实际完成处理的回调函数
static ngx_int_t ngx_http_mytest_handler(ngx_http_request_t *r)
{
    // 必须是GET或者HEAD方法, 否则返回405 Not Allowed
    if (!(r->method & (NGX_HTTP_GET | NGX_HTTP_HEAD))) {
        return NGX_HTTP_NOT_ALLOWED;
    }

    // 丢弃请求中的包体
    ngx_int_t rc = ngx_http_discard_request_body(r);
    if (rc != NGX_OK) {
        return rc;
    }

    // 设置返回的Content-Type
    ngx_str_t type = ngx_string("text/plain");
    // 返回的包体内容
    ngx_str_t response = ngx_string("Hello World!");
    // 设置返回状态码
    r->headers_out.status = NGX_HTTP_OK;
    // 响应包是有包体内容的, 需要设置Content-Length长度
    r->headers_out.content_length_n = response.len;
    // 设置Content-Type
    r->headers_out.content_type = type;

    // 发送HTTP头部
    rc = ngx_http_send_header(r);
    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {
        return rc;
    }

    // 构造ngx_buf_t结构体准备发送包体
    ngx_buf_t *b;
    b = ngx_create_temp_buf(r->pool, response.len);
    if (b == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    // 将Hello World复制到ngx_buf_t指向的内存中
    ngx_memcpy(b->pos, response.data, response.len);
    // 注意, 一定要设置好last指针
    b->last = b->pos + response.len;
    // 声明这时最后一块缓冲区
    b->last_buf = 1;

    // 构造发送时的ngx_chain_t结构体
    ngx_chain_t out;
    // 复制ngx_buf_t
    out.buf = b;
    // 设置next为NULL
    out.next = NULL;

    // 最后一步为发送包体, 发送结束后HTTP框架会调用ngx_http_finalize_request方法结束请求
    return ngx_http_output_filter(r, &out);
}