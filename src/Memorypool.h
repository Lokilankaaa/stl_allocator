//
// Created by 薛伟 on 2020/6/11.
//

#ifndef STL_ALLOCATOR_MEMORYPOOL_H
#define STL_ALLOCATOR_MEMORYPOOL_H

#define POOL_SIZE 4096 //one pool contains 4096 T variables

template<class T>
struct one_block {
    T *p = new T;
    one_block *pre;
    one_block *next;
};

template<class T>
class Memorypool {
private:
    one_block<T> *freelist;
    one_block<T> *occupied_list;
    int occupied_num;
    int total_size;

public:
    explicit Memorypool(int size = POOL_SIZE);

    ~Memorypool();

    int get_total_size() const;

    int get_occupied_size() const;

    one_block<T> get_freelist() const;

//    one_block<T> get
};

template<class T>
Memorypool<T>::Memorypool(int size) {
    total_size = size;
    occupied_num = 0;
    occupied_list = nullptr;
    freelist = new one_block<T>;
    freelist->pre = nullptr;
    auto cur = freelist;
    for (int i = 0; i < total_size - 1; ++i) {
        auto tmp = new one_block<T>;
        tmp->next = nullptr;
        tmp->pre = cur;
        cur->next = tmp;
        cur = cur->next;
    }
}

template<class T>
Memorypool<T>::~Memorypool() {
    auto cur = freelist;
    for (int i = 0; i < total_size - 1; ++i) {
        auto t = cur;
        cur = cur->next;
        delete t;
    }
}

template<class T>
int Memorypool<T>::get_total_size() const {
    return total_size;
}

template<class T>
int Memorypool<T>::get_occupied_size() const {
    return occupied_num;
}

template<class T>
one_block<T> Memorypool<T>::get_freelist() const {
    return freelist;
}


#endif //STL_ALLOCATOR_MEMORYPOOL_H
