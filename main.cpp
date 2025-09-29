#include <iostream>
#include <complex>
#include <random>

constexpr double MY_PI = 3.14159265358979323846;

template<typename>
struct is_complex : std::false_type {};

template<typename T>
struct is_complex<std::complex<T>> : std::true_type {};

template<typename T>
struct Point {
	T x, y;

	Point() : x(), y() {}
	Point(const T& val) : x(val), y(val) {}
	Point(const T& x_, const T& y_) : x(x_), y(y_) {}
};

// Distance calculation
template<typename T>
struct DistanceHelper {
	static double distance(const Point<T>& a, const Point<T>& b) {
		if constexpr (std::is_floating_point_v<T>) {
			double dx = static_cast<double>(b.x) - static_cast<double>(a.x);
			double dy = static_cast<double>(b.y) - static_cast<double>(a.y);
			return std::sqrt(dx * dx + dy * dy);
		}
		else if constexpr (is_complex<T>::value) {
			double dx = std::abs(b.x - a.x);
			double dy = std::abs(b.y - a.y);
			return std::sqrt(dx * dx + dy * dy);
		}
		else {
			double dx = static_cast<double>(b.x) - static_cast<double>(a.x);
			double dy = static_cast<double>(b.y) - static_cast<double>(a.y);
			return std::sqrt(dx * dx + dy * dy);
		}
	}
};

template<typename T>
class Polyline {
private:
	Point<T>* points;
	std::size_t size;

public:
	static constexpr double precision = 1e-6;

	Polyline() : points(nullptr), size(0) {}

	explicit Polyline(const Point<T>& pt) : size(1) {
		points = new Point<T>[1];
		points[0] = pt;
	}

	explicit Polyline(const T& val) : size(1) {
		points = new Point<T>[1];
		points[0] = Point<T>(val);
	}

	explicit Polyline(std::size_t n) : size(n) {
		points = new Point<T>[n];
		for (std::size_t i = 0; i < n; ++i) {
			points[i] = Point<T>();
		}
	}

	Polyline(std::size_t n, T m1, T m2) : size(n) {
		points = new Point<T>[n];
		std::random_device rd;
		std::mt19937 gen(rd());

		if constexpr (std::is_integral_v<T>) {
			std::uniform_int_distribution<T> dist(m1, m2);
			for (std::size_t i = 0; i < n; ++i) {
				points[i] = Point<T>{ dist(gen), dist(gen) };
			}
		}
		else if constexpr (std::is_floating_point_v<T>) {
			std::uniform_real_distribution<T> dist(m1, m2);
			for (std::size_t i = 0; i < n; ++i) {
				points[i] = Point<T>{ dist(gen), dist(gen) };
			}
		}
		else if constexpr (is_complex<T>::value) {
			using VT = typename T::value_type;
			std::uniform_real_distribution<VT> dist_real(m1.real(), m2.real());
			std::uniform_real_distribution<VT> dist_imag(m1.imag(), m2.imag());
			for (std::size_t i = 0; i < n; ++i) {
				T x(dist_real(gen), dist_imag(gen));
				T y(dist_real(gen), dist_imag(gen));
				points[i] = Point<T>{ x, y };
			}
		}
		else {
			throw std::runtime_error("Unsupported type for random generation");
		}
	}

	Polyline(const Polyline& other) : size(other.size) {
		points = new Point<T>[size];
		for (std::size_t i = 0; i < size; ++i) {
			points[i] = other.points[i];
		}
	}

	~Polyline() {
		delete[] points;
	}

};


int main() {
	return 0;
}
