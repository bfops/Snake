#pragma once
#include <cstddef>

/**
	Clark Gaebel's queue. This is exactly what the doctor (Ben) ordered. An
	efficient FIFO queue implementation based off of an std::vector instead of
	an std::deque. It supports a very minimal interface compared to the STL,
	but still aims to be mostly compatible.

	The following operations are supported:
		iterator
		const_iterator
		reverse_iterator
		const_reverse_iteration
		begin() (const, non-const, and reverse)
		end() (const, non-const, and reverse)
		size()
		empty()
		cqueue()
		cqueue(defaultSize)
		cqueue(const cqueue&)
		operator=(const cqueue&)
		~cqueue()
		reserve()
		push()
		bool pop(Ty& x)
		clear()
*/
template <typename Ty>
class cgq
{
public:
	typedef Ty        value_type;
	typedef Ty*       pointer;
	typedef Ty&       reference;
	typedef const Ty& const_reference;
	typedef size_t    size_type;
	typedef ptrdiff_t difference_type;
};
