#include <iostream>
#include "allocator.h"


int main()
{
	using Tint = Allocator<int>;
	// rebind double
	using Tdouble = Tint::rebind<double>::other;

	// example with int
	std::vector<int, Tint> vec1(10);
	std::cout << vec1.get_allocator().get_allocations() << "\n";

	// example with double
	std::vector<double, Tdouble> vec2(5);
	std::cout << vec2.get_allocator().get_allocations() << "\n";


	// clear and change capacity 
	vec1.clear();
	vec1.shrink_to_fit();
	std::cout << vec1.get_allocator().get_allocations() << "\n";
	return 0;
}