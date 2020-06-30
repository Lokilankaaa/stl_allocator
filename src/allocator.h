//
// Created by xw on 2020/6/11.
//

#ifndef STL_ALLOCATOR_ALLOCATOR_H
#define STL_ALLOCATOR_ALLOCATOR_H

#include <cstddef>
#include <type_traits>
#include "Memorypool.h"
#include <cstdlib>
#include <new>

namespace my_allocator {
    template<class T>
    class allocator {
    public:
        typedef void Not_user_specialized;
        typedef T value_type;
        typedef value_type *pointer;
        typedef const value_type *const_pointer;
        typedef value_type &reference;
        typedef const value_type &const_reference;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef std::true_type propagate_on_container_move_assignment;
        typedef std::true_type is_always_equal;


    public:
        allocator() = default;

        ~allocator() = default;

        pointer address(reference _Val) const _NOEXCEPT { return &_Val; }

        const_pointer address(const_reference _Val) const _NOEXCEPT { return &_Val; }

        void deallocate(pointer _Ptr, size_type _Count) {
            memory_pool::deallocate(_Ptr, _Count);
        }

//    _DECLSPEC_ALLOCATOR
        pointer allocate(size_type _Count) {
            auto total_size = _Count * sizeof(value_type);
            if (total_size > MAX_SIZE)
                return (pointer) malloc(total_size);
            else {
                auto res = memory_pool::allocate(total_size);
                if (res)
                    return (pointer) res;
                else
                    throw std::bad_alloc();
            }
        }

        template<class _Uty>
        void destroy(_Uty *_Ptr) {
            _Ptr->~_Uty();
        }

        template<class _Objty, class... _Types>
        void construct(_Objty *_Ptr, _Types &&... _Args) {
            new(_Ptr) _Objty(std::forward<_Types>(_Args)...);
        }

    };
}


#endif //STL_ALLOCATOR_ALLOCATOR_H
