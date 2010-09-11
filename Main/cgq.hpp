#pragma once
#include <cstddef>
#include <cassert>

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
		COMPLETE size()
		COMPLETE empty()
		COMPLETE cqueue()
		COMPLETE cqueue(defaultSize)
		cqueue(const cqueue&)    NEEDS ITERATOR
		operator=(const cqueue&) NEEDS ITERATOR
		COMPLETE ~cqueue()
		COMPLETE reserve()
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

private:
	size_type minSize;
	size_type capacity;
	size_type numElems;

	pointer buffer;
	pointer head;
	pointer tail;

	static inline bool is_power_of_two(size_type x)
	{
		return (x != 0) && ((x & (x - 1)) == 0);
	}

	// Determins the next power of two that is higher than k.
	// This only works for unsigned types.
	template <class T>
	static inline T next_power_of_two(T k)
	{
		if(is_power_of_two(k))
			return k;

		if (k == 0)
			return 1;
		k--;
		for(T i = 1; i < sizeof(T)*8; i <<= 1)
			k = k | k >> i;
		return k + 1;
	}

	// Moves a pointer "p" "n" spaces forward in the queue, wrapping around
	// to the front when necessary. Think of it as finite field addition, where
	// the field is the buffer.
	inline pointer move_pointer(pointer p, size_type n)
	{
		assert(
			is_power_of_two(capacity)
		 && "We are using optimizations that assume the capacity will always be a power of two. Obviously, this is not the case!"
		);

		return buffer + ((p - buffer + n) & (capacity - 1));
	}

	/// This function resizes the buffer to any size, with no error checking.
	void resize(size_type newSize)
	{
		pointer newBuffer = new value_type[newSize];

		// TODO: Use iterators and std::copy.
		for(size_type i = 0; i < numElems; ++i)
		{
			newBuffer[i] = *head;
			head = move_pointer(head, 1);
		}

		delete[] buffer;
		buffer = newBuffer;
		head = newBuffer;
		tail = newBuffer + numElems;
		capacity = newSize;
	}

public:
	cgq(size_type minCapacity = 64)
	{
		capacity = next_power_of_two(minCapacity);

		minSize = capacity;
		numElems = 0;

		buffer = new value_type[capacity];
		head = buffer;
		tail = buffer;
	}

	/**
		Requests that the capacity of the allocated storage space for the
		elements of the queue be at least enough to hold n elements.
	*/
	void reserve(size_type minimumCapacity)
	{
		size_type n = next_power_of_two(minimumCapacity);

		if(n < numElems)
			return;

		resize(n);
	}

	void push(const_reference e)
	{
		size_type newSize = numElems + 1;

		// This line does two important things:
		//   1) It maintains the "there must be at least one unused space in
		//      the buffer (for end() and rend()).
		//   2) It doubles the size every time the buffer's about to overflow.
		if((newSize + 1) > capacity)
			resize(capacity << 1);

		*tail = e;
		tail = move_pointer(tail, 1);

		numElems = newSize;
	}

	/// Pops an element out of the queue and into "dest". This function returns
	/// true if an element could be popped, and false otherwise.
	bool pop(reference dest)
	{
		if(numElems == 0)
			return false;

		// When the number of elements drops below 1/4 of total capacity, the
		// buffer is resized to 1/2 it's current length. If the resize would
		// make it so that the buffer would drop below minSize, it is ignored.
		// These two lines ensure 3 properties:
		//     1) The buffer's memory usage stays sane.
		//     2) The buffer's capacity never drops below minSize.
		//     3) The buffer's capacity is always a power of two.
		if((numElems <= (capacity >> 2)) && (numElems >= (minSize << 1)))
			resize(capacity >> 1);

		dest = *head;
		head = move_pointer(head, 1);
		--numElems;

		return true;
	}

	/// Returns the number of elements in the queue. Runs in O(1).
	inline size_type size() const
	{
		return numElems;
	}

	/// This predicate answers the question "is the queue empty".
	inline bool empty() const
	{
		return size() == 0;
	}

	/// Removes all elements from the queue.
	inline void clear()
	{
		delete[] buffer;

		buffer = new value_type[minSize];
		head = buffer;
		tail = buffer;

		numElems = 0;
		capacity = minSize;
	}

	~cgq()
	{
		delete[] buffer;
	}
};
