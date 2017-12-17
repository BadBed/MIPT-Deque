#include "pch.h"
#include "deque.h"
#include <vector>
#include <string>
#include <ctime>
#include <iostream>

extern int random(int max);
extern int random(int min, int max);

using std::vector;
using std::string;
using std::to_string;

class CEvent{
private:
	int type_;//0 push_back, 1 push_front, 2 pop_back, 3 pop_front, 4 []
	int i_, key_;
	static const int COUNT_OF_TYPES = 5;

public:
	CEvent (int min_key, int max_key):
		type_(0),
		i_(0),
		key_(random(min_key, max_key)) {}

	CEvent (int n, int min_key, int max_key):
		type_(random(COUNT_OF_TYPES - 1)),
		i_(random(n - 1)),
		key_(random(min_key, max_key)) {}

	bool is_correct(int& size) const;

	void do_it(Deque<int>& deque) const;

	string log() const;
};

bool CEvent::is_correct(int& size) const {
	if (type_ >= 2 && size == 0) return false;

	if (type_ == 0 || type_ == 1) ++size;
	if (type_ == 2 || type_ == 3) --size;

	return true;
}

void CEvent::do_it(Deque<int>& deque) const {
	if (type_ == 0) {
		deque.push_back(key_);
	}
	if (type_ == 1) {
		deque.push_front(key_);
	}
	if (type_ == 2) {
		deque.pop_back();
	}
	if (type_ == 3) {
		deque.pop_front();
	}
	if (type_ == 4) {
		deque[i_];
	}
}

string CEvent::log() const {
	string result = to_string(type_) + " ";

	if (type_ == 0 || type_ == 1)
		result += to_string(key_);

	if (type_ == 4)
		result += to_string(i_);

	return result + "\n";
}

vector<CEvent> gen_random_events(size_t count_of_events, int min_key, int max_key) {
	vector<CEvent> events;
	int deque_size = 0;
	events.push_back(CEvent(min_key, max_key));
	events[0].is_correct(deque_size);

	while (events.size() < count_of_events) {
		CEvent new_event = CEvent(deque_size, min_key, max_key);
		if (new_event.is_correct(deque_size))
			events.push_back(new_event);
	}
	return events;
}

TEST(time, this_is_the_only_time_test_what_you_want_to_find_there) {
	size_t count_of_iter = (size_t)1e6;
	Deque<int> deque;
	vector<CEvent> events = gen_random_events(count_of_iter, -100, 100);

	clock_t start = clock();
	size_t i = 0;
	try {
		for (i = 0; i < count_of_iter; ++i) {
			events[i].do_it(deque);
		}
	}
	catch (...) {
		string log;
		for (size_t j = 0; j <= i; ++j)
			log += events[i].log();
		std::cout << "Error log is:\n" << log;
	}
	clock_t end = clock();

	std::cout << count_of_iter << " iterations, time: " << (float)(end - start)/CLK_TCK << "\n";
	//0.095
}
