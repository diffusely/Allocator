#ifndef PULL_ALLOCATOR
#define PULL_ALLOCATOR

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
        pool = static_cast<T*>(::operator new(count * sizeof(T)));
        freeList = nullptr;

		// push pointers freeList
        for (size_t i = 0; i < chunkCount; i++) 
		{
            FreeNode* node = reinterpret_cast<FreeNode*>(pool + i);
            node->next = freeList;
            freeList = node;
        }
    }

    ~PoolAllocator() 
	{
        ::operator delete(pool);
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
    T* pool;
    FreeNode* freeList;
    std::size_t chunkCount;
};


#endif