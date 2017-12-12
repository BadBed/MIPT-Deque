#include <cassert>
#include <cstring>
#include <type_traits>
#include <iterator>

//!              Interface

template<typename T, bool IS_CONST>
class my_deque_iterator;
	
template<typename T>
class Cyclic_array{
private:
	T* array_;
	size_t size_;

	static int mod(int a, int b);
public:
	Cyclic_array();
	explicit Cyclic_array(size_t size);
	Cyclic_array(size_t size, T value);
	Cyclic_array(const Cyclic_array& constructor);
	~Cyclic_array();
	Cyclic_array& operator=(const Cyclic_array& other);

	T& operator[] (int index);
	const T& operator[] (int index) const;
	size_t size() const {return size_;}
};

template<typename T>
class Deque
{
private:
	Cyclic_array<T> array_;
	size_t size_;
	int first_;

	static const size_t FACTOR_SIZE_CRITICAL = 4;
	static const size_t FACTOR_SIZE_RESERVE = 2;

	void resize(size_t new_size);
public:
	Deque();
	Deque(const Deque& constructor);
	~Deque();

	size_t size() const;
	bool empty() const;

	void push_back(const T& element);
	void push_front(const T& element);
	void pop_back();
	void pop_front();
	T& operator[](int index);
	const T& operator[](int index) const;

	T& back() {return operator[](size_ - 1);}
	T& front() {return operator[](0);}
	const T& back() const {return operator[](size_ - 1);}
	const T& front() const {return operator[](0);}

	typedef my_deque_iterator<T, false> iterator;
	typedef my_deque_iterator<T, true> const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	iterator begin() {return iterator(this, 0);}
	const_iterator begin() const {return const_iterator(this, 0);} 
	const_iterator cbegin() const {return const_iterator(this, 0);}

	iterator end() {return iterator(this, size_);}
	const_iterator end() const {return const_iterator(this, size_);} 
	const_iterator cend() const {return const_iterator(this, size_);}

	reverse_iterator rbegin() {return reverse_iterator(end());}
	const_reverse_iterator rbegin() const {return const_reverse_iterator(cend());}
	const_reverse_iterator crbegin() const {return const_reverse_iterator(cend());}

	reverse_iterator rend() {return reverse_iterator(begin());}
	const_reverse_iterator rend() const {return const_reverse_iterator(cbegin());}
	const_reverse_iterator crend() const {return const_reverse_iterator(cbegin());}
};

template<typename T, bool IS_CONST>
class my_deque_iterator :
		public std::iterator<std::random_access_iterator_tag, 
		typename std::conditional<IS_CONST, const T, T>::type, int> {
private:
	typedef typename std::conditional<IS_CONST, const Deque<T>*, Deque<T>*>::type Deq;
	typedef typename std::conditional<IS_CONST, const T, T>::type output_value_type;

	Deq container_;
	int index_;
	my_deque_iterator(Deq container, int index):
		container_(container), index_(index) {}
public:
	friend class Deque<T>;
	my_deque_iterator() : container_(nullptr), index_(0) {}

	my_deque_iterator<T, IS_CONST>& operator++();
	my_deque_iterator<T, IS_CONST> operator++(int);
	my_deque_iterator<T, IS_CONST>& operator--();
	my_deque_iterator<T, IS_CONST> operator--(int);

	my_deque_iterator<T, IS_CONST>& operator+=(int n);
	my_deque_iterator<T, IS_CONST> operator+(int n) const;
	my_deque_iterator<T, IS_CONST>& operator-=(int n);
	my_deque_iterator<T, IS_CONST> operator-(int n) const;
	int operator-(const my_deque_iterator<T, IS_CONST>& other) const;

	bool operator<=(const my_deque_iterator<T, IS_CONST>& other) const 
		{ return index_ <= other.index_; }
	bool operator>=(const my_deque_iterator<T, IS_CONST>& other) const 
		{ return index_ >= other.index_; }
	bool operator<(const my_deque_iterator<T, IS_CONST>& other) const 
		{ return index_ < other.index_; }
	bool operator>(const my_deque_iterator<T, IS_CONST>& other) const 
		{ return index_ > other.index_; }
	bool operator==(const my_deque_iterator<T, IS_CONST>& other) const 
		{ return index_ == other.index_; }
	bool operator!=(const my_deque_iterator<T, IS_CONST>& other) const 
		{ return index_ != other.index_; }

	output_value_type& operator*() {return (*container_)[index_];}
	output_value_type& operator[](int n) {return (*container_)[index_ + n];}
	output_value_type* operator->() {return &(operator*());}
};

//                                  Implementation
//cyclic array
//########################################################################################

template<typename T>
Cyclic_array<T>::Cyclic_array(): array_(nullptr), size_(0) {}

template<typename T>
Cyclic_array<T>::Cyclic_array(size_t size): array_(new T[size]), size_(size) {}

template<typename T>
Cyclic_array<T>::Cyclic_array(size_t size, T value): array_(new T[size]), size_(size) {
	for (size_t i = 0; i < size_; i++) {
		array_[i] = value;
	}
}

template<typename T>
Cyclic_array<T>::~Cyclic_array() {
	delete[] array_;
}

template<typename T>
T& Cyclic_array<T>::operator[] (int index) {
	if (size() == 0) throw "My exeption: call to empty cyclic array";//
	return array_[mod(index, size_)];
}

template<typename T>
const T& Cyclic_array<T>::operator[] (int index) const {
	if (size() == 0) throw "My exeption: call to empty cyclic array";//
	return array_[mod(index, size_)];
}

template<typename T>
int Cyclic_array<T>::mod(int a, int b) {
	int result = a % b;
	if (result < 0) result += b;
	return result;
}

template<typename T>
Cyclic_array<T>::Cyclic_array(const Cyclic_array& constructor):
array_(new T[constructor.size_]), size_(constructor.size_) {
	memcpy(array_, constructor.array_, size_*sizeof(T));
}

template<typename T>
Cyclic_array<T>& Cyclic_array<T>::operator=(const Cyclic_array& other) {
	if (array_ == other.array_) return (*this);

	size_ = other.size_;
	delete[] array_;
	array_ = new T[size_];
	memcpy(array_, other.array_, size_*sizeof(T));

	return *this;
}

//deque
//########################################################################################

template<typename T>
void Deque<T>::resize(size_t new_size) {
	Cyclic_array<T> new_array(new_size);
	for (int i = first_; i < first_ + (int)size_; ++i) {
		new_array[i] = array_[i];
	}
	array_ = new_array;
}

template<typename T>
Deque<T>::Deque():array_(Cyclic_array<T>(1)), size_(0), first_(0) {}

template<typename T>
Deque<T>::Deque(const Deque& constructor):
array_(constructor.array_), size_(constructor.size_), first_(constructor.first_) {}

template<typename T>
Deque<T>::~Deque() {}

template<typename T>
size_t Deque<T>::size() const {
	return size_;
}

template<typename T>
bool Deque<T>::empty() const {
	return size_ == 0;
}

template<typename T>
T& Deque<T>::operator[](int index) {
	if(index < 0 || index >= (int)size_) throw "My exeption: Deque: index is out of range";
	return array_[first_ + index];
}

template<typename T>
const T& Deque<T>::operator[](int index) const {
	if(index < 0 || index >= (int)size_) throw "My exeption: Deque: index is out of range";
	return array_[first_ + index];
}

template<typename T>
void Deque<T>::push_back(const T& element) {
	if (array_.size() == 0) resize(1);
	if (size_ == array_.size()) resize(size_*FACTOR_SIZE_RESERVE);

	array_[first_ + size_] = element;
	++size_;
}

template<typename T>
void Deque<T>::push_front(const T& element) {
	if (array_.size() == 0) resize(1); 
	if (size_ == array_.size()) resize(size_*FACTOR_SIZE_RESERVE);

	array_[first_ - 1] = element;
	++size_;
	--first_;
}

template<typename T>
void Deque<T>::pop_back() {
	if(empty()) throw "My exeption: using pop with empty Deque";
	if (size_*FACTOR_SIZE_CRITICAL <= array_.size()) resize(size_*FACTOR_SIZE_RESERVE);

	--size_;
}

template<typename T>
void Deque<T>::pop_front() {
	if(empty()) throw "My exeption: using pop with empty Deque";
	if (size_*FACTOR_SIZE_CRITICAL <= array_.size()) resize(size_*FACTOR_SIZE_RESERVE);

	++first_;
	--size_;
}

//iterators
//########################################################################################

template<typename T, bool IS_CONST>
my_deque_iterator<T, IS_CONST>& my_deque_iterator<T, IS_CONST>::operator++() {
	++index_;
	return *this;
}

template<typename T, bool IS_CONST>
my_deque_iterator<T, IS_CONST>& my_deque_iterator<T, IS_CONST>::operator--() {
	--index_;
	return *this;
}

template<typename T, bool IS_CONST>
my_deque_iterator<T, IS_CONST> my_deque_iterator<T, IS_CONST>::operator++(int) {
	my_deque_iterator<T, IS_CONST> result = (*this);
	++index_;
	return result;
}

template<typename T, bool IS_CONST>
my_deque_iterator<T, IS_CONST> my_deque_iterator<T, IS_CONST>::operator--(int) {
	my_deque_iterator<T, IS_CONST> result = (*this);
	--index_;
	return result;
}

template<typename T, bool IS_CONST>
my_deque_iterator<T, IS_CONST>& my_deque_iterator<T, IS_CONST>::operator+=(int n) {
	index_ += n;
	return *this;
}

template<typename T, bool IS_CONST>
my_deque_iterator<T, IS_CONST>& my_deque_iterator<T, IS_CONST>::operator-=(int n) {
	index_ -= n;
	return *this;
}

template<typename T, bool IS_CONST>
my_deque_iterator<T, IS_CONST> my_deque_iterator<T, IS_CONST>::operator+(int n) const {
	my_deque_iterator<T, IS_CONST> result = (*this);
	result += n;
	return result;
}

template<typename T, bool IS_CONST>
my_deque_iterator<T, IS_CONST> my_deque_iterator<T, IS_CONST>::operator-(int n) const {
	my_deque_iterator<T, IS_CONST> result = (*this);
	result -= n;
	return result;
}

template<typename T, bool IS_CONST>
int my_deque_iterator<T, IS_CONST>::operator-(const my_deque_iterator<T, IS_CONST>& other) const {
	return index_ - other.index_;
}
