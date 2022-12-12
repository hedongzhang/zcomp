TARGET1 = zzstd
TARGET2 = zzlib

CC = gcc
CFLAGS = -Og	\
	 -Wall	\
	 -std=gnu99

SRCS_ZSTD = zstd_main.c alg_zstd.c statistic.c
OBJS_ZSTD = $(patsubst %.c, %.o, ${SRCS_ZSTD})

SRCS_ZLIB = zlib_main.c alg_zlib.c statistic.c
OBJS_ZLIB = $(patsubst %.c, %.o, ${SRCS_ZLIB})

INCLUDE = -I/usr/local/include
LIB_ZSTD = -L/usr/local/lib -lzstd -Wl,--rpath=/usr/local/lib

LIB_ZLIB = -lz

.PHONY: all clean

all: ${TARGET1} ${TARGET2}

${TARGET1}: ${OBJS_ZSTD}
	$(CC) $(CFLAGS) $(LIB_ZSTD) -o $@ $^ 

${TARGET2}: ${OBJS_ZLIB}
	$(CC) $(CFLAGS) $(LIB_ZLIB) -o $@ $^ 

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) -o $@ $^

clean:
	rm -f $(OBJS_ZSTD) $(OBJS_ZLIB) $(TARGET1) $(TARGET2)