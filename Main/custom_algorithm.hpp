#pragma once
// This file implements "fancier" algorithms than the STL provides natively.

#include <vector>
#include <algorithm>

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
	unordered_remove(v, loc);

	return loc != v.end();
}
