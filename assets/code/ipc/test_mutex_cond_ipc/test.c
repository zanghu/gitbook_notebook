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
#include <sys/wait.h>

#include "test_debug_macros.h"
#include "protected_shm.h"

#define NUM_LOOP (100)


int main() 
{
    // 创建共享内存
    struct SHM *shm = NULL;
    CHK_ERR(createSHM(&shm, 512));

    // 初始化第一个问题
    CHK_ERR(writeQueryToSHM(shm, "Qquery 0"));
    char a[256];
    char q[256];
    //~ here the fork
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } 
    else {
        if (pid > 0) { //~ parent
            int cnt = 0;
            while (cnt < NUM_LOOP) {
                snprintf(q, 256, "query %d", cnt);
                CHK_ERR(readAnswerFromAndWriteQueryToSHM(a, 256, shm, q)); // 从共享内存读取答案，之后写入下一个问题
                fprintf(stdout, "parent, got answer[%d]: %s\n", cnt, a); 
                ++cnt;
            }
            fprintf(stdout, "parent: waiting child...\n");
            CHK_ERR((wait(NULL) != -1)? 0: 1);
            CHK_ERR(destroySHMDeep(shm));
            fprintf(stdout, "parent: finish\n");
            exit(EXIT_SUCCESS);
        }
        else { // child
            int cnt = 0;
  
            while (cnt < NUM_LOOP) {
                snprintf(a, 256, "answer %d", cnt);
                CHK_ERR(readQueryFromAndWriteAnswerToSHM(q, 256, shm, a)); // 从共享内存读取问题，之后写入答案
                fprintf(stdout, "child, recv query[%d]: %s\n", cnt, q);
                ++cnt;
            }
            CHK_ERR(destroySHM(shm));
            exit(EXIT_SUCCESS);
        }
    }

    return 0;
}
