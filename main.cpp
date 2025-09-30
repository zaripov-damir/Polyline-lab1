#include "lab1.h"

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
