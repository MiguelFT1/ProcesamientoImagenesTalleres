#include "RadiometricCorrection.hpp"

using namespace cv;
using namespace std;

void ejercicio7_vignette(const Mat &src, Mat &dst, float k_factor) {
  if (src.empty())
    return;
  dst = Mat::zeros(src.size(), CV_8UC3);
  int cx = src.cols / 2;
  int cy = src.rows / 2;
  double max_dist = sqrt(cx * cx + cy * cy);

  for (int i = 0; i < src.rows; ++i) {
    for (int j = 0; j < src.cols; ++j) {
      double dist = sqrt(pow(j - cx, 2) + pow(i - cy, 2));
      double d_norm = dist / max_dist;

      double denominator = 1.0 - k_factor * pow(d_norm, 2);
      if (denominator < 1e-5)
        denominator = 1e-5;

      double f = 1.0 / denominator;

      for (int c = 0; c < 3; ++c) {
        dst.at<Vec3b>(i, j)[c] =
            saturate_cast<uchar>(src.at<Vec3b>(i, j)[c] * f);
      }
    }
  }
}
