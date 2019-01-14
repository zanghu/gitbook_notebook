#pragma once

struct SHM;

// 初始化
int createSHM(struct SHM **shm, int size);

// 初始化第一个问题
int writeQueryToSHM(struct SHM *shm, const char *q);

// 提问者: 读取答案并写入下一个问题
int readAnswerFromAndWriteQueryToSHM(char *a, int len_a, struct SHM *shm, const char *q);

// 解答者: 读取问题并写入答案
int readQueryFromAndWriteAnswerToSHM(char *a, int len_a, struct SHM *shm, const char *q);

// 销毁共享内存
int destroySHMDeep(struct SHM *shm);

// 销毁共享内存
int destroySHM(struct SHM *shm);
