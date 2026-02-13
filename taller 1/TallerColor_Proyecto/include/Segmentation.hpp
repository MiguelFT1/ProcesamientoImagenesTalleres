#ifndef SEGMENTATION_HPP
#define SEGMENTATION_HPP

#include <cmath>
#include <opencv2/opencv.hpp>
#include <random>
#include <vector>

struct Pixel {
  double r, g, b;
};

// Distancia Euclidiana entre dos colores
double distancia_euclidiana(const Pixel &p1, const Pixel &p2);

// Ejercicio 3: K-Means Manual
void ejercicio3_kmeans_manual(const cv::Mat &src, cv::Mat &dst, int K);

#endif // SEGMENTATION_HPP
