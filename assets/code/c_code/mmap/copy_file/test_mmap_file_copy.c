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

#define COPYINCR (1024*1024*1024)	/* 1 GB */

int copy_file(const char *src_path, const char *dst_path)
{
    CHK_NIL(src_path);
    CHK_NIL(dst_path);

    // 打开原始文件
    int fdin;
	if ((fdin = open(src_path, O_RDONLY)) < 0) {
		ERROR_MSG("can't open %s for reading", src_path);
        return RAISE_ERROR();
    }

    // 打开目标文件
    int fdout;
	if ((fdout = open(dst_path, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0) {
		ERROR_MSG("can't open %s for reading", dst_path);
        return RAISE_ERROR();
    }

    // 计算原始文件长度
	struct stat	sbuf;
	CHK_ERR(fstat(fdin, &sbuf));

    // 重设目标文件大小等于原始文件
	CHK_ERR(ftruncate(fdout, sbuf.st_size));

    // 逐段拷贝文件
	size_t		copysz;
	off_t		fsz = 0;
	void		*src, *dst;
	while (fsz < sbuf.st_size) {
		if ((sbuf.st_size - fsz) > COPYINCR) {
			copysz = COPYINCR;
        }
		else {
			copysz = sbuf.st_size - fsz;
        }

        // mmap建立映射关系
		if ((src = mmap(0, copysz, PROT_READ, MAP_SHARED, fdin, fsz)) == MAP_FAILED) {
			ERROR_MSG("mmap error for input");
            return RAISE_ERROR();
        }
		if ((dst = mmap(0, copysz, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, fsz)) == MAP_FAILED) {
			ERROR_MSG("mmap error for output");
            return RAISE_ERROR();
        }

        // 执行拷贝
		memcpy(dst, src, copysz);

        // munmap解除映射关系, 注意munmap并不保证立即将写入内容刷新到文件
		CHK_ERR(munmap(src, copysz));
		CHK_ERR(munmap(dst, copysz));
		fsz += copysz;
	}

    // 关闭文件描述符
    CHK_ERR(close(fdin));
    CHK_ERR(close(fdout));

	return 0;
}

int main()
{
    struct timeval t0;
    struct timeval t1;
    CHK_ERR(gettimeofday(&t0, NULL));
    CHK_ERR(copy_file("/home/zanghu/pylearn2_data/stl10_matlab/unlabeled.mat", "/home/zanghu/copy_file")); // 文件大小约2.4G
    CHK_ERR(gettimeofday(&t1, NULL));
    fprintf(stdout, "time elapsed: %lus\n", t1.tv_sec - t0.tv_sec);

    return 0;
}