#include "pch.h"
#include "deque.h"
#include <deque>
#include <iostream>
#include <vector>
#include <algorithm>

extern int random(int max);
extern int random(int min, int max);

//#######################################################

Deque<int> gen_random(size_t size, int max_value) {
	Deque<int> result;
	for (size_t i = 0; i < size; i++) {
		result.push_back(random(-max_value, max_value));
	}
	return result;
}

Deque<int> gen_id(size_t size) {
	Deque<int> result;
	for (size_t i = 0; i < size; i++) {
		result.push_back((int)i);
	}
	return result;
}

namespace {
TEST (deque, shifting_push_front) {
	Deque<int> a = gen_id(5);
	a.push_front(-5);
	EXPECT_EQ(a[0], -5);
	EXPECT_EQ(a.front(), -5);
	EXPECT_EQ(a[3], 2);
}

TEST (deque, shifting_pop_front) {
	Deque<int> a = gen_id(5);
	a.pop_front();
	EXPECT_EQ(a[2], 3);
}

TEST (deque, front) {
	for (int i = 0; i < 50; i++) {
		Deque<int> a = gen_random(random(20) + 1, 10);
		EXPECT_EQ(a[0], a.front());
	}
}

TEST (deque, back) {
	for (int i = 0; i < 50; i++) {
		Deque<int> a = gen_random(random(20) + 1, 10);
		EXPECT_EQ(a[a.size() - 1], a.back());
	}
}

TEST (deque, size1) {
	Deque<int> a;
	a.push_back(0);
	a.push_back(0);
	a.pop_front();
	a.pop_back();
	a.push_front(3);
	a.push_back(7);
	a.pop_back();
	EXPECT_EQ(3, a[0]);
}

TEST (deque, max_min_int) {
	Deque<int> a;
	a.push_back(0);
	a.push_back(INT_MAX);
	a.push_back(INT_MIN);
	a.push_back(0);

	EXPECT_EQ(0, a[0]);
	EXPECT_EQ(INT_MAX, a[1]);
	EXPECT_EQ(INT_MIN, a[2]);
	EXPECT_EQ(0, a[3]);
}

TEST (deque, constructor_copy) {
	Deque<int> a = gen_id(10);
	Deque<int> b(a);
	b[1] = -1;
	a[3] = -1;
	EXPECT_EQ(a[1], 1);
	EXPECT_EQ(b[3], 3);
}

TEST (deque, assign_test) {
	Deque<int> a = gen_id(10);
	Deque<int> b = gen_random(15, 1000);
	b = a;
	b[1] = -1;
	a[3] = -1;
	EXPECT_EQ(a[1], 1);
	EXPECT_EQ(b[3], 3);
}

void test_const_access(const Deque<int>& deq) {
	EXPECT_EQ(deq.front(), -1);
	EXPECT_EQ(deq.back(), 4);
	EXPECT_EQ(deq[2], 1);
}

TEST(deque, const_access) {
	Deque<int> deq = gen_id(5);
	deq.push_front(-1);
	test_const_access(deq);
}

//auto test
//#######################################################################
const int COUNT_OF_TYPES = 12;

template<class T>
void write_container(T container) {
	for (size_t i = 0; i < container.size(); i++) {
		std::cout << container[i] << " ";
	}
	std::cout << "\n";
}

template<class DEQ1, class DEQ2>
int random_operation(DEQ1& first, DEQ2& second, size_t max_size, int max_value) {
	int type = random(COUNT_OF_TYPES);
	bool ok = true;

	if (type == 0) {
		ok = (first.empty() == second.empty());
	}
	if (type == 1) {
		ok = (first.size() == second.size());	
	}
	if (type == 2) {
		if (first.empty()) return 0;
		int index = random((int)first.size() - 1);
		ok = (first[index] == second[index]);
	}
	if (type == 3) {
		if (first.empty()) return 0;
		int index = random((int)first.size() - 1);
		int value = random(-max_value, max_value);
		first[index] = value;
		second[index] = value;
	}
	if (type == 4) {
		if (first.size() >= max_size) return 0;
		int value = random(-max_value, max_value);
		first.push_back(value);
		second.push_back(value);
	}
	if (type == 5) {
		if (first.size() >= max_size) return 0;
		int value = random(-max_value, max_value);
		first.push_front(value);
		second.push_front(value);
	}
	if (type == 6) {
		if (first.empty()) return 0;
		first.pop_back();
		second.pop_back();
	}
	if (type == 7) {
		if (first.empty()) return 0;
		first.pop_front();
		second.pop_front();
	}
	if (type == 8) {
		if (first.empty()) return 0;
		ok = (first.front() == second.front());
	}
	if (type == 9) {
		if (first.empty()) return 0;
		ok = (first.back() == second.back());
	}
	if (type == 10) {
		if (first.empty()) return 0;
		int value = random(-max_value, max_value);
		first.back() = value;
		second.back() = value;
	}
	if (type == 11) {
		if (first.empty()) return 0;
		int value = random(-max_value, max_value);
		first.front() = value;
		second.front() = value;
	}

	if (!ok) {
		EXPECT_TRUE(false);
		std::cerr << type << "\n";
		write_container(first);
		write_container(second);
		return -1;
	}
	else {
		return type;
	}
}

void test_series(size_t max_size, int max_value, int count_of_iterations) {
	Deque<int> my_deque;
	std::deque<int> std_deque;
	std::vector<int> operations;
	for (int i = 0; i < count_of_iterations; ++i) {
		operations.push_back(random_operation(my_deque, std_deque, max_size, max_value));
		if (operations.back() == -1) {
			for (size_t j = 0; j < operations.size(); ++j) std::cout << operations[j] << " ";
			std::cout << "\n";
			return;
		}
	}
}

TEST (deque, everything) {
	for (int i = 0; i < 1000; i++) test_series(20, 10, 1000);

	for (int i = 0; i < 100; i++) test_series(10000, INT_MAX, 10000);
}
};//namespace
