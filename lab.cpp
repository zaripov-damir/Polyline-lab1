#ifndef LAB1_CPP
#define LAB1_CPP

#include "lab1.h"

// DistanceHelper implementation
template<typename T>
double DistanceHelper<T>::distance(const Point<T>& a, const Point<T>& b) {
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

// Polyline implementation
template<typename T>
Polyline<T>::Polyline() : points(nullptr), size(0) {}

template<typename T>
Polyline<T>::Polyline(const Point<T>& pt) : size(1) {
    points = new Point<T>[1];
    points[0] = pt;
}

template<typename T>
Polyline<T>::Polyline(const T& val) : size(1) {
    points = new Point<T>[1];
    points[0] = Point<T>(val);
}

template<typename T>
Polyline<T>::Polyline(std::size_t n) : size(n) {
    points = new Point<T>[n];
    for (std::size_t i = 0; i < n; ++i) {
        points[i] = Point<T>();
    }
}

template<typename T>
Polyline<T>::Polyline(std::size_t n, T m1, T m2) : size(n) {
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

template<typename T>
Polyline<T>::Polyline(const Polyline& other) : size(other.size) {
    points = new Point<T>[size];
    for (std::size_t i = 0; i < size; ++i) {
        points[i] = other.points[i];
    }
}

template<typename T>
Polyline<T>& Polyline<T>::operator=(const Polyline& other) {
    if (this == &other) return *this;

    delete[] points;
    size = other.size;
    points = new Point<T>[size];
    for (std::size_t i = 0; i < size; ++i) {
        points[i] = other.points[i];
    }
    return *this;
}

template<typename T>
Polyline<T>::~Polyline() {
    delete[] points;
}

template<typename T>
Point<T>& Polyline<T>::operator[](std::size_t idx) {
    if (idx >= size) {
        throw std::out_of_range("Index out of range");
    }
    return points[idx];
}

template<typename T>
const Point<T>& Polyline<T>::operator[](std::size_t idx) const {
    if (idx >= size) {
        throw std::out_of_range("Index out of range");
    }
    return points[idx];
}

template<typename T>
std::size_t Polyline<T>::length() const {
    return size;
}

template<typename T>
Polyline<T> Polyline<T>::operator+(const Polyline& other) const {
    Polyline result(size + other.size);
    for (std::size_t i = 0; i < size; ++i) {
        result.points[i] = points[i];
    }
    for (std::size_t i = 0; i < other.size; ++i) {
        result.points[size + i] = other.points[i];
    }
    return result;
}

template<typename T>
Polyline<T> Polyline<T>::operator+(const Point<T>& p) const {
    Polyline result(size + 1);
    for (std::size_t i = 0; i < size; ++i) {
        result.points[i] = points[i];
    }
    result.points[size] = p;
    return result;
}

template<typename T>
Polyline<T> operator+(const Point<T>& p, const Polyline<T>& pl) {
    Polyline<T> result(pl.size + 1);
    result.points[0] = p;
    for (std::size_t i = 0; i < pl.size; ++i) {
        result.points[i + 1] = pl.points[i];
    }
    return result;
}

template<typename T>
double Polyline<T>::lengthPolyline() const {
    if (size < 2) {
        return 0.0;
    }
    double len = 0.0;
    for (std::size_t i = 1; i < size; ++i) {
        len += DistanceHelper<T>::distance(points[i - 1], points[i]);
    }
    return len;
}

template<typename T>
bool Polyline<T>::operator==(const Polyline& other) const {
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

template<typename T>
bool Polyline<T>::operator!=(const Polyline& other) const {
    return !(*this == other);
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Polyline<T>& pl) {
    os << "Polyline with " << pl.size << " points:\n";
    for (std::size_t i = 0; i < pl.size; ++i) {
        os << "(" << pl.points[i].x << ", " << pl.points[i].y << ")";
        if (i + 1 < pl.size) {
            os << " <-> ";
        }
    }
    return os;
}

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

#endif
