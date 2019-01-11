#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test_debug_macros.h"
#include "buffer.h"

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