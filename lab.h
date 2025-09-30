#ifndef LAB1_H
#define LAB1_H

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
    static double distance(const Point<T>& a, const Point<T>& b);
};

template<typename T>
class Polyline {
private:
    Point<T>* points;
    std::size_t size;

public:
    static constexpr double precision = 1e-6;

    Polyline();
    explicit Polyline(const Point<T>& pt);
    explicit Polyline(const T& val);
    explicit Polyline(std::size_t n);
    Polyline(std::size_t n, T m1, T m2);
    Polyline(const Polyline& other);
    Polyline& operator=(const Polyline& other);
    ~Polyline();

    Point<T>& operator[](std::size_t idx);
    const Point<T>& operator[](std::size_t idx) const;
    std::size_t length() const;

    // polyline + polyline
    Polyline operator+(const Polyline& other) const;

    // point + polyline end
    Polyline operator+(const Point<T>& p) const;

    // point + polyline begin
    template<typename U>
    friend Polyline<U> operator+(const Point<U>& p, const Polyline<U>& pl);

    double lengthPolyline() const;

    bool operator==(const Polyline& other) const;
    bool operator!=(const Polyline& other) const;

    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const Polyline<U>& pl);
};

template<typename T>
Polyline<T> makeIsoscelesTriangle(double alpha, double a);


#include "lab1.cpp"

#endif
