#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <zlib.h>

#include "alg_zlib.h"
#include "statistic.h"


#define CHUNK 512 * 16
#define COMP_LEVEL Z_BEST_SPEED

#define HEAD_SIZE 4

int main(int argc, char *argv[])
{
        int ret = 0;
        int fd_in = -1;
        int fd_out = -1;
        char file_input[64] = {0};
        char file_output[64] = {0};
        int compress;

        size_t size = CHUNK;
        // char in[HEAD_SIZE + CHUNK * 2] = {0};
        // char out[HEAD_SIZE + CHUNK * 2] = {0};

        char *in = calloc(HEAD_SIZE + CHUNK * 2, 1);
        char *out = calloc(HEAD_SIZE + CHUNK * 2, 1);
        if(!in || !out) {
                ret = -1;
                goto final;
        }  

        char *buff_in = in + HEAD_SIZE;
        char *buff_out = out + HEAD_SIZE;

        if(argc != 2) {
                printf("参数不正确，例子: zlib file\n");
                return -1;
        }

        strncpy(file_input, argv[1], 64 - 6);
        
        if(strlen(file_input) > 5) {
                if(strcmp(file_input + strlen(file_input) - 5, ".zlib") == 0) {
                        compress = 0;
                        strncat(file_output, file_input, strlen(file_input) - 5);
                } else {
                        compress = 1;
                        strncat(file_output, file_input, strlen(file_input));
                        strncat(file_output, ".zlib", sizeof(".zlib"));
                }
        } else {
                compress = 1;
                strncat(file_output, file_input, strlen(file_input));
                strncat(file_output, ".zlib", strlen(".zlib"));
        }

        fd_in = open(file_input, O_RDONLY);
        if (fd_in == -1) {
                ret = errno;
                printf("open input_file:%s failed:%d\n", file_input, errno);
                goto final1;
        }

        fd_out = open(file_output, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (fd_out == -1) {
                ret = errno;
                printf("open output_file:%s failed:%d\n", file_output, errno);
                goto final1;
        }

        if(zlib_init(COMP_LEVEL)) {
                ret = -1;
                goto final1;
        }

        while (size) {
                if(compress) {
                        size = read(fd_in, buff_in, CHUNK);
                        if(size == -1) {
                                ret = errno;
                                printf("read from input_file:%s failed:%d\n", file_input, errno);
                                goto final2;
                        } else if(size == 0)
                                break;
   
                        ret = zlib_comp(buff_out, CHUNK * 2, buff_in, size);
                        if(ret < 0) {
                                printf("zlib_comp failed ret:%d\n", ret);
                                ret = -1;
                                goto final2;
                        }

                        *((uint32_t*)out) = ret;
                        if(write(fd_out, out, HEAD_SIZE + ret) < (HEAD_SIZE + ret)) {
                                ret = errno;
                                printf("write to output_file:%s failed:%d\n", file_output, errno);
                                goto final2;
                        }

                        increase(ret);
                } else {
                        size = read(fd_in, buff_in, HEAD_SIZE);
                        if(size == 0)
                                break;
                        else if(size != HEAD_SIZE) {
                                ret = errno;
                                printf("read from input_file:%s failed:%d\n", file_input, errno);
                                goto final2;
                        }

                        int decomp_size = *((uint32_t*)buff_in);

                        size = read(fd_in, buff_in, decomp_size);
                        if(size != decomp_size) {
                                ret = errno;
                                printf("read from input_file:%s failed:%d\n", file_input, errno);
                                goto final2;
                        }

                        ret = zlib_decomp(buff_out, CHUNK * 2, buff_in, size);
                        if(ret < 0) {
                                ret = -1;
                                printf("zlib_decomp failed:%d\n", errno);
                                goto final2;
                        }

                        if(write(fd_out, buff_out, ret) < ret) {
                                ret = errno;
                                printf("write to output_file:%s failed:%d\n", file_output, errno);
                                goto final2;
                        }
                        increase(ret);
                }
        }

        dump_statistic();
final2:
        zlib_free();
final1:
        if(fd_in != -1)
                close(fd_in);
        if(fd_out != -1)
                close(fd_out);
final:
        free(in);
        free(out);
        return ret;
}