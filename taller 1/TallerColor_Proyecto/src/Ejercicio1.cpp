#include "ColorConversions.hpp"
#include <vector>

using namespace cv;
using namespace std;

void ejercicio1_rgb_a_hsv(const Mat &src, Mat &dst) {
  if (src.empty())
    return;
  dst = Mat::zeros(src.size(), CV_8UC3);

  for (int i = 0; i < src.rows; ++i) {
    for (int j = 0; j < src.cols; ++j) {
      Vec3b pixel = src.at<Vec3b>(i, j);
      double b = pixel[0] / 255.0;
      double g = pixel[1] / 255.0;
      double r = pixel[2] / 255.0;

      double cmax = max({r, g, b});
      double cmin = min({r, g, b});
      double delta = cmax - cmin;

      double h = 0.0;
      if (delta == 0) {
        h = 0;
      } else if (cmax == r) {
        h = 60 * fmod(((g - b) / delta), 6);
      } else if (cmax == g) {
        h = 60 * (((b - r) / delta) + 2);
      } else if (cmax == b) {
        h = 60 * (((r - g) / delta) + 4);
      }

      if (h < 0)
        h += 360;

      double s = (cmax == 0) ? 0 : (delta / cmax);
      double v = cmax;

      // Escalamiento para OpenCV: H/2 [0-180], S*255 [0-255], V*255 [0-255]
      dst.at<Vec3b>(i, j)[0] = static_cast<uchar>(h / 2);
      dst.at<Vec3b>(i, j)[1] = static_cast<uchar>(s * 255);
      dst.at<Vec3b>(i, j)[2] = static_cast<uchar>(v * 255);
    }
  }
}
