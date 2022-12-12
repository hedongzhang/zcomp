/*
 * Date: 2022-12-11 16:15:53
 * Author: hdzhang<hdzhang920@163.com>
 * Description: 
*/

#include <zlib.h>
#include <string.h>

#include "alg_zlib.h"

static int compress_level;      //Z_DEFAULT_COMPRESSION
// static z_stream comp_stream;
// static z_stream decomp_stream;

int zlib_init(int level)
{
        // int ret;

        compress_level = level;

        // memset(&comp_stream, 0, sizeof(comp_stream));
        // memset(&decomp_stream, 0, sizeof(decomp_stream));

        // ret = deflateInit(&comp_stream, level);
        // if (ret != Z_OK) 
        //         goto failed;

        // ret = inflateInit(&decomp_stream);
        // if (ret != Z_OK) 
        //         goto failed1;

        return 0;
// failed1:
//         deflateEnd(&comp_stream);
// failed:
//         return -1;
}

int zlib_free()
{
        // deflateEnd(&comp_stream);
        // deflateEnd(&decomp_stream);
        return 0;
}

int zlib_comp(void* dst, size_t dstCapacity, const void* src, size_t srcSize)
{
        int ret;

        // comp_stream.avail_in = srcSize;
        // comp_stream.next_in = (Byte*)src;
        // comp_stream.avail_out = dstCapacity;
        // comp_stream.next_out = dst;

        // ret = deflate(&comp_stream, Z_NO_FLUSH);
        // if(ret != Z_OK)
        //         return -1;

        // return dstCapacity - comp_stream.avail_out;

        ret = compress2(dst, &dstCapacity, src, srcSize, compress_level);
        if(ret != Z_OK)
                return -1;
        return dstCapacity;
}

int zlib_decomp(void* dst, size_t dstCapacity, const void* src, size_t compressedSize)
{
        int ret;

        // decomp_stream.avail_in = compressedSize;
        // decomp_stream.next_in = (Byte*)src;
        // decomp_stream.avail_out = dstCapacity;
        // decomp_stream.next_out = dst;

        // ret = inflate(&comp_stream, Z_NO_FLUSH);
        // if(ret != Z_OK)
        //         return -1;

        // return dstCapacity - decomp_stream.avail_out;

        ret = uncompress(dst, &dstCapacity, src, compressedSize);
        if(ret != Z_OK)
                return -1;
        return dstCapacity;
}