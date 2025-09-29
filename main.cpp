#include <iostream>
#include <complex>
#include <stdexcept>
#include <random>
#include <type_traits>
#include <iomanip>
#include <cmath>

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

	Polyline& operator=(const Polyline& other) {
		if (this == &other) return *this;

		delete[] points;
		size = other.size;
		points = new Point<T>[size];
		for (std::size_t i = 0; i < size; ++i) {
			points[i] = other.points[i];
		}
		return *this;
	}

	~Polyline() {
		delete[] points;
	}

	Point<T>& operator[](std::size_t idx) {
		if (idx >= size) {
			throw std::out_of_range("Index out of range");
		}
		return points[idx];
	}

	const Point<T>& operator[](std::size_t idx) const {
		if (idx >= size) {
			throw std::out_of_range("Index out of range");
		}
		return points[idx];
	}

	std::size_t length() const {
		return size;
	}

	// polyline + polyline
	Polyline operator+(const Polyline& other) const {
		Polyline result(size + other.size);
		for (std::size_t i = 0; i < size; ++i) {
			result.points[i] = points[i];
		}
		for (std::size_t i = 0; i < other.size; ++i) {
			result.points[size + i] = other.points[i];
		}
		return result;
	}

	// point + polyline end
	Polyline operator+(const Point<T>& p) const {
		Polyline result(size + 1);
		for (std::size_t i = 0; i < size; ++i) {
			result.points[i] = points[i];
		}
		result.points[size] = p;
		return result;
	}

	// point + polyline begin
	static friend Polyline operator+(const Point<T>& p, const Polyline& pl) {
		Polyline result(pl.size + 1);
		result.points[0] = p;
		for (std::size_t i = 0; i < pl.size; ++i) {
			result.points[i + 1] = pl.points[i];
		}
		return result;
	}

	double lengthPolyline() const {
		if (size < 2) {
			return 0.0;
		}
		double len = 0.0;
		for (std::size_t i = 1; i < size; ++i) {
			len += DistanceHelper<T>::distance(points[i - 1], points[i]);
		}
		return len;
	}

	bool operator==(const Polyline& other) const {
		if (size != other.size) {
			return false;
		}
		for (std::size_t i = 0; i < size; ++i) {
			if constexpr (std::is_floating_point_v<T>) {
				if (std::abs(points[i].x - other.points[i].x) > precision ||
					std::abs(points[i].y - other.points[i].y) > precision) {
					return false;
				}
			}
			else if constexpr (is_complex<T>::value) {
				using VT = typename T::value_type;
				if (std::abs(points[i].x.real() - other.points[i].x.real()) > precision ||
					std::abs(points[i].x.imag() - other.points[i].x.imag()) > precision ||
					std::abs(points[i].y.real() - other.points[i].y.real()) > precision ||
					std::abs(points[i].y.imag() - other.points[i].y.imag()) > precision) {
					return false;
				}
			}
			else {
				if (!(points[i].x == other.points[i].x && points[i].y == other.points[i].y)) {
					return false;
				}
			}
		}
		return true;
	}

	bool operator!=(const Polyline& other) const {
		return !(*this == other);
	}

	static friend std::ostream& operator<<(std::ostream& os, const Polyline& pl) {
		os << "Polyline with " << pl.size << " points:\n";
		for (std::size_t i = 0; i < pl.size; ++i) {
			os << "(" << pl.points[i].x << ", " << pl.points[i].y << ")";
			if (i + 1 < pl.size) {
				os << " <-> ";
			}
		}
		return os;
	}
};

template<typename T>
Polyline<T> makeIsoscelesTriangle(double alpha, double a) {
	double alpha_rad = alpha * MY_PI / 180.0;

	if (alpha_rad <= 0 || alpha_rad >= MY_PI) {
		throw std::invalid_argument("Alpha must be between 0 and PI (radians).");
	}

	Point<T> p0(0, 0);
	T x = static_cast<T>(a * std::cos(alpha_rad / 2));
	T y = static_cast<T>(a * std::sin(alpha_rad / 2));

	Point<T> p1(x, y);
	Point<T> p2(x, -y);

	Polyline<T> triangle(static_cast<std::size_t>(3));
	triangle[0] = p0;
	triangle[1] = p1;
	triangle[2] = p2;

	return triangle;
}

int main() {
	try {
		Polyline<int> p1(3, 0, 10);
		std::cout << p1 << "\nLength: " << p1.lengthPolyline() << "\n\n";

		Polyline<float> p2(2, 0.f, 5.f);
		std::cout << p2 << "\nLength: " << p2.lengthPolyline() << "\n\n";

		Polyline<double> p3(4, 0.0, 3.0);
		std::cout << p3 << "\nLength: " << p3.lengthPolyline() << "\n\n";

		using cfloat = std::complex<float>;
		Polyline<cfloat> p4(3, cfloat(0.f, 0.f), cfloat(5.f, 5.f));
		std::cout << p4 << "\nLength: " << p4.lengthPolyline() << "\n\n";

		using cdouble = std::complex<double>;
		Polyline<cdouble> p5(3, cdouble(0., 0.), cdouble(3., 3.));
		std::cout << p5 << "\nLength: " << p5.lengthPolyline() << "\n\n";

		Polyline<int> singlePointLine(Point<int>(10, 20));
		std::cout << "Single-point polyline:\n" << singlePointLine
			<< "\nLength: " << singlePointLine.lengthPolyline() << "\n\n";

		Polyline<float> defaultPointsLine(static_cast<std::size_t>(3));
		std::cout << "Polyline with 3 default float points:\n" << defaultPointsLine
			<< "\nLength: " << defaultPointsLine.lengthPolyline() << "\n\n";

		auto p6 = p1 + p1;
		std::cout << "p6 (p1 + p1):\n" << p6 << "\n";

		Point<int> pt(100);
		auto p7 = p6 + pt;
		std::cout << "p7 (p6 + pt):\n" << p7 << "\n";

		auto p8 = pt + p6;
		std::cout << "p8 (pt + p6):\n" << p8 << "\n";

		p8[0] = Point<int>(-5);
		std::cout << "p8 after modification:\n" << p8 << "\n\n";

		try {
			auto bad = p8[100];
		}
		catch (const std::out_of_range& e) {
			std::cout << "Caught expected exception: " << e.what() << "\n\n";
		}

		std::cout << std::boolalpha << "p6 == p6: " << (p6 == p6) << "\n";
		std::cout << "p6 != p7: " << (p6 != p7) << "\n";
		std::cout << "p6 == p7: " << (p6 == p7) << "\n";

		// Triangle example
		std::cout << "\nIsosceles triangle example:\n";
		double alpha = 60;
		double a = 12.0;

		auto triangle = makeIsoscelesTriangle<double>(alpha, a);

		std::cout << triangle << "\nLength: " << triangle.lengthPolyline() << "\n";
	}
	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}
}
