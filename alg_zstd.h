/*
 * Date: 2022-12-08 16:09:58
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/

/*
 * Date: 2022-12-09 18:27:00
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/

#ifndef __ALG_ZSTD_
#define __ALG_ZSTD_

#define DICT "./dict"
#define DICT_SIZE 512 * 16

int zstd_init(int level, int dict_on);

int zstd_free();

int zstd_comp(void* dst, size_t dstCapacity, const void* src, size_t srcSize);

int zstd_decomp(void* dst, size_t dstCapacity, const void* src, size_t compressedSize);

#endif
