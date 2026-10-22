#include <iostream>

template <typename T>
class MySet {
private:
	T* data;
	int count;
	int capacity;
public:
	//MySet() : data(nullptr), count(0), capacity(0) {};

	MySet() {
		data = new T[10];
		count = 0;
		capacity = 10;
	};
	
	int size() const { return count; }

	bool contains(const T& item) const {
		for (int i = 0;i < count;++i) {
			if (data[i] == item) {
				return true;
			}
		}
		return false;
	}

	void add(const T& item) {
		if (contains(item)) {
			return;
		}
		data[count] = item;
		count++;
	}

	~MySet() {
		delete[] data;
	}
};

int main() {

	MySet<std::string> names;
	std::cout << "size = " << names.size() << "\n";

	names.add("apple");
	names.add("banana");
	names.add("apple");
	std::cout << names.contains("banana") << "\n";  

	/*MySet <int> s;
	s.add(5);
	std::cout << s.contains(5) << "\n";
	std::cout << "size= " << s.size() << "\n";
	return 0;*/
}