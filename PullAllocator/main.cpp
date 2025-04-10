#include <iostream>
#include "PullAllocator.h"

struct Test
{
public:
    Test(int a, int b) 
		: x(a)
	 	, y(b) 
	{
        std::cout << "ctor: " << x << ", " << y << std::endl;
    }

    Test() 
	{
        std::cout << "dtor: " << x << ", " << y << std::endl;
    }
public:
	int x, y;
};

int main() {
    PoolAllocator<Test> allocator(5);

    Test* obj = allocator.allocate(42, 99);
    allocator.deallocate(obj);
    return 0;
}
