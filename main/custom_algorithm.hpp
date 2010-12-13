#pragma once
// This file implements "fancier" algorithms than the STL provides natively.

#ifdef MSVC
#pragma warning(push, 0)
#endif

#include <algorithm>
#include <boost/bind/bind.hpp>
#include <cassert>
#include <functional>
#include <vector>

#ifdef MSVC
#pragma warning(pop)
#endif

// O(1) removal of *_i_ from _v_
// Will not maintain relative element order
template <typename _T>
void unordered_remove(std::vector<_T>& v, const typename std::vector<_T>::iterator i)
{
	assert(i != v.end());

	std::swap(*i, *v.rbegin());
	v.pop_back();
}

// unordered_remove the first element equal to _val_ from _v_
// returns true iff an element was removed
template <typename _T>
bool unordered_find_and_remove(std::vector<_T>& v, const _T& val)
{
	const typename std::vector<_T>::iterator loc = std::find(v.begin(), v.end(), val);
	if(loc == v.end())
		return false;

	unordered_remove(v, loc);
	return true;
}

// Returns true iff _predicate_ is true for every element
// between _begin_ and _end_ (not including end)
template <typename IterType, typename PredType>
inline bool all(IterType begin, IterType end, PredType predicate)
{
	return std::find_if(begin, end, std::unary_negate<PredType>(predicate)) == end;
}

// Returns true iff _predicate_ is true for any element
// between _begin_ and _end_ (not including end)
template <typename IterType, typename PredType>
inline bool any(IterType begin, IterType end, PredType predicate)
{
	return std::find_if(begin, end, predicate) != end;
}

// Returns true iff _elem_ is between _begin_ and _end_, not including end
template <typename IterType, typename T>
inline bool in(IterType begin, IterType end, const T& elem)
{
	return any(begin, end, boost::bind(std::equal_to<T>(), elem, _1));
}
