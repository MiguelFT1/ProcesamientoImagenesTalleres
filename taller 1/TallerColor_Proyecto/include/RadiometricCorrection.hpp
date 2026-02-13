#ifndef RADIOMETRIC_CORRECTION_HPP
#define RADIOMETRIC_CORRECTION_HPP

#include <cmath>
#include <opencv2/opencv.hpp>

// Ejercicio 6: Corrección Gamma con LUT
void ejercicio6_gamma(const cv::Mat &src, cv::Mat &dst, float gamma);

// Ejercicio 7: Vignette Correction
void ejercicio7_vignette(const cv::Mat &src, cv::Mat &dst, float k_factor);

#endif // RADIOMETRIC_CORRECTION_HPP
