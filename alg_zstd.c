#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <zstd.h>

#include "alg_zstd.h"

static int compress_level;      //ZSTD_CLEVEL_DEFAULT
static ZSTD_CCtx* cctx;
static ZSTD_DCtx* dctx;

static int use_dict;
static ZSTD_CDict* cdict;
static ZSTD_DDict* ddict;

int zstd_init(int level, int dict_on)
{
        int fd;
        char buff[DICT_SIZE];
        int size;

        compress_level = level;

        cctx = ZSTD_createCCtx();
        if(!cctx)
                goto failed1;
        dctx = ZSTD_createDCtx();
        if(!dctx)
                goto failed1;

        if(dict_on) {
                fd = open(DICT, O_RDONLY);
                if(fd == -1) {
                        printf("open dict file:%s failed:%d\n", DICT, errno);
                        goto failed1;
                }

                size = read(fd, buff, DICT_SIZE);
                if(size < DICT_SIZE) {
                        printf("read dict file:%s failed:%d\n", DICT, errno);
                        goto failed2;
                }
                close(fd);

                cdict = ZSTD_createCDict(buff, size, compress_level);
                ddict = ZSTD_createDDict(buff, size);
                use_dict = 1;
        }

        return 0;
failed2:
        if(fd)
                close(fd);
failed1:
        if(cctx)
                ZSTD_freeCCtx(cctx);
        if(dctx)
                ZSTD_freeDCtx(dctx);
        if(cdict)
                ZSTD_freeCDict(cdict);
        if(ddict)
                ZSTD_freeDDict(ddict);
        return -1;
}

int zstd_free()
{
        if(cctx)
                ZSTD_freeCCtx(cctx);
        if(dctx)
                ZSTD_freeDCtx(dctx);
        if(cdict)
                ZSTD_freeCDict(cdict);
        if(ddict)
                ZSTD_freeDDict(ddict);

        return 0;
}

int zstd_comp(void* dst, size_t dstCapacity, const void* src, size_t srcSize)
{
        size_t ret;
        extern int use_dict;

        if(!cctx)
                return -1;

        if(use_dict) {
                ret = ZSTD_compress_usingCDict(cctx, dst, dstCapacity, src, srcSize, cdict);
        } else {
                // ret = ZSTD_compress(dst, dstCapacity, src, srcSize, compress_level);
                ret = ZSTD_compressCCtx(cctx, dst, dstCapacity, src, srcSize, compress_level);
        }
        
        if(ZSTD_isError(ret)) {
                printf("zstd error is:%s\n", ZSTD_getErrorName(ret));
                return -1;
        }

        return ret;
}

int zstd_decomp(void* dst, size_t dstCapacity, const void* src, size_t compressedSize)
{
        size_t ret;
        extern int use_dict;

        if(!dctx)
                return -1;

        if(use_dict) {
                ret = ZSTD_decompress_usingDDict(dctx, dst, dstCapacity, src, compressedSize, ddict);
        } else {
                // ret = ZSTD_decompress(dst, dstCapacity, src, compressedSize);
                ret = ZSTD_decompressDCtx(dctx, dst, dstCapacity, src, compressedSize);
        }
        
        if(ZSTD_isError(ret))
                return -1;
                
        return ret;
}