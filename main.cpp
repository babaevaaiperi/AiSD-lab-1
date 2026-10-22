#include <iostream>
#include <random>
#include <cmath>

template <typename A, typename B>
std::ostream& operator<<(std::ostream& os, const std::pair<A, B>& p) {
	return os << '(' << p.first << ", " << p.second << ')';
}

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
		static_assert(std::is_arithmetic_v<T>,
			"Random filling is available only for numeric types");
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

	T operator[](int index) const {
		if (index < 0 || index >= count) {
			throw std::out_of_range("index out of bounds");
		}
		return data[index];
	}

	friend std::ostream& operator<< (std::ostream& os, const MySet& s) {
		os << "{ ";
		for (int i = 0;i < s.count;++i) {
			os << s.data[i] << " ";
		};
		os << "}";
		return os;
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

MySet<int> findDuplicatesInArray(const int* arr, int n) {
	MySet<int> seen;
	MySet<int> duplicates;
	for (int i = 0; i < n; ++i) {
		if (seen.contains(arr[i])) duplicates += arr[i];
		else seen += arr[i];
	}
	return duplicates;
}


template <typename T>
void runUserDemo() {
	int n;
	std::cout << "\n--- Create Set A ---\n";
	std::cout << "Enter number of elements: ";
	std::cin >> n;
	T* arrA = new T[n];
	for (int i = 0; i < n; ++i) {
		std::cout << "  [" << i << "]: ";
		std::cin >> arrA[i];
	}
	MySet<T> a(arrA, n);
	delete[] arrA;

	std::cout << "\n--- Create Set B ---\n";
	std::cout << "Enter number of elements: ";
	std::cin >> n;
	T* arrB = new T[n];
	for (int i = 0; i < n; ++i) {
		std::cout << "  [" << i << "]: ";
		std::cin >> arrB[i];
	}
	MySet<T> b(arrB, n);
	delete[] arrB;

	std::cout << "\na = " << a << "\n";
	std::cout << "b = " << b << "\n";
	std::cout << "a + b = " << (a + b) << "\n";
	std::cout << "a - b = " << (a - b) << "\n";
	std::cout << "a intersection b = " << a.intersection(b) << "\n";
}

template <typename T>
void demoOperations(const MySet<T>& a, const MySet<T>& b, const T& extra) {
	std::cout << "a = " << a << std::endl;
	std::cout << "b = " << b << std::endl;
	std::cout << "a[0] = " << a[0] << std::endl;
	std::cout << "a + b (union) = " << (a + b) << std::endl;
	std::cout << "a - b (difference) = " << (a - b) << std::endl;
	std::cout << "a intersection b = " << a.intersection(b) << std::endl;
	std::cout << "a + extra = " << (a + extra) << std::endl;
	std::cout << "a - a[0] = " << (a - a[0]) << std::endl;

	MySet<T> c(a);
	c += extra;
	std::cout << "c += extra: " << c << std::endl;
	c -= extra;
	std::cout << "c -= extra: " << c << std::endl;

	std::cout << "a contains a[0]? " << (a.contains(a[0]) ? "yes" : "no") << std::endl;
	std::cout << "a contains extra? " << (a.contains(extra) ? "yes" : "no") << std::endl;
	std::cout << "a == c? " << (a == c ? "yes" : "no") << std::endl;
	std::cout << "a != b? " << (a != b ? "yes" : "no") << std::endl;

	try {
		std::cout << a[a.size()] << std::endl;
	}
	catch (const std::out_of_range& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
}


int main() {
	try {
		std::cout << "========== AUTO DEMO (ALL OPERATORS) ==========\n";

		std::cout << "\n--- INT DEMO ---\n";
		MySet<int> intA(5, 1, 10);
		MySet<int> intB(4, 5, 15);
		demoOperations(intA, intB, 100);

		int raw[] = { 3, 7, 3, 9 };
		MySet<int> fromArray(raw, 4);
		std::cout << "from array {3, 7, 3, 9} = " << fromArray << "\n";

		std::cout << "\n--- FLOAT DEMO ---\n";
		MySet<float> floatA(4, 1.0f, 5.0f);
		MySet<float> floatB(4, 3.0f, 7.0f);
		demoOperations(floatA, floatB, 99.5f);

		std::cout << "\n--- STRING DEMO ---\n";
		std::string wordsA[] = { "apple", "banana", "apple", "cherry" };
		std::string wordsB[] = { "banana", "date", "elderberry" };
		MySet<std::string> strA(wordsA, 4);
		MySet<std::string> strB(wordsB, 3);
		demoOperations(strA, strB, std::string("fig"));

		std::cout << "\n--- PAIR DEMO ---\n";
		std::pair<int, double> pairsA[] = { { 1, 2.5 },{ 3, 4.5 },{ 1, 2.5 } };
		std::pair<int, double> pairsB[] = { { 3, 4.5 },{ 5, 6.5 } };
		MySet<std::pair<int, double>> pA(pairsA, 3);
		MySet<std::pair<int, double>> pB(pairsB, 2);
		demoOperations(pA, pB, std::make_pair(9, 0.5));

		std::cout << "\n--- TASK: DUPLICATES ---\n";
		int taskArr[] = { 1, 2, 3, 4, 2, 5, 6, 3, 7, 1, 8, 9, 8 };
		std::cout << "Source array: ";
		for (int i = 0; i < 13; ++i) std::cout << taskArr[i] << " ";
		std::cout << "\nDuplicates: " << findDuplicatesInArray(taskArr, 13) << "\n";

	
		std::cout << "\n========== USER INPUT ==========\n";

		while (true) {
			std::cout << "\nSelect type for interactive input:\n";
			std::cout << "  [1] int\n";
			std::cout << "  [2] float\n";
			std::cout << "  [3] string\n";
			std::cout << "  [0] exit\n";
			std::cout << "Choice: ";

			int choice;
			std::cin >> choice;

			if (choice == 0) break;

			if (choice == 1) {
				runUserDemo<int>();
			}
			else if (choice == 2) {
				runUserDemo<float>();
			}
			else if (choice == 3) {
				runUserDemo<std::string>();
			}
			else {
				std::cout << "Invalid choice.\n";
			}
		}
	}
	catch (const std::exception& e) {
		std::cout << "Fatal error: " << e.what() << "\n";
	}

	return 0;
}