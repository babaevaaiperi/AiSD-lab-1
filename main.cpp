#include <iostream>

template <typename T>
bool areEqual(const T& a, const T& b, double) {
	return a == b;
}

inline bool areEqual(float a, float b, double eps) {
	return std::fabs(a - b) < eps;
}

inline bool areEqual(double a, double b, double eps) {
	return std::fabs(a - b) < eps;
}

template <typename T>
class MySet {
private:
	T* data;
	int count;

	static constexpr double EPS = 1e-6;

	static bool equalElems(const T& a, const T& b) {
		return areEqual(a, b, EPS);
	}

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
			if (equalElems(data[i], item)) {
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

	bool operator==(const MySet& other) const {
		if (count != other.count) return false;
		for (int i = 0; i < count; ++i)
			if (!other.contains(data[i])) return false;
		return true;
	}

	bool operator!=(const MySet& other) const {
		return !(*this == other);
	}

	MySet operator+(const MySet& other) const {
		MySet result(*this);
		for (int i = 0; i < other.count; ++i)
			result.add(other.data[i]);
		return result;
	}

	MySet operator-(const MySet& other) const {
		MySet result;
		for (int i = 0; i < count; ++i)
			if (!other.contains(data[i]))
				result.add(data[i]);
		return result;
	}

	MySet intersection(const MySet& other) const {
		MySet result;
		for (int i = 0; i < count; ++i)
			if (other.contains(data[i]))
				result.add(data[i]);
		return result;
	}

	MySet operator+(const T& value) const {
		MySet result(*this);
		result += value;
		return result;
	}

	MySet& operator+=(const T& value) {
		add(value);
		return *this;
	}

	MySet operator-(const T& value) const {
		MySet result(*this);
		result -= value;
		return result;
	}

	MySet& operator-=(const T& value) {
		for (int i = 0; i < count; ++i) {
			if (equalElems(data[i], value)) {
				for (int j = i; j + 1 < count; ++j)
					data[j] = data[j + 1];
				count--;
				return *this;
			}
		}
		return *this;
	}

};

int main() {
	MySet<float> f;
	f.add(0.1f + 0.2f);
	std::cout << f.contains(0.3f) << "\n";  

	MySet<int> a; a.add(1); a.add(2); a.add(3);
	MySet<int> b; b.add(2); b.add(3); b.add(4);
	std::cout << "a+b = " << (a + b) << "\n";   
	std::cout << "a-b = " << (a - b) << "\n";   
	std::cout << "a&b = " << a.intersection(b) << "\n";  

	MySet<int> c; c.add(1); c.add(2);
	std::cout << "c + 5 = " << (c + 5) << "\n";     
	std::cout << "c - 1 = " << (c - 1) << "\n";    
	MySet<int> d = c;
	d += 10; d -= 1;
	std::cout << "d = " << d << "\n";              

}