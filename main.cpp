#include <iostream>

template <typename T>
class MySet {
private:
	T* data;
	int count;
	int capacity;
public:
	MySet() : data(nullptr), count(0), capacity(0) {};
	int size() const { return count; }

};

int main() {
	MySet <int> s;
	std::cout << "size= " << s.size() << "\n";
	return 0;
}