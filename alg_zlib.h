/*
 * Date: 2022-12-11 16:15:57
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/

#ifndef __ALG_ZLIB_
#define __ALG_ZLIB_

int zlib_init(int level);

int zlib_free();

int zlib_comp(void* dst, size_t dstCapacity, const void* src, size_t srcSize);

int zlib_decomp(void* dst, size_t dstCapacity, const void* src, size_t compressedSize);

#endif
