#pragma once

#include <cmath>
#include <memory>
#include <vector>
#include <stdexcept>

using namespace std;

const double PI = 3.14159265358979323846;

struct Point3D {
    double x, y, z;
    Point3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
};

struct Vector3D {
    double x, y, z;
    Vector3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
};

class Curve3D { // ������� �����
public:
    virtual ~Curve3D() = default; // ����������� ���������� �� ��������� (����������� - ��� ��������� ������ ������)
    virtual Point3D getPoint(double t) const = 0; // ����������� ������� (��-�� �const� �� ������ ���� ������ (��� �������-������))
    virtual Vector3D getDerivative(double t) const = 0;
    virtual double getRadius() const = 0;
};

class Circle : public Curve3D { // ����� Circle ��������� �� ������ Curve3D
private:
    double radius;
public:
    Circle(double r) : radius(r) { // �����������
        if (r <= 0) throw invalid_argument("Radius must be positive");
    }

    Point3D getPoint(double t) const override { // �������� ����� �override� ���� ���������, ��� ���� ����� �������������� ����������� ����� �������� ������
        return Point3D(radius * cos(t), radius * sin(t), 0);
    }

    Vector3D getDerivative(double t) const override {
        return Vector3D(-radius * sin(t), radius * cos(t), 0);
    }

    double getRadius() const override { return radius; }
};

class Ellipse : public Curve3D {
private:
    double radiusX, radiusY;
public:
    Ellipse(double rx, double ry) : radiusX(rx), radiusY(ry) {
        if (rx <= 0 || ry <= 0) throw invalid_argument("Radii must be positive"); // ����
    }

    Point3D getPoint(double t) const override {
        return Point3D(radiusX * cos(t), radiusY * sin(t), 0);
    }

    Vector3D getDerivative(double t) const override {
        return Vector3D(-radiusX * sin(t), radiusY * cos(t), 0);
    }

    double getRadius() const override { return (radiusX + radiusY) / 2.0; }
};

class Helix : public Curve3D {
private:
    double radius, step;
public:
    Helix(double r, double s) : radius(r), step(s) {
        if (r <= 0) throw invalid_argument("Radius must be positive");
    }

    Point3D getPoint(double t) const override {
        return Point3D(radius * cos(t), radius * sin(t), step * t / (2 * PI));
    }

    Vector3D getDerivative(double t) const override {
        return Vector3D(-radius * sin(t), radius * cos(t), step / (2 * PI));
    }

    double getRadius() const override { return radius; }
};

// Factory functions
shared_ptr<Curve3D> createRandomCurve(); // �shared_ptr� � ����� ��������� (������ ������������� ������������� ����� ������ �� ������� ���������)
vector<shared_ptr<Curve3D>> populateCurves(int count);
vector<shared_ptr<Circle>> extractCircles(const vector<shared_ptr<Curve3D>>& curves);
double sumRadii(const vector<shared_ptr<Circle>>& circles);