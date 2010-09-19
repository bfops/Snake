#pragma once
// This file implements "fancier" algorithms than the STL provides natively.

#include <vector>
#include <algorithm>
#include <functional>

#include <boost/bind/bind.hpp>

// Removes an element _i_ from a vector _v_ in constant time. The only caveat
// is that the order of the vector will not necessarily be maintained.
template <typename _T>
void unordered_remove(std::vector<_T>& v, typename std::vector<_T>::iterator i)
{
	if(i == v.end())
		return;

	std::swap(*i, *v.rbegin());
	v.pop_back();
}

// Performs an unordered_remove on the vector _v_ on the first value that is
// equal to _val_. Returns whether or not an element was removed.
template <typename _T>
bool unordered_find_and_remove(std::vector<_T>& v, const _T& val)
{
	typename std::vector<_T>::iterator loc = std::find(v.begin(), v.end(), val);
	if(loc == v.end())
		return false;

	unordered_remove(v, loc);
	return true;
}

// Returns true iff predicate(element) returns true for every element in the
// interval [begin, end).
template <typename IterType, typename PredType>
inline bool all(IterType begin, IterType end, PredType predicate)
{
	return std::find_if(begin, end, std::unary_negate<PredType>(predicate)) == end;
}

// Returns true iff predicate(element) returns true for any element in the
// interval [begin, end).
template <typename IterType, typename PredType>
inline bool any(IterType begin, IterType end, PredType predicate)
{
	return std::find_if(begin, end, predicate) != end;
}

// Returns true if `elem` is in the interval [begin, end) and false otherwise.
template <typename IterType, typename T>
inline bool in(IterType begin, IterType end, const T& elem)
{
	return any(begin, end, boost::bind(std::equal_to<T>(), elem, _1));
}
