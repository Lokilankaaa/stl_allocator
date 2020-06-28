#include <iostream>
#include <vector>
#include "./src/allocator.h"


int main() {

    std::vector<int, my_allocator::allocator<int> > test(10000);
    std::cout << test[500];
    return 0;
}
