//
// Created by xw on 2020/6/11.
//

#ifndef STL_ALLOCATOR_MEMORYPOOL_H
#define STL_ALLOCATOR_MEMORYPOOL_H

#include <cstdlib>

#define MAX_SIZE 65536 //The max size of block in free lists
#define ALIGN_SIZE 32 //The incremental size of blocks
static const int NUM_OF_FREELISTS = MAX_SIZE / ALIGN_SIZE; //The size of free list

union my_obj {
    union my_obj *free_list_link;
    char data[1];
};

class memory_pool {
private:
    static my_obj *volatile free_lists[NUM_OF_FREELISTS];

    static char *start_pool;

    static char *end_pool;

    static std::size_t heap_size;

    //to get the index of a free list to be allocate
    static std::size_t get_freelists_index(std::size_t bytes);

    //to align the bytes to the multiples of align_byte
    static std::size_t align_bytes(std::size_t bytes);

    //to get memory from memory pool with size (obj_size * obj_num)
    static char *chunk_allocate(std::size_t obj_size, unsigned long &obj_num);

    //to refill the memory pool
    static void *refill(std::size_t size);

public:
    //allocate memory to the client from memory pool
    static void *allocate(std::size_t N);

    //return memory to memory pool or system
    static void deallocate(void *p, std::size_t size);
};


#endif //STL_ALLOCATOR_MEMORYPOOL_H
