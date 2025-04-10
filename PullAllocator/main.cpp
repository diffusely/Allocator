#include <iostream>

template <typename T>
class PoolAllocator 
{
	struct FreeNode 
	{
        FreeNode* next;
    }; 

public:
    PoolAllocator(std::size_t count)
        : chunkCount(count)
    {
        memory = static_cast<T*>(::operator new(count * sizeof(T)));
        freeList = nullptr;

		// push pointers freeList
        for (size_t i = 0; i < chunkCount; i++) 
		{
            FreeNode* node = reinterpret_cast<FreeNode*>(memory + i);
            node->next = freeList;
            freeList = node;
        }
    }

    ~PoolAllocator() 
	{
        ::operator delete(memory);
    }

    template <typename... Args>
    T* allocate(Args&&... args) 
	{
        if (!freeList)
            throw std::bad_alloc();

        FreeNode* node = freeList;

        freeList = freeList->next;
        return new (node) T(std::forward<Args>(args)...);
    }

    void deallocate(T* ptr) 
	{
        ptr->~T();

        FreeNode* node = reinterpret_cast<FreeNode*>(ptr);

        node->next = freeList;
        freeList = node;
    }

private:
    T* memory;
    FreeNode* freeList;
    std::size_t chunkCount;
};

struct MyObject 
{
public:
    MyObject(int a, int b) 
		: x(a)
	 	, y(b) 
	{
        std::cout << "ctor: " << x << ", " << y << std::endl;
    }

    ~MyObject() 
	{
        std::cout << "dtor: " << x << ", " << y << std::endl;
    }
public:
	int x, y;
};

int main() {
    PoolAllocator<MyObject> allocator(5);

    MyObject* obj = allocator.allocate(42, 99);
    allocator.deallocate(obj);
    return 0;
}
