//
// Created by xw on 2020/6/28.
//
#include "Memorypool.h"

char *memory_pool::start_pool = nullptr;

char *memory_pool::end_pool = nullptr;

std::size_t memory_pool::heap_size = 0;

my_obj *volatile memory_pool::free_lists[NUM_OF_FREELISTS] = {nullptr};

std::size_t memory_pool::align_bytes(std::size_t bytes) {
    return (bytes + ALIGN_SIZE - 1) & ~(ALIGN_SIZE - 1);
}

std::size_t memory_pool::get_freelists_index(std::size_t bytes) {
    return (bytes + ALIGN_SIZE - 1) / ALIGN_SIZE - 1;
}

char *memory_pool::chunk_allocate(std::size_t obj_size, unsigned long &obj_num) {
    char *res;
    auto total = obj_size * obj_num;
    auto left = end_pool - start_pool;

    if (left >= total) {
        res = start_pool;
        start_pool += total;
        return res;
    } else if (left >= obj_size) {
        obj_num = left / obj_size;
        total = obj_size * obj_num;
        res = start_pool;
        start_pool += total;
        return res;
    } else {
        auto get_bytes = 2 * total + align_bytes(heap_size >> 4);
        if (get_bytes > 0 and start_pool) {
            auto my_free_list = free_lists + get_freelists_index(left);
            ((my_obj *) start_pool)->free_list_link = *my_free_list;
            *my_free_list = (my_obj *) start_pool;
        }
        start_pool = (char *) malloc(get_bytes);
        heap_size += get_bytes;
        end_pool = start_pool + get_bytes;
        return chunk_allocate(obj_size, obj_num);
    }
}

void *memory_pool::refill(std::size_t size) {
    unsigned long obj_num = 20;
    auto chunk_mem = chunk_allocate(size, obj_num);
    auto my_free_list = free_lists + get_freelists_index(size);
    my_obj *res;
    my_obj *cur_block, *next_block;

    if (obj_num == 1) return chunk_mem;
    res = (my_obj *) chunk_mem;

    *my_free_list = next_block = (my_obj *) (chunk_mem + size);

    for (int i = 1;; ++i) {
        cur_block = next_block;
        next_block = (my_obj *) ((char *) next_block + size);
        if (obj_num - 1 == i) {
            cur_block->free_list_link = nullptr;
            break;
        } else {
            cur_block->free_list_link = next_block;
        }
    }
    return res;
}

void *memory_pool::allocate(std::size_t N) {
    if (N > MAX_SIZE) return malloc(N);
    auto my_free_list = free_lists + get_freelists_index(N);
    auto res = *my_free_list;

    if (!res) {
        return refill(align_bytes(N));
    }

    *my_free_list = res->free_list_link;
    return res;
}

void memory_pool::deallocate(void *p, std::size_t size) {
    if (size > MAX_SIZE) {
        free(p);
    } else {
        auto my_free_list = free_lists + get_freelists_index(align_bytes(size));
        ((my_obj *) p)->free_list_link = *my_free_list;
        *my_free_list = (my_obj *) p;
    }
}