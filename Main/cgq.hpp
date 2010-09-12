#pragma once
#include <cstddef>
#include <cassert>
#include <algorithm>

namespace detail {

template <typename T>
inline bool is_power_of_two(T x)
{
	return (x != 0) && ((x & (x - 1)) == 0);
}

// Returns a + b in the finite field [0, m) where m is a power of two. A must
// already be in the finite field, and b may be any integer representable by a
// long. This works for b being a positive AND negative number.
// a - b = a + (-b). Get it?
inline long finite_field_addition(unsigned long a, long b, unsigned long m)
{
	assert(a >= 0 && a < m);
	assert(is_power_of_two(m));

	if(b < 0)
	{
		if(b > -long(m))
			return finite_field_addition(a, m + b, m);
		else
			return finite_field_addition(a, -(-b & (m - 1)), m);
	}

	return (a + b) & (m - 1);
}

}

/**
	Clark Gaebel's queue. This is exactly what the doctor (Ben) ordered. An
	efficient FIFO queue implementation based off of an std::vector instead of
	an std::deque. It supports a very minimal interface compared to the STL,
	but still aims to be mostly compatible.
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

	template <typename TargetTy>
	class bidirectional_iterator
	{
		friend class cgq;

	public:
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef TargetTy value_type;
		typedef cgq::difference_type difference_type;
		typedef value_type* pointer;
		typedef value_type& reference;

	private:
		TargetTy* loc;

		TargetTy* buffer;
		size_type capacity;

		bidirectional_iterator(TargetTy* _loc, TargetTy* _buffer, size_type _capacity)
			: loc(_loc), buffer(_buffer), capacity(_capacity) {}

	public:
		bidirectional_iterator()
		{
			loc = NULL;
			buffer = NULL;
			capacity = 0;
		}

		bidirectional_iterator(const bidirectional_iterator& other)
			: loc(other.loc), buffer(other.buffer), capacity(other.capacity) {}

		bidirectional_iterator& operator=(const bidirectional_iterator& other)
		{
			loc = other.loc;
			buffer = other.buffer;
			capacity = other.capacity;
		}

		// Since operator== is much less used than operator!= on iterators,
		// we use a double negative. It's all in the name of efficiency! I swear!
		bool operator==(const bidirectional_iterator& other)
		{
			return !(*this != other);
		}

		bool operator!=(const bidirectional_iterator& other)
		{
			// If any of these asserts fail, we're comparing iterators of
			// different containers.
			assert(buffer);
 			assert(loc);

			assert(buffer == other.buffer);
			assert(capacity == other.capacity);

			return loc != other.loc;
		}

		TargetTy& operator*()
		{
			return *loc;
		}

		TargetTy* operator->()
		{
			return loc;
		}

		bidirectional_iterator& operator++()
		{
			loc = buffer + detail::finite_field_addition(loc - buffer, 1, capacity);
			return *this;
		}

		bidirectional_iterator operator++(int)
		{
			bidirectional_iterator old(loc, buffer, capacity);
			++(*this);
			return old;
		}

		bidirectional_iterator& operator--()
		{
			loc = buffer + detail::finite_field_addition(loc - buffer, -1, capacity);
			return *this;
		}

		bidirectional_iterator operator--(int)
		{
			bidirectional_iterator old(loc, buffer, capacity);
			--(*this);
			return old;
		}

		~bidirectional_iterator() {}
	};

	typedef bidirectional_iterator<Ty>       iterator;
	typedef bidirectional_iterator<const Ty> const_iterator;
	typedef bidirectional_iterator<Ty>       reverse_iterator;
	typedef bidirectional_iterator<const Ty> const_reverse_iterator;

private:
	size_type minSize;
	size_type capacity;
	size_type numElems;

	pointer buffer;
	pointer head;
	pointer tail;

private:
	// Determins the next power of two that is higher than k.
	// This only works for unsigned types.
	template <class T>
	static inline T next_power_of_two(T k)
	{
		if(detail::is_power_of_two(k))
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
	// the field is the buffer. Negative numbers are acceptable.
	inline pointer move_pointer(pointer p, difference_type n)
	{
		return buffer + detail::finite_field_addition(p - buffer, n, capacity);
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

	cgq(const cgq& other)
	{
		buffer = NULL;
		*this = other;
	}

	cgq& operator=(const cgq& other)
	{
		if(buffer)
			delete[] buffer;

		capacity = other.capacity;
		minSize = other.minSize;
		numElems = other.numElems;
		buffer = new value_type[capacity];
		head = buffer;
		tail = buffer;

		pointer next = other.head;

		for(size_type i = 0; i < numElems; ++i)
		{
			*tail++ = *next;
			next = move_pointer(next, 1);
		}

		return *this;
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

	/// Appends an item to the queue. The queue will be resized as necessary.
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

	inline size_type max_size() const
	{
		size_type ret = static_cast<size_type>(-1);
		assert(ret > 0);

		return ret;
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

	void swap(cgq& other)
	{
		std::swap(buffer, other.buffer);
		std::swap(head, other.head);
		std::swap(tail, other.tail);

		std::swap(numElems, other.numElems);
		std::swap(capacity, other.capacity);
		std::swap(minSize, other.minSize);
	}

private:
	template <typename IterTy>
	inline IterTy make_iterator(pointer from)
	{
		return IterTy(from, buffer, capacity);
	}

public:
	iterator begin()
	{
		return make_iterator<iterator>(head);
	}

	iterator end()
	{
		return make_iterator<iterator>(tail);
	}

	const_iterator begin() const
	{
		return make_iterator<const_iterator>(head);
	}

	const_iterator end() const
	{
		// See the non-const version for an explanation.
		return make_iterator<const_iterator>(tail);
	}

	reverse_iterator rbegin()
	{
		return make_iterator<reverse_iterator>(move_pointer(tail, -1));
	}

	reverse_iterator rend()
	{
		return make_iterator<reverse_iterator>(move_pointer(head, -1));
	}

	const_reverse_iterator rbegin() const
	{
		return make_iterator<const_reverse_iterator>(move_pointer(tail, -1));
	}

	const_reverse_iterator rend() const
	{
		return make_iterator<const_reverse_iterator>(move_pointer(head, -1));
	}

	~cgq()
	{
		delete[] buffer;
	}
};

namespace std {
	template <typename T>
	void swap(cgq<T>& a, cgq<T>& b)
	{
		a.swap(b);
	}
}
