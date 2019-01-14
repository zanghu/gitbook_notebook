#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/time.h>

#include "test_debug_macros.h"
#include "md5.h"

#define COPYINCR (128*1024*1024)	/* 128MB */

int show_md5(const unsigned char *result)
{
    char md5[64];
    int sp_res = 0;
    int i = 0;
    for (i = 0; i < 16; ++i) {
        sp_res = sprintf((char *)(md5 + i * 2), "%02X", (int)(result[i]));
        CHK_ERR((sp_res == 2)? 0: 1);
    }
    md5[32] = '\0';
    fprintf(stdout, "ms5: %s\n", md5);
    return 0;
}

int md5_large_file(const char *src_path)
{
    CHK_NIL(src_path);

    unsigned char result[256];
    MD5_CTX ctx; // md5.h
    MD5_Init(&ctx); // 初始化

    // 打开原始文件
    int fd;
	if ((fd = open(src_path, O_RDONLY)) < 0) {
		ERROR_MSG("can't open %s for reading", src_path);
        return RAISE_ERROR();
    }

    // 计算原始文件长度
	struct stat	sbuf;
	CHK_ERR(fstat(fd, &sbuf));

    // 逐段读取文件并计算加入一段后的累积md5
	size_t		mem_size;
	off_t		offset = 0;
	void		*src = NULL;
    int cnt = 0;
	while (offset < sbuf.st_size) {
		if ((sbuf.st_size - offset) > COPYINCR) {
			mem_size = COPYINCR;
        }
		else {
			mem_size = sbuf.st_size - offset;
        }

        // mmap建立映射关系
		if ((src = mmap(0, mem_size, PROT_READ, MAP_SHARED, fd, offset)) == MAP_FAILED) {
			ERROR_MSG("mmap error for input");
            return RAISE_ERROR();
        }

        // 计算md5
        MD5_Update(&ctx, src, mem_size); // 执行加密

        // munmap解除映射关系, 注意munmap并不保证立即将写入内容刷新到文件
		CHK_ERR(munmap(src, mem_size));
		offset += mem_size;
        ++cnt;
	}
    MD5_Final(result, &ctx); // 获取加密结果
    fprintf(stdout, "md5摘要计算完毕, 共经历 %d 轮\n", cnt);
    CHK_ERR(show_md5(result));

	return 0;
}

int main()
{
    struct timeval t0;
    struct timeval t1;
    CHK_ERR(gettimeofday(&t0, NULL));
    CHK_ERR(md5_large_file("/home/zanghu/pylearn2_data/stl10_matlab/unlabeled.mat")); // 文件大小约2.4G
    //CHK_ERR(md5_large_file("/home/zanghu/123.jpg")); // 文件大小约2.4G
    CHK_ERR(gettimeofday(&t1, NULL));
    fprintf(stdout, "time elapsed: %lus\n", t1.tv_sec - t0.tv_sec);

    return 0;
}