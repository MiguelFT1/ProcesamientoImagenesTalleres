#include "convex_hull.hpp"
#include <algorithm>

// Función auxiliar para determinar la orientación geométrica de 3 puntos.
// Retorna un valor positivo si O, A, B forman un giro a la izquierda (anti-horario),
// negativo si es un giro a la derecha (horario), y cero si son colineales.
double crossProduct(const cv::Point& O, const cv::Point& A, const cv::Point& B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

std::vector<cv::Point> computeConvexHullManual(const std::vector<cv::Point>& contourPoints) {
    int n = contourPoints.size(), k = 0;
    if (n <= 3) return contourPoints;

    std::vector<cv::Point> hull(2 * n);
    std::vector<cv::Point> points = contourPoints;

    // 1. Ordenar los puntos lexicográficamente (por X, luego por Y)
    std::sort(points.begin(), points.end(), [](const cv::Point& a, const cv::Point& b) {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });

    // 2. Construir la parte inferior del Convex Hull (Lower Hull)
    for (int i = 0; i < n; ++i) {
        while (k >= 2 && crossProduct(hull[k - 2], hull[k - 1], points[i]) <= 0) {
            k--;
        }
        hull[k++] = points[i];
    }

    // 3. Construir la parte superior del Convex Hull (Upper Hull)
    for (int i = n - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && crossProduct(hull[k - 2], hull[k - 1], points[i]) <= 0) {
            k--;
        }
        hull[k++] = points[i];
    }

    // Quitar el último punto que es repetido (es el mismo que el inicio)
    hull.resize(k - 1);

    return hull;
}
