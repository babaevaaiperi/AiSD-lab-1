#include <iostream>
#include <random>

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

	void fillRandom(int n, T minVal, T maxVal) {
		std::random_device rd;
		std::mt19937 gen(rd());
		if constexpr (std::is_integral_v<T>) {
			std::uniform_int_distribution<long long> dist(
				static_cast<long long>(minVal), static_cast<long long>(maxVal));
			int attempts = 0;
			while (count < n && attempts < n * 100) {
				add(static_cast<T>(dist(gen)));
				++attempts;
			}
		}
		else {
			std::uniform_real_distribution<double> dist(
				static_cast<double>(minVal), static_cast<double>(maxVal));
			int attempts = 0;
			while (count < n && attempts < n * 100) {
				add(static_cast<T>(dist(gen)));
				++attempts;
			}
		}
		if (count < n)
			throw std::runtime_error("Cannot fill set with unique values");
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

	MySet(const T* arr, int n) : MySet() {
		if (arr == nullptr && n > 0)
			throw std::invalid_argument("Array pointer is null");
		for (int i = 0; i < n; ++i)
			add(arr[i]);
	}

	MySet(int n, T minVal, T maxVal) : MySet() {
		static_assert(std::is_arithmetic_v<T>,
			"Random filling is available only for numeric types");
		if (n <= 0) throw std::invalid_argument("Count must be positive");
		if (minVal > maxVal) throw std::invalid_argument("Invalid range");
		fillRandom(n, minVal, maxVal);
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
	int raw[] = { 3, 7, 3, 9 };
	MySet<int> s(raw, 4);
	std::cout << s << "\n";   

	MySet<int> r(5, 1, 100);
	std::cout << "random: " << r << " size=" << r.size() << "\n";  

}