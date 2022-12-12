## zcomp
compress file tool

## example
```shell
# compress data.dbf
./zzstd /mnt/nvme0n1/data.dbf

TOTAL:3538945    UNKNOW:0
========================
0K~1K: 228769
1K~2K: 2301901
2K~3K: 1004854
3K~4K: 797
4K~5K: 596
5K~6K: 1198
6K~7K: 830
7K~8K: 0

# decompress data.dbf.zstd
./zzstd /mnt/nvme0n1/data.dbf.zstd

TOTAL:3538945    UNKNOW:0
========================
0K~1K: 0
1K~2K: 0
2K~3K: 0
3K~4K: 0
4K~5K: 0
5K~6K: 0
6K~7K: 0
7K~8K: 3538945


```
