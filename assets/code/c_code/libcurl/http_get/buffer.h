#pragma once

// 回调函数写入的缓冲区对象
struct Buffer
{
    char *buf;
    size_t len;
    size_t size;
};

int createBuffer(struct Buffer **buf);

int writeBuffer(struct Buffer *buf, char *src, size_t len);

int destroyBuffer(struct Buffer *buf);