// program on dealing with 3D curves

#include "curves.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

int main() {
    const int NUM_CURVES = 20;
    const double T = PI / 4.0;

    try {
        // заполняем контейнер случайными кривыми
        auto curves = populateCurves(NUM_CURVES); // «auto» - тип переменной определяется компилятором автоматически из конструктора 

        cout << "=== 3D Curves at t = PI/4 ===" << endl;
        cout << fixed << setprecision(3); // использовать фиксированную точку, 3 знака после запятой

        // распечатываем координаты точек и векторов-производных
        for (size_t i = 0; i < curves.size(); ++i) { // «size_t» - беззнаковый целый тип данных для размеров контейнеров
            const auto& curve = curves[i];
            Point3D point = curve->getPoint(T);
            Vector3D derivative = curve->getDerivative(T);

            cout << "Curve " << i + 1 << ": ";
            if (dynamic_cast<Circle*>(curve.get())) { // "dynamic_cast" определяет, можно ли привести объект к указанному типу
                cout << "Circle (r=" << curve->getRadius() << ")";
            }
            else if (dynamic_cast<Ellipse*>(curve.get())) {
                cout << "Ellipse";
            }
            else if (dynamic_cast<Helix*>(curve.get())) {
                cout << "Helix";
            }

            cout << "\n  Point: (" << point.x << ", " << point.y << ", " << point.z << ")";
            cout << "\n  Derivative: (" << derivative.x << ", " << derivative.y << ", " << derivative.z << ")\n";
        }

        // выбор кругов (умные разделенные указатели, не копии)
        auto circles = extractCircles(curves);

        cout << "\n=== Extracted Circles ===" << endl;
        cout << "Found " << circles.size() << " circles\n";

        // сортируем круги по возрастанию радиуса
        sort(circles.begin(), circles.end(),
            [](const shared_ptr<Circle>& a, const shared_ptr<Circle>& b) {
                return a->getRadius() < b->getRadius();
            });

        cout << "\nCircles sorted by radius:\n";
        for (size_t i = 0; i < circles.size(); ++i) {
            cout << "Circle " << i + 1 << ": radius = " << circles[i]->getRadius() << endl;
        }

        // подсчет суммы радиусов
        double totalSum = sumRadii(circles);
        cout << "\nTotal sum of radii: " << totalSum << endl;

        // опционально: параллельные вычисления
#ifdef _OPENMP
        double parallelSum = sumRadiiParallel(circles);
        cout << "Parallel sum of radii: " << parallelSum << endl;
#endif

    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
