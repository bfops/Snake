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

#define STRESS_LEVEL 10000

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
