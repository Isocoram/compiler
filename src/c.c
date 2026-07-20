#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/types.h"

typedef struct block {
    u8 *buffer;
    struct block *next;
    struct block *previous;
    u64 count;
} block_t;

off_t get_file_size_in_bytes(const u8 *path) {
    i32 file_descriptor = open(path, O_RDONLY);
    if (file_descriptor == -1) { perror("open failed"); return -1; }
    off_t file_length = lseek(file_descriptor, 0, SEEK_END);
    i32 file_close_status = close(file_descriptor);
    return file_length;
}

i32 main(i32 argc, i8 **argv) {
    printf("%s", copy_source_to_4kbuffer("src/c.c"));
    return 0;
}