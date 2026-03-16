#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

/**
 * @brief Calcula el convex hull de un conjunto de puntos 2D (por ejemplo un contorno).
 *        No depende de funciones embebidas de OpenCV para el cálculo del convex hull.
 *        Utiliza el algoritmo de Monotone Chain (Andrew's algorithm).
 *
 * @param points Vector de puntos del contorno.
 * @return std::vector<cv::Point> Puntos que conforman el envolvente convexo calculado a mano.
 */
std::vector<cv::Point> computeConvexHullManual(const std::vector<cv::Point>& points);
