/**
 * 进程间互斥锁的典型风险: 进程意外终止锁未被正确释放导致死锁, 使用条件变量进一步是这种风险恶化
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <errno.h>

#include "test_debug_macros.h"
#include "protected_shm.h"

struct SHM
{
    int key; // 共享内存描述符
    char *mem; // 共享内存空间
    int size; // 共享内存大小, 单位: 字节
    pthread_mutex_t *lock; // 进程同步锁
    pthread_cond_t *cond; // 进程同步条件变量
};

// 初始化进程间互斥锁
static int initLock(pthread_mutex_t **lock)
{
    CHK_NIL(lock);

    pthread_mutex_t *lock_tmp = mmap(NULL, sizeof(pthread_mutex_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
    CHK_ERR((lock_tmp != MAP_FAILED)? 0: 1);

    pthread_mutexattr_t lock_attr;
    CHK_ERR(pthread_mutexattr_init(&lock_attr)); //~necessary, or weird EINVAL error occurs when operating on the mutex
    CHK_ERR(pthread_mutexattr_setpshared(&lock_attr, PTHREAD_PROCESS_SHARED));
    CHK_ERR(pthread_mutex_init(lock_tmp, &lock_attr));
    CHK_ERR(pthread_mutexattr_destroy(&lock_attr));
    *lock = lock_tmp;
    return 0;
}

// 初始化进程间条件变量
static int initCond(pthread_cond_t **cond)
{
    pthread_cond_t *cond_tmp = mmap(NULL, sizeof(pthread_cond_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
    CHK_ERR((cond_tmp != MAP_FAILED)? 0: 1);

    pthread_condattr_t cond_attr;
    CHK_ERR(pthread_condattr_init(&cond_attr));
    CHK_ERR(pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED));
    CHK_ERR(pthread_cond_init(cond_tmp, &cond_attr));
    CHK_ERR(pthread_condattr_destroy(&cond_attr));

    *cond = cond_tmp;
    return 0;
}

// 初始化
int createSHM(struct SHM **shm, int size)
{
    struct SHM *shm_tmp = NULL;
    CHK_NIL((shm_tmp = calloc(1, sizeof(struct SHM))));

    // 创建共享内存对象
    int shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666);
    CHK_ERR((shmid != -1)? 0: 1);

    shm_tmp->key = shmid;
    shm_tmp->size = size;
    CHK_ERR(initLock(&(shm_tmp->lock)));
    CHK_ERR(initCond(&(shm_tmp->cond)));

    // 共享内存挂载到进程内存空间
    char *mem_tmp = shmat(shm_tmp->key, NULL, 0);
    CHK_ERR((mem_tmp != (void *) -1)? 0: 1);
    shm_tmp->mem = mem_tmp;

    *shm = shm_tmp;
    return 0;
}

//
static int wq(struct SHM *shm, const char *q)
{
    // 写入
    int len = strlen(q);
    CHK_ERR((len + 1 < shm->size)? 0: 1);
    memcpy(shm->mem + 1, q, len);
    shm->mem[0] = 'Q';

    return 0;
}

// 初始化第一个问题
int writeQueryToSHM(struct SHM *shm, const char *q)
{
    CHK_NIL(shm);
    CHK_NIL(q);

    // 加锁
    CHK_ERR(pthread_mutex_lock(shm->lock));
    int error = wq(shm, q);
    if (error != 0) {
        ERROR_MSG("wq failed, error.\n");
    }

    // 释放锁
    CHK_ERR(pthread_cond_broadcast(shm->cond));
    CHK_ERR(pthread_mutex_unlock(shm->lock));

    if (error != 0) {
        return RAISE_ERROR();
    }

    return 0;
}

//
static int ra_wq(char *a, int len_a, struct SHM *shm, const char *q)
{
    // 读取
    int len = strlen(shm->mem);
    CHK_ERR((len < len_a)? 0: 1);
    memcpy(a, shm->mem + 1, len - 1);
    a[len] = '\0';

    // 写入
    len = strlen(q);
    CHK_ERR((len + 1 < shm->size)? 0: 1);
    memcpy(shm->mem + 1, q, len);
    shm->mem[len + 1] = '\0';
    shm->mem[0] = 'Q';

    return 0;
}

// 提问者: 读取答案并写入下一个问题
int readAnswerFromAndWriteQueryToSHM(char *a, int len_a, struct SHM *shm, const char *q)
{
    CHK_NIL(a);
    CHK_NIL(shm);
    CHK_NIL(q);

    // 加锁
    CHK_ERR(pthread_mutex_lock(shm->lock));
    while (shm->mem[0] != 'A') {
        CHK_ERR(pthread_cond_wait(shm->cond, shm->lock));
    }

    int error = ra_wq(a, len_a, shm, q);
    if (error != 0) {
        ERROR_MSG("ra_wq failed, error.\n");
    }

    // 释放锁
    CHK_ERR(pthread_cond_broadcast(shm->cond));
    CHK_ERR(pthread_mutex_unlock(shm->lock));

    if (error != 0) {
        return RAISE_ERROR();
    }

    return 0;
}

//
static int rq_wa(char *a, int len_a, struct SHM *shm, const char *q)
{
    // 读取
    int len = strlen(shm->mem);
    CHK_ERR((len < len_a)? 0: 1);
    memcpy(a, shm->mem + 1, len - 1);
    a[len] = '\0';

    // 写入
    len = strlen(q);
    CHK_ERR((len + 1 < shm->size)? 0: 1);
    memcpy(shm->mem + 1, q, len);
    shm->mem[len + 1] = '\0';
    shm->mem[0] = 'A';

    return 0;
}

// 解答者: 读取问题并写入答案
int readQueryFromAndWriteAnswerToSHM(char *a, int len_a, struct SHM *shm, const char *q)
{
    CHK_NIL(a);
    CHK_NIL(shm);
    CHK_NIL(q);

    // 加锁
    CHK_ERR(pthread_mutex_lock(shm->lock));
    while (shm->mem[0] != 'Q') {
        CHK_ERR(pthread_cond_wait(shm->cond, shm->lock));
    }

    int error = rq_wa(a, len_a, shm, q);
    if (error != 0) {
        ERROR_MSG("rq_wa failed, error.\n");
    }

    // 释放锁
    CHK_ERR(pthread_cond_broadcast(shm->cond));
    CHK_ERR(pthread_mutex_unlock(shm->lock));

    if (error != 0) {
        return RAISE_ERROR();
    }

    return 0;
}

// 销毁
int destroySHMDeep(struct SHM *shm)
{
    if (shm != NULL) {
        // 分离共享内存
        CHK_ERR(shmdt(shm->mem));
        // 删除共享内存. 不保证返回时共享内存被删除, 当共享内存引用次数为0时系统执行删除
        CHK_ERR((shmctl(shm->key, IPC_RMID, (struct shmid_ds *)NULL) != -1)? 0: 1);
        // 销毁锁
        CHK_ERR(pthread_mutex_destroy(shm->lock));
        // 删除锁mmap共享内存
        CHK_ERR(munmap(shm->lock, sizeof(pthread_mutex_t)));
        // 销毁条件变量
        CHK_ERR(pthread_cond_destroy(shm->cond));
        // 删除条件变量mmap共享内存
        CHK_ERR(munmap(shm->cond, sizeof(pthread_cond_t)));
    }
    free(shm);

    return 0;
}

// 销毁
int destroySHM(struct SHM *shm)
{
    free(shm);

    return 0;
}