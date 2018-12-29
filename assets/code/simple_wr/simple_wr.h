#pragma once

#include <stdio.h>



// 1.字符串操作=========================

// 1.1.字符串读操作=========================

// 将文件指针全部内容读取为字符串, 读取模式为"rb"
int readStringFromFileSimple(char **file_str, FILE *file_in);

// 将路径文件全部内容读取为字符串, 读取模式为"rb"
int readStringFromPathSimple(char **file_str, const char *file_path);


// 1.2.字符串写操作=========================

// 将字符串写入文件指针, 写入模式为"wb"
int writeStringToFileSimple(FILE *fp, const char *msg);

// 将字符串写入文件路径, 写入模式为"wb"
int writeStringToPathSimple(const char *file_path, const char *msg);


// 2.字节流操作=========================

// 2.1.字节流读操作=========================

// 简化版的readFileToString，需要手动释放, 只适用于内容不包括'\0'的文档!!!
int readBytesFromFileSimple(char **buf, long *len, FILE *file_in);

// 简化版的readFileToStringFromPath，需要手动释放, 只适用于读取内容中不含有'\0'的文档!!!
int readBytesFromPathSimple(char **file_bytes, long *len_file, const char *file_path);

// 2.2.字节流写操作=========================

// 将指定长度字节流写入到制定的文件指针，要求文件已打开, 长度单位是字节
int writeBytesToFileSimple(FILE *fp, const char *msg, int size_t);

// 将指定长度字节流写入到指定路径的文件, 长度单位是字节. 省略了模式项, 默认使用wb
int writeBytesToPathSimple(const char *file_path, const char *msg, int size_t);