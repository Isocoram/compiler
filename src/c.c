#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/types.h"

#define BUFFER_SIZE 4096

typedef struct block {
    u8 buffer[BUFFER_SIZE];
    struct block *next;
    struct block *previous;
    u32 count;
} block_t;

off_t get_file_size_in_bytes(const i8 *path) {
    off_t file_length = 0;
    i32 file_descriptor = open(path, O_RDONLY);
    if (file_descriptor < 0) { goto cleanup; }
    file_length = lseek(file_descriptor, 0, SEEK_END);
    if (file_length < 0) { goto cleanup; }
    cleanup:
    i32 file_close_status = close(file_descriptor);
    if (file_close_status < 0) { goto cleanup; }
    (void)file_close_status;
    return file_length;
}

block_t *create_block(void) {
    block_t *block = malloc(sizeof(block_t));
    if (!block) { goto cleanup; }
    for (u32 i = 0; i < BUFFER_SIZE - 1; i++) {
        *((block->buffer) + i) = 0;
    }
    *((block->buffer) + BUFFER_SIZE - 1) = '\0';
    block->next = NULL;
    block->previous = NULL;
    block->count = 0;
    cleanup:
        free(block);
    return block;
}

block_t *chunk_memory(u32 byte_count) {
    u32 block_count = (byte_count + BUFFER_SIZE -1) / BUFFER_SIZE;
    block_t *tail = create_block();
    block_t *head = tail;
    for (u32 i = 0; i < block_count; i++) {
        block_t *new_block = create_block();
        if (!(head->next)) {
            head->next = new_block;
            tail = new_block;
            new_block->previous = head;
            new_block->count = new_block->previous->count + 1;
        } else {
            tail->next = new_block;
            new_block->previous = tail;
            tail = new_block;
            new_block->count = new_block->previous->count + 1;
        }
    }
    return head;
}

i32 get_block_count(block_t *list) {
    i32 count = 1;
    block_t *ptr = list;
    if (!(ptr->next)) { return count; }
    while(ptr->next) { count++; ptr = ptr->next; }
    return count;
}

block_t *linked_list_source_file(const i8 *path) {
    i32 file_descriptor = open(path, O_RDONLY);
    i32 file_size = get_file_size_in_bytes(path);
    block_t *block_linked_list = chunk_memory(file_size);
    block_t *tail = block_linked_list;
    i32 block_count = get_block_count(block_linked_list);
    for (u32 i = 0; i < (u32)block_count; i++) {
        lseek(file_descriptor, (BUFFER_SIZE - 1)*i, SEEK_SET);
        read(file_descriptor, tail->buffer, BUFFER_SIZE - 1);
        tail = tail->next;
    }
    close(file_descriptor);
    return block_linked_list;
}

block_t *block_by_index(block_t *list, u32 index) {
    block_t *block_ptr = list;
    while (block_ptr->count != index) {
        block_ptr = block_ptr->next;
    }
    return block_ptr;
}

void dump_data(block_t *list) {
    printf("Block Index: %d | Content: \n%s\n", list->count, list->buffer);
    if (list->next == NULL) { return; }
    dump_data(list->next);
}

void free_list(block_t *list) {
    block_t *temporary_pointer = list;
    if (!(list->next)) { free(list); return; }
    free_list(temporary_pointer->next);
    free(list);
}

i32 main(i32 argc, i8 **argv) {
    (void)argc;
    u32 filesize = get_file_size_in_bytes(argv[1]);
    block_t *linked_list = chunk_memory(filesize);
    block_t *writtenlist = linked_list_source_file(argv[1]);
    block_t *zerothblock = block_by_index(writtenlist, 0);
    (void)zerothblock;
    //printf("\n%s\n\n", zerothblock->buffer);
    dump_data(writtenlist);
    free_list(linked_list);
    free_list(writtenlist);
    printf("%d", filesize);
    return 0;
}