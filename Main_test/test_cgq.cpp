#include <gtest/gtest.h>
#include "../Main/cgq.hpp"

// Blank, because we're just seeing if cqueue can compile for now ;)
TEST(cgq, default_construction)
{
	cgq<int> q;
}

TEST(cgq, no_edge_conditions)
{
	cgq<int> q(2);

	q.push(3);
	q.push(2);
	q.push(1);

	int temp = 0;

	EXPECT_TRUE(q.pop(temp));
	EXPECT_EQ(3, temp);

	EXPECT_TRUE(q.pop(temp));
	EXPECT_EQ(2, temp);

	EXPECT_TRUE(q.pop(temp));
	EXPECT_EQ(1, temp);

	EXPECT_FALSE(q.pop(temp));
}

#define STRESS_LEVEL 100000

TEST(cgq, stress_test)
{
	cgq<int> q;

	for(int i = 0; i < STRESS_LEVEL; ++i)
		q.push(i);

	int temp = -1;

	for(int i = 0; i < STRESS_LEVEL; ++i)
	{
		EXPECT_TRUE(q.pop(temp));
		EXPECT_EQ(i, temp);
	}

	for(int i = 0; i < STRESS_LEVEL; ++i)
		EXPECT_FALSE(q.pop(temp));
}

TEST(cgq, swapping)
{
	cgq<int> a;
	cgq<int> b;

	std::swap(a, b);
}

TEST(cgq, copy_construction_and_assignment)
{
	cgq<int> a;
	cgq<int> b;

	a = b;

	cgq<int> c(a);
}

TEST(cgq, finite_field_addition)
{
	EXPECT_EQ(2, detail::finite_field_addition(1, -3, 4));
	EXPECT_EQ(2, detail::finite_field_addition(1, -15, 4));
}

TEST(cgq, forward_iteration)
{
	cgq<int> q;

	for(int i = 0; i < 100; ++i)
		q.push(i);

	int n = 0;

	for(cgq<int>::iterator i = q.begin(), e = q.end(); i != e; ++i)
	{
		EXPECT_EQ(n, *i);
		++n;
	}

	EXPECT_EQ(100, n);
}

TEST(cgq, backward_iteration)
{
	cgq<int> q;

	for(int i = 99; i >= 0; --i)
		q.push(i);

	int n = 0;

	for(cgq<int>::reverse_iterator i = q.rbegin(), e = q.rend(); i != e; --i)
	{
		EXPECT_EQ(n, *i);
		++n;
	}

	EXPECT_EQ(100, n);
}

TEST(cgq, clear)
{
	cgq<int> q(2);

	q.push(1);
	q.push(2);
	q.push(3);
	q.push(4);
	q.push(5);

	q.clear();

	EXPECT_TRUE(q.empty());
}
