#include "pch.h"
#include "deque.h"
#include <deque>
#include <iterator>
#include <iostream>
#include <vector>
#include <algorithm>

extern int random(int max);
extern int random(int min, int max);
extern Deque<int>gen_random(size_t size, int max_value);
extern Deque<int>gen_id(size_t size);

namespace {
TEST(iterator, sort) {
	Deque<int> deq = gen_random(20, 100);
	std::sort(deq.begin(), deq.end());
	for (size_t i = 0; i < deq.size() - 1; ++i) {
		EXPECT_LE(deq[i], deq[i + 1]);
	}
}

TEST(iterator, const_reverse_iterator) {
	Deque<int> a = gen_id(5);
	Deque<int>::const_reverse_iterator it;
	it = a.crbegin();
	EXPECT_EQ(*it, 4);
}

TEST(iterator, reverse_iterator) {
	Deque<int> a = gen_id(10);
	auto it = a.rbegin();
	it++;
	EXPECT_EQ(*it, 8);
}

TEST(iterator, const_iterator_and_plus_and_random_access) {
	Deque<int> a = gen_id(8);
	auto it = a.cbegin();
	int i = 3;
	EXPECT_EQ(it[i], i);
	EXPECT_EQ(it + i, i);
}

TEST(iterator, increment) {
	Deque<int> a = gen_id(8);
	auto it = a.cbegin();
	auto it2 = it++;

	EXPECT_EQ(*it2, 0);
	EXPECT_EQ(*it, 1);
	it2 = ++it;
	EXPECT_EQ(*it2, 2);
	EXPECT_EQ(*it, 2);
}

TEST(iterator, minus) {
	int n = 11, x = 3, y = 5;
	Deque<int> a = gen_random(n, 20);
	auto it = a.begin() + x;
	auto it2 = a.end() - y;
	EXPECT_EQ(it - it2, x - n + y);
}

}//namespace