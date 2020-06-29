#include <iostream>
#include <vector>
#include "./src/allocator.h"
#include <random>
#include <chrono>
#pragma GCC optimize(3, "Ofast", "inline")

using Point2D = std::pair<int, int>;

//Obj *volatile second_allocator::mem_pool[COUNT_FREE_LISTS] = {nullptr};
//char *second_allocator::start_mem_pool = nullptr;
//char *second_allocator::end_mem_pool = nullptr;
//size_t second_allocator::heap_size = 0;

template<template<class> class allocator>
class test {
private:
    int TestSize = 10000;
    int PickSize = 10000;
public:
    void run();
};

template<template<class> class allocator>
void test<allocator>::run() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, TestSize);

    // vector creation
    using IntVec = std::vector<int, allocator<int>>;
    std::vector<IntVec, allocator<IntVec>> vecints(TestSize);
    for (int i = 0; i < TestSize; i++)
        vecints[i].resize(dis(gen));

    using PointVec = std::vector<Point2D, allocator<Point2D>>;
    std::vector<PointVec, allocator<PointVec>> vecpts(TestSize);
    for (int i = 0; i < TestSize; i++)
        vecpts[i].resize(dis(gen));

    // vector resize
    for (int i = 0; i < PickSize; i++) {
        int idx = dis(gen) - 1;
        int size = dis(gen);
        vecints[idx].resize(size);
        vecpts[idx].resize(size);
    }

    // vector element assignment
    {
        int val = 10;
        int idx1 = dis(gen) - 1;
        int idx2 = vecints[idx1].size() / 2;
        vecints[idx1][idx2] = val;
        if (vecints[idx1][idx2] == val)
            std::cout << "correct assignment in vecints: " << idx1 << std::endl;
        else
            std::cout << "incorrect assignment in vecints: " << idx1 << std::endl;
    }
    {
        Point2D val(11, 15);
        int idx1 = dis(gen) - 1;
        int idx2 = vecpts[idx1].size() / 2;
        vecpts[idx1][idx2] = val;
        if (vecpts[idx1][idx2] == val)
            std::cout << "correct assignment in vecpts: " << idx1 << std::endl;
        else
            std::cout << "incorrect assignment in vecpts: " << idx1 << std::endl;
    }

}

int main() {
    test<my_allocator::allocator> test1;
//    test<DefaultAllocator> test2;
    test<std::allocator> test3;
    auto start = std::chrono::system_clock::now();
    test1.run();
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "memory pool allocator cost " << duration.count() << " ms" << std::endl;
//    start = std::chrono::system_clock::now();
//    test2.run();
//    end = std::chrono::system_clock::now();
//    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
//    std::cout << "default allocator cost " << duration.count() << " ms" << std::endl;
    start = std::chrono::system_clock::now();
    test3.run();
    end = std::chrono::system_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "std allocator cost " << duration.count() << " ms" << std::endl;
    return 0;
}
