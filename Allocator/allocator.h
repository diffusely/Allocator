#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <exception>
#include <stdlib.h>
#include <vector>
#include <limits>

template <class T>
class Allocator
{
public:

	using value_type = T;

	using pointer = T *;
	using const_pointer = const T *;

	using size_type = size_t;

	template <class U>
	struct rebind
	{
    	typedef Allocator<U> other;
	};

	Allocator() = default;
	~Allocator() = default;

	pointer allocate(size_type size)
	{
		allocations += size;
		return static_cast<pointer>(operator new(sizeof(T) * size));
	}

	void deallocate(pointer ptr, size_type size)
	{
		allocations -= size;
		operator delete(ptr);
	}

	template<class U, class ...Args>
	void construct(U *ptr, Args && ...args)
	{
		new(ptr) U(std::forward<Args>(args)...);
	}

	template<class U>
	void destroy(U *ptr)
	{
		ptr->~U();
	}

	size_type max_size() const
	{
		return std::numeric_limits<size_type>::max();
	}

	size_type get_allocations() const
	{
		return allocations;
	}

	bool operator==(const Allocator& other) const
    {
        return true;
    }

    bool operator!=(const Allocator& other) const
    {
        return !(*this == other);
    }

private:
	static size_type allocations;
};

template <class T>
typename Allocator<T>::size_type Allocator<T>::allocations = 0;

#endif
