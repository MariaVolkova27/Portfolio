#include "curves.h"
#include <random>
#include <algorithm>
#include <stdexcept>
#include <iostream>

using namespace std;

shared_ptr<Curve3D> createRandomCurve() {
    static random_device rd; // создание источника истинной случайности
    static mt19937 gen(rd()); // создаем основной генератор - 32-разрядный беззнаковый генератор вихря Мерсенна (Mersenne Twister)
    static uniform_int_distribution<> typeDist(0, 2); // генератор целых чисел от 0 до 2 включительно
    static uniform_real_distribution<> paramDist(1.0, 10.0); // генератор вещественных чисел от 1 до 10 включительно

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
    curves.reserve(count); // резервируем место под count элементов

    for (int i = 0; i < count; ++i) { // префиксный инкремент
        curves.push_back(createRandomCurve());
    }

    return curves;
}

vector<shared_ptr<Circle>> extractCircles(const vector<shared_ptr<Curve3D>>& curves) {
    vector<shared_ptr<Circle>> circles;

    for (const auto& curve : curves) {
        if (auto circle = dynamic_pointer_cast<Circle>(curve)) { // отбор кругов из всех кривых
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
