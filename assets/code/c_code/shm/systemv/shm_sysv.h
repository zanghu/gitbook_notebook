#ifndef SHM_SYSV_H_
#define SHM_SYSV_H_


// 创建共享内存. perm是权限参数, 例如: IPC_CREAT | IPC_EXCL | 0600, 保守建议0644: 创建者wr, 其余r; 保险建议: 0777; size单位: 字节
int allocShmSysv(int *shmid, const char *pathname, int proj_id, int perm, int size);

// 释放共享内存
int freeShmSysv(int shmid);

// 连接共享内存
int connectShmSysv(void **mem, int shmid);

// 断开共享内存
int detachShmSysv(void *mem);

#endif