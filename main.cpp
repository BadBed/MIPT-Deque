#include "pch.h"
#include "deque.h"
#include <deque>
#include <iostream>
using std::cout;

void hand_test();
int test_all(int argc, char **argv);

int main(int argc, char **argv) {
	if (false) {
		hand_test();
		system("pause");
		return 0;
	}
	else {
		return test_all(argc, argv);
	}
}

int test_all(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();
	system("pause");
	return ret;
}

void hand_test() {
	int n = 5;
	std::deque<int> deq(5);
	std::deque<int> deq2(6);
	std::deque<int>::iterator it;
	std::deque<int>::iterator it2;
	for (int i = 0; i < n; i++) {
		deq.push_back(i);
	}

	it = deq.begin();
	it2 = deq2.begin();

	//if (it == it2) cout << "kek";
}