#include "curves.h"
#include <random>
#include <algorithm>
#include <stdexcept>
#include <iostream>

using namespace std;

shared_ptr<Curve3D> createRandomCurve() {
    static random_device rd; // �������� ��������� �������� �����������
    static mt19937 gen(rd()); // ������� �������� ��������� - 32-��������� ����������� ��������� ����� �������� (Mersenne Twister)
    static uniform_int_distribution<> typeDist(0, 2); // ��������� ����� ����� �� 0 �� 2 ������������
    static uniform_real_distribution<> paramDist(1.0, 10.0); // ��������� ������������ ����� �� 1 �� 10 ������������

    int type = typeDist(gen);

    try {
        switch (type) {
        case 0: // Circle
            return make_shared<Circle>(paramDist(gen));
        case 1: // Ellipse
            return make_shared<Ellipse>(paramDist(gen), paramDist(gen));
        case 2: // Helix
            return make_shared<Helix>(paramDist(gen), paramDist(gen));
        default:
            return make_shared<Circle>(paramDist(gen));
        }
    }
    catch (const exception& e) {
        // Fallback to circle if invalid parameters
        return make_shared<Circle>(paramDist(gen));
    }
}

vector<shared_ptr<Curve3D>> populateCurves(int count) {
    vector<shared_ptr<Curve3D>> curves;
    curves.reserve(count); // ����������� ����� ��� count ���������

    for (int i = 0; i < count; ++i) { // ���������� ���������
        curves.push_back(createRandomCurve());
    }

    return curves;
}

vector<shared_ptr<Circle>> extractCircles(const vector<shared_ptr<Curve3D>>& curves) {
    vector<shared_ptr<Circle>> circles;

    for (const auto& curve : curves) {
        if (auto circle = dynamic_pointer_cast<Circle>(curve)) { // ����� ������ �� ���� ������
            circles.push_back(circle);
        }
    }

    return circles;
}

double sumRadii(const vector<shared_ptr<Circle>>& circles) {
    double sum = 0.0;
    for (const auto& circle : circles) {
        sum += circle->getRadius();
    }
    return sum;
}

// Parallel version using OpenMP
double sumRadiiParallel(const vector<shared_ptr<Circle>>& circles) {
    double sum = 0.0;
#pragma omp parallel for reduction(+:sum)
    for (size_t i = 0; i < circles.size(); ++i) {
        sum += circles[i]->getRadius();
    }
    return sum;
}
