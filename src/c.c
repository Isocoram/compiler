#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/types.h"

typedef block {
    void *buffer;
    block *next;
    block *previous;
} block_t;

off_t get_file_size_in_bytes(const u8 *path) {
    i32 file_descriptor = open(path, O_RDONLY);
    if (file_descriptor == -1) { perror("open failed"); return -1; }
    off_t file_length = lseek(file_descriptor, 0, SEEK_END);
    i32 file_close_status = close(file_descriptor);
    return file_length;
}

void *copy_source_to_4kbuffer(const u8 *path) {
    u8 *buffer = malloc(4096 * sizeof(u8));
    if (buffer == NULL) { perror("Memory Allocation Failed Within 4KB Buffer Copy");}
    else {
        i32 source_file = open(path, O_RDONLY);
        if (source_file == -1) {
            perror("File Open Failed");
        }
        read(source_file, buffer, 4096);
        close(source_file);
        return buffer;
    }
}

void *source_code_buffer(const u8 *path){
    off_t file_size = get_file_size_in_bytes(path);
    i32 amount_needed_blocks = file_size / 4096;
}

i32 main(i32 argc, i8 **argv) {
    printf("%s", copy_source_to_4kbuffer("src/c.c"));
    return 0;
}