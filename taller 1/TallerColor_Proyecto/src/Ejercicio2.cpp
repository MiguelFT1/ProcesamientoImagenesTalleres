#include "ColorConversions.hpp"
#include <vector>

using namespace cv;
using namespace std;

void ejercicio2_hsv_a_rgb(const Mat &src, Mat &dst) {
  if (src.empty())
    return;
  dst = Mat::zeros(src.size(), CV_8UC3);

  for (int i = 0; i < src.rows; ++i) {
    for (int j = 0; j < src.cols; ++j) {
      Vec3b pixel = src.at<Vec3b>(i, j);
      double h = pixel[0] * 2.0;   // [0, 360]
      double s = pixel[1] / 255.0; // [0, 1]
      double v = pixel[2] / 255.0; // [0, 1]

      double c = v * s;
      double x = c * (1 - abs(fmod(h / 60.0, 2) - 1));
      double m = v - c;

      double r_prime = 0, g_prime = 0, b_prime = 0;

      if (0 <= h && h < 60) {
        r_prime = c;
        g_prime = x;
        b_prime = 0;
      } else if (60 <= h && h < 120) {
        r_prime = x;
        g_prime = c;
        b_prime = 0;
      } else if (120 <= h && h < 180) {
        r_prime = 0;
        g_prime = c;
        b_prime = x;
      } else if (180 <= h && h < 240) {
        r_prime = 0;
        g_prime = x;
        b_prime = c;
      } else if (240 <= h && h < 300) {
        r_prime = x;
        g_prime = 0;
        b_prime = c;
      } else if (300 <= h && h < 360) {
        r_prime = c;
        g_prime = 0;
        b_prime = x;
      }

      dst.at<Vec3b>(i, j)[2] = static_cast<uchar>((r_prime + m) * 255);
      dst.at<Vec3b>(i, j)[1] = static_cast<uchar>((g_prime + m) * 255);
      dst.at<Vec3b>(i, j)[0] = static_cast<uchar>((b_prime + m) * 255);
    }
  }
}

void ejercicio2_modificar_saturacion(Mat &img) {
  if (img.empty())
    return;
  for (int i = 0; i < img.rows; ++i) {
    for (int j = 0; j < img.cols; ++j) {
      // Canal 1 es Saturación en HSV OpenCV
      int s = img.at<Vec3b>(i, j)[1];
      s = static_cast<int>(s * 1.5);
      if (s > 255)
        s = 255;
      img.at<Vec3b>(i, j)[1] = static_cast<uchar>(s);
    }
  }
}
