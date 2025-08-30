// программа для вычисления пересечения двух 3D отрезков

#include <iostream>
#include <cmath>
using namespace std;

class Vector3D {
public:
    double X;
    double Y;
    double Z;

    Vector3D(double x = 0, double y = 0, double z = 0) : X(x), Y(y), Z(z) {}

    // Операции с векторами
    Vector3D operator+(const Vector3D& other) const {
        return Vector3D(X + other.X, Y + other.Y, Z + other.Z);
    }

    Vector3D operator-(const Vector3D& other) const {
        return Vector3D(X - other.X, Y - other.Y, Z - other.Z);
    }

    // умножение на число
    Vector3D operator*(double scalar) const {
        return Vector3D(X * scalar, Y * scalar, Z * scalar);
    }

    // векторное произведение
    Vector3D CrossProduct(const Vector3D& other) const {
        return Vector3D(
            Y * other.Z - Z * other.Y,
            Z * other.X - X * other.Z,
            X * other.Y - Y * other.X
        );
    }

    double Length() const {
        return sqrt(X * X + Y * Y + Z * Z);
    }

    // Для вывода
    friend ostream& operator<<(ostream& os, const Vector3D& vec) {
        return os << "(" << vec.X << ", " << vec.Y << ", " << vec.Z << ")";
    }
};

class Segment3D {
public:
    Vector3D start;
    Vector3D end;

    Segment3D(const Vector3D& s, const Vector3D& e) : start(s), end(e) {}
};

Vector3D Intersect(const Segment3D& s1, const Segment3D& s2) {
    Vector3D r = s1.start - s2.start;
    Vector3D d1 = s1.end - s1.start;
    Vector3D d2 = s2.end - s2.start;

    Vector3D cross_d1_d2 = d1.CrossProduct(d2);
    double a = cross_d1_d2.Length();

    if (abs(a) < 1e-6) { // Параллельны или лежат на одной прямой
        return Vector3D(); // Возвращаем нулевой вектор как признак отсутствия пересечения
    }

    // Коэффициенты системы уравнений
    double A11 = d1.X;
    double A21 = d1.Y + d1.Z;
    double A12 = -(d2.X);
    double A22 = -(d2.Y + d2.Z);
    double B1 = s2.start.X - s1.start.X;
    double B2 = s2.start.Y - s1.start.Y + s2.start.Z - s1.start.Z;

    // решаем систему по методу Крамера
    double denominator = A11 * A22 - A12 * A21;
    double delta_t = B1 * A22 - B2 * A12;
    double delta_u = A11 * B2 - A21 * B1;

    double t = delta_t / denominator;
    double u = delta_u / denominator;

    if (t < 0 || t > 1 || u < 0 || u > 1) {
        return Vector3D(); // Точки пересечения нет
    }

    return s1.start + d1 * t;
}

int main() {
    Segment3D s1(Vector3D(0, 0, 0), Vector3D(1, 1, 1));
    Segment3D s2(Vector3D(0, 1, 0), Vector3D(1, 0, 1));

    Vector3D result = Intersect(s1, s2);

    if (result.Length() > 0) {
        cout << "Intersection point: " << result << endl;
    }
    else {
        cout << "There is no intersection" << endl;
    }

    return 0;
}