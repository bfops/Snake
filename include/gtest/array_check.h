#pragma once
#include <boost/cstdint.hpp>
#include <gtest/gtest.h>

#include <cassert>
#include <string>

static char hexify_octet(boost::uint8_t octet)
{
	assert(octet < 16);

	static const char* hex = "0123456789ABCDEF";

	return hex[octet];
}

static std::string hexify_element(boost::uint8_t elem)
{
	std::string s;

	s += hexify_octet((elem >> 4) & 0x0F);
	s += hexify_octet(elem & 0x0F);

	return s;
}

static std::string stringify_array(const boost::uint8_t* arr, size_t len)
{
	if(len == 0)
		return "[]";

	std::string result = "[";

	result += hexify_element(arr[0]);

	for(size_t i = 1; i < len; ++i)
	{
		result += " ";
		result += hexify_element(arr[i]);
	}

	return result + "]";
}

static std::string generate_failure_message(const boost::uint8_t* arr1, size_t len1, const boost::uint8_t* arr2, size_t len2)
{
	return std::string("Arrays not equal: ") + stringify_array(arr1, len1) + ", " + stringify_array(arr2, len2);
}

template <typename Arr1Type, typename Arr2Type>
bool are_arrays_equal(const Arr1Type* arr1, size_t len1, const Arr2Type* arr2, size_t len2)
{
	if(len1 != len2)
		return false;

	for(size_t i = 0; i < len1; ++i)
		if(arr1[i] != arr2[i])
			return false;

	return true;
}

template <typename Arr1Type, typename Arr2Type>
void check_arrays(const Arr1Type* expected, size_t len1, const Arr2Type* actual, size_t len2)
{
	EXPECT_TRUE(are_arrays_equal(expected, len1, actual, len2)) << generate_failure_message(reinterpret_cast<const boost::uint8_t*>(expected), len1*sizeof(Arr1Type), reinterpret_cast<const boost::uint8_t*>(actual), len2*sizeof(Arr2Type));
}

template <typename Arr1Type, typename Arr2Type, size_t len1, size_t len2>
void check_arrays(const Arr1Type (&expected)[len1], const Arr2Type (&actual)[len2])
{
	return check_arrays(expected, len1, actual, len2);
}
