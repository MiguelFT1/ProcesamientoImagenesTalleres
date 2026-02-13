#include "RadiometricCorrection.hpp"

using namespace cv;
using namespace std;

void ejercicio6_gamma(const Mat &src, Mat &dst, float gamma) {
  if (src.empty())
    return;
  dst = Mat::zeros(src.size(), CV_8UC3);

  // Crear LUT
  unsigned char lut[256];
  for (int i = 0; i < 256; ++i) {
    lut[i] = saturate_cast<uchar>(255.0 * pow(i / 255.0, gamma));
  }

  for (int i = 0; i < src.rows; ++i) {
    for (int j = 0; j < src.cols; ++j) {
      for (int c = 0; c < 3; ++c) {
        dst.at<Vec3b>(i, j)[c] = lut[src.at<Vec3b>(i, j)[c]];
      }
    }
  }
}
