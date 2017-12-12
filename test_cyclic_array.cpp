#include "pch.h"
#include "deque.h"

extern int random(int max);
extern int random(int min, int max);

namespace{

const int COUNT_OF_TEST = 10000;
const int TEST_SIZE_1 = 10;
const int TEST_SIZE_2 = 100;
const int TEST_MAXVALUE_1 = 10;
const int TEST_MAXVALUE_2 = INT_MAX;

Cyclic_array<int> gen_random(size_t size, int max_value) {
	Cyclic_array<int> result(size);
	for (int i = 0; i < (int)size; ++i) {
		result[i] = random(-max_value, max_value);
	}
	return result;
}

//#############################################################################

template<typename T>
void test_constructor_fill(size_t n, T value, int index) {
	Cyclic_array<T> a(n, value);
	EXPECT_EQ(a[index], value);
}

TEST (cyclic_array, constructor_fill) {
	test_constructor_fill(1, 5, 7);
	test_constructor_fill(8, -100, 7);
	test_constructor_fill(5, INT_MAX, 3);
	test_constructor_fill(100, 5, -7);
	test_constructor_fill(100, 'c', INT_MAX);

	for (int i = 0; i < COUNT_OF_TEST; i++) {
		test_constructor_fill(random(TEST_SIZE_1) + 1, TEST_MAXVALUE_1, INT_MAX);
		test_constructor_fill(random(TEST_SIZE_2) + 1, TEST_MAXVALUE_2, INT_MAX);
	}
}

void test_cycles (size_t n, int i, int di){
	Cyclic_array<int> arr = gen_random(n, INT_MAX);
	EXPECT_EQ(arr[i], arr[i + di*(int)n]);
}

TEST (cyclic_array, cycles) {
	test_cycles(1, 5, 7);
	test_cycles(5, 0, 1);
	test_cycles(10, 0, -3);

	for (int i = 0; i < COUNT_OF_TEST; i++) {
		test_cycles(random(TEST_SIZE_1) + 1, TEST_MAXVALUE_1, 5);
		test_cycles(random(TEST_SIZE_2) + 1, TEST_MAXVALUE_2, 10000);
	}
}

void change(int &x) {
	x = -x;
	if (x == 0) ++x;
}

void test_equal(Cyclic_array<int>& first, Cyclic_array<int>& second) {
	int index = random(-INT_MAX/2, INT_MAX/2);
	EXPECT_EQ(first[index], second[index]);
	change(first[index]);
	EXPECT_NE(first[index], second[index]);
	index++;
	change(second[index]);
	EXPECT_NE(first[index], second[index]);
}

void test_constructor_copy(size_t max_size, int max_value) {
	Cyclic_array<int> construct = gen_random(random(max_size) + 2, max_value);
	Cyclic_array<int> copy(construct);
	test_equal(copy, construct);
}

TEST (cyclic_array, constructor_copy) {
	for (int i = 0; i < COUNT_OF_TEST; i++) {
		test_constructor_copy(TEST_SIZE_1, TEST_MAXVALUE_1);
		test_constructor_copy(TEST_SIZE_2, TEST_MAXVALUE_2);
	}
}

void test_assign(size_t max_size, int max_value) {
	Cyclic_array<int> construct = gen_random(random(max_size) + 2, max_value);
	Cyclic_array<int> copy = gen_random(random(max_size) + 2, max_value);
	copy = construct;
	test_equal(copy, construct);
}

TEST (cyclic_array, assign) {
	for (int i = 0; i < COUNT_OF_TEST; i++) {
		test_assign(TEST_SIZE_1, TEST_MAXVALUE_1);
		test_assign(TEST_SIZE_2, TEST_MAXVALUE_2);
	}
}

TEST(cyclic_array, a_assign_a) {
	Cyclic_array<int> a(5);
	a[1] = 3;
	a = a;
	EXPECT_EQ(a[1], 3);
}

}