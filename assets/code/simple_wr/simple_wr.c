#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "test_debug_macros.h"

// 获取文件长度
static int getFileSize(long *file_size, FILE *fp)
{
    CHK_NIL(file_size);
    CHK_NIL(fp);

    long dist_to_file_head = ftell(fp); // 记录当前文件指针位置与文件头部距离
    CHK_ERR(fseek(fp, 0L, SEEK_END)); // 指针移动到文件尾部
    long file_size_tmp = ftell(fp);
    CHK_ERR((file_size_tmp != -1L)? 0: 1);
    CHK_ERR(fseek(fp, dist_to_file_head, SEEK_SET)); // 文件指针移动回原位
    *file_size = file_size_tmp;
    return 0;
}


// 1.字符读操作=========================

// 1.1.字符串读操作=========================

// 简化版的readFileToString，需要手动释放, 只适用于内容不包括'\0'的文档!!!
// 2017.06.23, 完成异常安全改造
int readStringFromFileSimple(char **buf, FILE *file_in)
{
    CHK_NIL(buf);
    CHK_NIL(file_in);

    int error = 0;

    char *buf_tmp = NULL;

    long file_len = -1;
    CHK_ERR(getFileSize(&file_len, file_in));

    CHK_NIL((buf_tmp = malloc(sizeof(char) * (file_len + 1))));

    size_t read_len = 0;
    do {
    read_len = fread(buf_tmp, sizeof(char), file_len, file_in);
    if (ferror(file_in) != 0) {
        error = TEST_ERROR;
        ERROR_MSG_0("读取结束后 ferror 检查发现文件指针有错误, error.\n");
        break;
    }
    if (read_len != file_len) {
        error = TEST_ERROR;
        ERROR_MSG_2("fread实际读取长度小于文件长度，file_len = %d, read_len = %d, error.\n", file_len, read_len);
        break;
    }
    buf_tmp[file_len] = '\0';

    // 检查文件中包含非法字符
    size_t len_str = strlen(buf_tmp);
    if (len_str != file_len) {
        ERROR_MSG_2("读到的文件字符串的文件长度是: %d, strlen()计算出长度是: %d, 二者不相等, 怀疑文件内容中包含'\\0'导致, error.\n", file_len, len_str);
        error = TEST_ERROR;
        break;
    }
    } while (0);

    if (error != 0) {
        free(buf_tmp);
        return RAISE_ERROR();
    }

    *buf = buf_tmp;

    return 0;
}

// 简化版的readFileToStringFromPath，需要手动释放, 只适用于读取内容中不含有'\0'的文档!!!
int readStringFromPathSimple(char **file_str, const char *file_path)
{
    CHK_NIL(file_str);
    CHK_NIL(file_path);

    int error = 0;

    FILE *file_in = NULL;
    if ((file_in = fopen(file_path, "rb")) == NULL) {
        fprintf(stderr, "file path: %s fopen failed, error.\n", file_path);
        return RAISE_ERROR();
    }

    char *buf = NULL;

    error = readStringFromFileSimple(&buf, file_in);
    if (error != 0) {
        ERROR_MSG_1("读取文件 %s 失败, error.\n", file_path);
    }
    CHK_ERR(fclose(file_in)); 

    if (error != 0) {
        return RAISE_ERROR();
    }

    *file_str = buf;
    return 0;
}


// 1.2.字符串写操作=========================

// 将字符串写入到指定的文件指针，要求文件已打开
int writeStringToFileSimple(FILE *fp, const char *msg)
{
    CHK_NIL(fp);
    CHK_NIL(msg);

    size_t len_msg = strlen(msg); 
    if (len_msg <= 0) {
        ERROR_MSG_1("不支持写入文件的字符串长度为 %d, error.\n", len_msg);
        return RAISE_ERROR();
    }

    size_t num_write = fwrite(msg, sizeof(char), len_msg, fp);
    CHK_ERR(ferror(fp)); // 对文件指针调用任何输入输出函数后，都应立即使用ferror检查
    if (num_write == len_msg) {
        ERROR_MSG_2("fwrite实际写入长度 %d 与 期望写入长度 %d 不一致, error.\n", num_write, error);
        return RAISE_ERROR();
    }
    return 0;
}

// 将字符串写入到指定路径的文件
// 省略了模式项, 默认使用w
int writeStringToPathSimple(const char *file_path, const char *msg)
{
    CHK_NIL(file_path);
    CHK_NIL(msg);

    int error = 0;

    FILE *fp = NULL;
    CHK_NIL((fp = fopen(file_path, "wb")));
    error = writeStringToFileSimple(fp, msg);
    CHK_ERR(fclose(fp));

    if (error != 0) {
        ERROR_MSG_1("写入文件 %s 失败, error.\n", file_path);
        return RAISE_ERROR(TEST_ERROR);
    }

    return 0;
}


// 1.3.字节流读操作=========================

// 简化版的readFileToString，需要手动释放, 只适用于内容不包括'\0'的文档!!!
int readBytesFromFileSimple(char **buf, long *len, FILE *file_in)
{
    CHK_NIL(buf);
    CHK_NIL(len);
    CHK_NIL(file_in);

    int error = 0;

    char *buf_tmp = NULL;

    long file_len = -1;
    CHK_ERR(getFileSize(&file_len, file_in));

    CHK_NIL((buf_tmp = malloc(sizeof(char) * (file_len + 1))));

    size_t read_len = 0;
    do {
    read_len = fread(buf_tmp, sizeof(char), file_len, file_in);
    if (read_len != file_len) {
        error = TEST_ERROR;
        ERROR_MSG_2("fread实际读取长度小于文件长度，file_len = %d, read_len = %d, error.\n", file_len, read_len);
        break;
    }
    buf_tmp[file_len] = '\0';

    if (ferror(file_in) != 0) {
        error = TEST_ERROR;
        ERROR_MSG_0("读取结束后 ferror 检查发现文件指针有错误, error.\n");
        break;
    }

    } while (0);

    if (error != 0) {
        free(buf_tmp);
        return RAISE_ERROR(TEST_ERROR);
    }

    *buf = buf_tmp;
    *len = file_len;

    return 0;
}

// 简化版的readFileToStringFromPath，需要手动释放, 只适用于读取内容中不含有'\0'的文档!!!
int readBytesFromPathSimple(char **file_bytes, long *len_file, const char *file_path)
{
    CHK_NIL(file_bytes);
    CHK_NIL(file_path);

    int error = 0;

    FILE *file_in = NULL;
    if ((file_in = fopen(file_path, "rb")) == NULL) {
        fprintf(stderr, "file path: %s fopen failed, error.\n", file_path);
        return TEST_ERROR;
    }

    char *buf = NULL;

    int len_tmp = 0;
    error = readBytesFromFileSimple(&buf, &len_tmp, file_in);
    if (error != 0) {
        ERROR_MSG_1("读取文件 %s 失败, error.\n", file_path);
    }
    if (fclose(file_in) ); 

    if (error != 0) {
        return RAISE_ERROR(TEST_ERROR);
    }

    *file_bytes = buf;
    *len_file = len_tmp;

    return 0;
}


// 1.4.字节流写操作=========================

// 将指定长度字节流写入到制定的文件指针，要求文件已打开, 长度单位是字节
int writeBytesToFileSimple(FILE *fp, const char *msg, size_t len)
{
    CHK_NIL(fp);
    CHK_NIL(msg);
    CHK_ERR((len >= 0)? 0: 1);


    int num_write = fwrite(msg, sizeof(char), len, fp);
    CHK_ERR(ferror(fp)); // 对文件指针调用任何输入输出函数后，都应立即使用ferror检查
    CHK_ERR((num_write == len)? 0: 1);

    return 0;
}


// 将指定长度字节流写入到指定路径的文件, 长度单位是字节
// 省略了模式项, 默认使用w
int writeBytesToPathSimple(const char *file_path, const char *msg, size_t len)
{
    CHK_NIL(file_path);
    CHK_NIL(msg);
    CHK_ERR((len >= 0)? 0: 1);

    int error = 0;

    FILE *fp = NULL;
    CHK_NIL((fp = fopen(file_path, "wb")));
    error = writeBytesToFileSimple(fp, msg, len);
    CHK_ERR(fclose(fp));

    if (error != 0) {
        ERROR_MSG_1("写入文件 %s 失败, error.\n", file_path);
        return RAISE_ERROR(TEST_ERROR);
    }

    return 0;
}