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

block_t *copy_source_to_4kbuffer(const u8 *path) {
    block_t block = malloc
    if (buffer == NULL) { perror("Memory Allocation Failed Within 4KB Buffer Copy");}
    else {
        i32 source_file = open(path, O_RDONLY);
        if (source_file == -1) {
            perror("File Open Failed");
        }
        read(source_file, buffer, 4096);
        close(source_file);
        block.buffer = buffer;
        block
        return buffer;
    }
}

block_t *init_block(void) {
    block_t *block = malloc(sizeof(block_t));
    block->buffer = NULL;
    block->next = NULL;
    block->previous = NULL;
    block->count = 0;
    return block;
}

void append(block_t *head, block_t block) {

}

block_t *source_code_buffer(const u8 *path){
    off_t file_size = get_file_size_in_bytes(path);
    i32 amount_needed_blocks = file_size / 4096;
    block_t head_code_chunk = init_block();

    for (size_t block_index = 0; block_index < amount_needed_blocks; block_index++) {
        code_chunk->buffer = copy_source_to_4kbuffer(path);
    }
}
 
i32 main(i32 argc, i8 **argv) {
    printf("%s", copy_source_to_4kbuffer("src/c.c"));
    return 0;
}