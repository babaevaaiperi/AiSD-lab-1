#include <iostream>

template <typename T>
class MySet {
private:
	T* data;
	int count;
public:
	MySet() : data(nullptr), count(0) {};

	MySet(const MySet& other) : data(nullptr), count(0) {
		if (other.count > 0) {
			data = new T[other.count];
			for (int i = 0;i < other.count;++i) {
				data[i] = other.data[i];
			}
		}
		count = other.count;
	}

	MySet& operator=(const MySet& other) {
		if (this != &other) {
			delete[] data;
			data = nullptr;
			count = 0;
			if (other.count > 0) {
				data = new T[other.count];
				for (int i = 0; i < other.count; ++i) {
					data[i] = other.data[i];
				}
			}
			count = other.count;
		}
		return *this;
	}

	~MySet() {
		delete[] data;
	}

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
		T* newData = new T[count + 1];
		for (int i = 0;i < count;++i) {
			newData[i] = data[i];
		}
		newData[count] = item;
		delete[] data;
		data = newData;
		count++;
	}

	friend std::ostream& operator<< (std::ostream& os, const MySet& s) {
		os << "{ ";
		for (int i = 0;i < s.count;++i) {
			os << s.data[i] << " ";
		};
		os << "}";
		return os;
	}

	T operator[](int index) const {
		if (index < 0 || index >= count) {
			throw std::out_of_range("index out of bounds");
		}
		return data[index];
	}

};

int main() {
	MySet<int> a; a.add(1); a.add(2);
	MySet<int> b = a; b.add(3);
	std::cout << "a.size=" << a.size() << " b.size=" << b.size() << "\n";  

}