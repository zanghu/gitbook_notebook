#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "debug_macros.h"
#include "exceptions.h"
#include <sys/time.h>
#ifdef _AIX
#include "time_utility.h"
#endif

#define _SHOW_DETAIL

#define _TIME_SHM

// 生成key
static int genShmSysvKey(key_t *key, const char *pathname, int proj_id)
{
    CHK_NIL(key);
    CHK_NIL(pathname);
    AST_ERR((proj_id > 0)? 0: 1);
    AST_ERR((proj_id <= 255)? 0: 1);

    // 获取key
    //const char *pathname = "config/acc.xml";
    //int proj_id =  127; // int的低八位且不能为0, 1~255, AIX大端系统可能有所不同
    key_t shm_key = ftok(pathname, proj_id);
    if (shm_key == -1) {
        int err = errno;
        ERR_MSG("ftok failed, pathname: %s, proj_id: %d, detail: %s, error.\n", pathname, proj_id, ERRNO_DETAIL(err));
        return RAISE_ERROR(ESB305_FTOK_FAILURE);
    }
#ifdef _SHOW_DETAIL
    fprintf(stdout, "shm_key生成成功, key: %d\n", shm_key);
#endif

    *key = shm_key;
    return 0;
}

// 创建共享内存. perm是权限参数, 例如: IPC_CREAT | IPC_EXCL | 0600; size单位: 字节
int allocShmSysv(int *shmid, const char *pathname, int proj_id, int perm, int size)
{
    CHK_NIL(shmid);
    //CHK_NIL(pathname);
    //AST_ERR((proj_id > 0)? 0: 1);
    //AST_ERR((proj_id <= 255)? 0: 1);
    AST_ERR((size > 0)? 0: 1);

    key_t shm_key = 0;
    CHK_ERR(genShmSysvKey(&shm_key, pathname, proj_id));
 
    // 获取共享内存
    //int id = shmget(shm_key, 32 * 1024 * 1024, IPC_CREAT | IPC_EXCL | perm); // 如果已创建则失败
    int id = shmget(shm_key, size, IPC_CREAT | IPC_EXCL | perm); // 如果已创建则失败
    if (id == -1) {
        int err = errno;
        ERR_MSG("shmget failed, detail: %s, error.\n", ERRNO_DETAIL(err));
        return RAISE_ERROR(ESB301_SHMGET_FAILURE);
    }
#ifdef _SHOW_DETAIL
    fprintf(stdout, "shmget共享内存分配成功, shm_id = %d\n", id);
#endif
    *shmid = id;
    return 0;
}

// 释放共享内存
int freeShmSysv(int shmid)
{
    // 释放共享内存
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        int err = errno;
        ERR_MSG("shmctl failed, detail: %s, error.\n", ERRNO_DETAIL(err));
        return RAISE_ERROR(ESB303_SHMCTL_FAILURE);
    }
#ifdef _SHOW_DETAIL
    fprintf(stdout, "shmctl共享内存释放成功, shm_id = %d\n", shmid);
#endif
    return 0;
}

// 连接共享内存
int connectShmSysv(void **mem, int shmid)
{
    CHK_NIL(mem);

    void *mem_tmp = shmat(shmid, NULL, 0);
    if (mem_tmp == (void *)(-1)) {
        int err = errno;
        ERR_MSG("shmat failed, detail: %s, error.\n", ERRNO_DETAIL(err));
        return RAISE_ERROR(ESB302_SHMAT_FAILURE);
    }
#ifdef _SHOW_DETAIL
    fprintf(stdout, "shmat共享内存地址连接成功, shmid = %d, shm addr: %p\n", shmid, mem_tmp);
#endif
    *mem = mem_tmp;
    return 0;
}

// 断开共享内存
int detachShmSysv(void *mem)
{
    CHK_NIL(mem);

    if (shmdt(mem) != 0) {
        int err = errno;
        ERR_MSG("shmdt failed, detail: %s, error.\n", ERRNO_DETAIL(err));
        return RAISE_ERROR(ESB304_SHMDT_FAILURE);
    }
#ifdef _SHOW_DETAIL
    fprintf(stdout, "shmdt共享内存断开成功\n");
#endif
    return 0;
}

/*
int main()
{
    //int shmid;
    //CHK_ERR(test_sysv_shm_alloc(&shmid));
    //CHK_ERR(test_sysv_shm_free(shmid));

    CHK_ERR(test_sysv_shm_free(18579501));
    return 0;
}
*/