#include "ColorConstancy.hpp"

using namespace cv;
using namespace std;

void ejercicio4_gray_world(const Mat &src, Mat &dst) {
  if (src.empty())
    return;
  dst = Mat::zeros(src.size(), CV_8UC3);
  double sum_r = 0, sum_g = 0, sum_b = 0;
  int N = src.rows * src.cols;

  for (int i = 0; i < src.rows; ++i) {
    for (int j = 0; j < src.cols; ++j) {
      Vec3b p = src.at<Vec3b>(i, j);
      sum_b += p[0];
      sum_g += p[1];
      sum_r += p[2];
    }
  }

  double avg_r = sum_r / N;
  double avg_g = sum_g / N;
  double avg_b = sum_b / N;
  double gray_avg = (avg_r + avg_g + avg_b) / 3.0;

  double scale_r = gray_avg / avg_r;
  double scale_g = gray_avg / avg_g;
  double scale_b = gray_avg / avg_b;

  for (int i = 0; i < src.rows; ++i) {
    for (int j = 0; j < src.cols; ++j) {
      Vec3b p = src.at<Vec3b>(i, j);
      double new_b = p[0] * scale_b;
      double new_g = p[1] * scale_g;
      double new_r = p[2] * scale_r;

      dst.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(new_b);
      dst.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(new_g);
      dst.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(new_r);
    }
  }
}
