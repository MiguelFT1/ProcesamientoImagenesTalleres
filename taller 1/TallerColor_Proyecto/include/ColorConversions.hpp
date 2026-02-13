#ifndef COLOR_CONVERSIONS_HPP
#define COLOR_CONVERSIONS_HPP

#include <algorithm>
#include <cmath>
#include <opencv2/opencv.hpp>

// Ejercicio 1: Convertir RGB a HSV manualmente
void ejercicio1_rgb_a_hsv(const cv::Mat &src, cv::Mat &dst);

// Ejercicio 2a: Convertir HSV a RGB manualmente
void ejercicio2_hsv_a_rgb(const cv::Mat &src, cv::Mat &dst);

// Ejercicio 2b: Modificar saturación (x1.5)
void ejercicio2_modificar_saturacion(cv::Mat &img);

#endif // COLOR_CONVERSIONS_HPP
