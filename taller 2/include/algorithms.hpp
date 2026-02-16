#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include <cmath>
#include <opencv2/opencv.hpp>

// Conversion BGR a escala de grises
void manualRGB2Gray(const cv::Mat &src, cv::Mat &dst) {
  if (src.empty())
    return;
  if (dst.empty() || dst.rows != src.rows || dst.cols != src.cols ||
      dst.type() != CV_8UC1)
    dst = cv::Mat(src.rows, src.cols, CV_8UC1);

  for (int i = 0; i < src.rows; ++i) {
    const cv::Vec3b *pSrc = src.ptr<cv::Vec3b>(i);
    uchar *pDst = dst.ptr<uchar>(i);
    for (int j = 0; j < src.cols; ++j) {
      pDst[j] = (uchar)(0.299f * pSrc[j][2] + 0.587f * pSrc[j][1] +
                        0.114f * pSrc[j][0]);
    }
  }
}

// Conversion escala de grises a BGR
void manualGray2BGR(const cv::Mat &src, cv::Mat &dst) {
  if (src.empty())
    return;
  if (dst.empty() || dst.rows != src.rows || dst.cols != src.cols ||
      dst.type() != CV_8UC3)
    dst = cv::Mat(src.rows, src.cols, CV_8UC3);

  for (int i = 0; i < src.rows; ++i) {
    const uchar *pSrc = src.ptr<uchar>(i);
    cv::Vec3b *pDst = dst.ptr<cv::Vec3b>(i);
    for (int j = 0; j < src.cols; ++j) {
      pDst[j] = cv::Vec3b(pSrc[j], pSrc[j], pSrc[j]);
    }
  }
}

// Redimensionamiento por vecino mas cercano
void manualResize(const cv::Mat &src, cv::Mat &dst, int newWidth,
                  int newHeight) {
  if (src.empty())
    return;
  if (dst.empty() || dst.rows != newHeight || dst.cols != newWidth ||
      dst.type() != src.type())
    dst = cv::Mat(newHeight, newWidth, src.type());

  float xr = (float)src.cols / newWidth;
  float yr = (float)src.rows / newHeight;
  bool color = (src.channels() == 3);

  for (int i = 0; i < newHeight; ++i) {
    int sy = std::min((int)(i * yr), src.rows - 1);
    if (color) {
      const cv::Vec3b *pS = src.ptr<cv::Vec3b>(sy);
      cv::Vec3b *pD = dst.ptr<cv::Vec3b>(i);
      for (int j = 0; j < newWidth; ++j)
        pD[j] = pS[std::min((int)(j * xr), src.cols - 1)];
    } else {
      const uchar *pS = src.ptr<uchar>(sy);
      uchar *pD = dst.ptr<uchar>(i);
      for (int j = 0; j < newWidth; ++j)
        pD[j] = pS[std::min((int)(j * xr), src.cols - 1)];
    }
  }
}

// Diferencia de frames
void manualFrameDifference(const cv::Mat &bg, const cv::Mat &curr,
                           cv::Mat &diff, int threshold) {
  if (bg.empty() || curr.empty())
    return;
  if (diff.empty() || diff.size() != curr.size() || diff.type() != CV_8UC1)
    diff = cv::Mat::zeros(curr.size(), CV_8UC1);

  for (int i = 0; i < curr.rows; ++i) {
    const uchar *pBg = bg.ptr<uchar>(i);
    const uchar *pCurr = curr.ptr<uchar>(i);
    uchar *pDiff = diff.ptr<uchar>(i);
    for (int j = 0; j < curr.cols; ++j) {
      int val = std::abs((int)pBg[j] - (int)pCurr[j]);
      pDiff[j] = (val > threshold) ? 255 : 0;
    }
  }
}

// Running Average
void runningAverage(const cv::Mat &curr, cv::Mat &avg, float alpha) {
  if (curr.empty())
    return;
  if (avg.empty()) {
    avg = cv::Mat(curr.size(), CV_32FC1);
    for (int i = 0; i < curr.rows; ++i) {
      const uchar *pC = curr.ptr<uchar>(i);
      float *pA = avg.ptr<float>(i);
      for (int j = 0; j < curr.cols; ++j)
        pA[j] = (float)pC[j];
    }
    return;
  }
  for (int i = 0; i < curr.rows; ++i) {
    const uchar *pC = curr.ptr<uchar>(i);
    float *pA = avg.ptr<float>(i);
    for (int j = 0; j < curr.cols; ++j)
      pA[j] = alpha * (float)pC[j] + (1.0f - alpha) * pA[j];
  }
}

// Conversion float a uchar
void manualFloat2Uchar(const cv::Mat &src, cv::Mat &dst) {
  if (src.empty())
    return;
  if (dst.empty() || dst.size() != src.size() || dst.type() != CV_8UC1)
    dst = cv::Mat(src.size(), CV_8UC1);
  for (int i = 0; i < src.rows; ++i) {
    const float *pS = src.ptr<float>(i);
    uchar *pD = dst.ptr<uchar>(i);
    for (int j = 0; j < src.cols; ++j)
      pD[j] = (uchar)std::min(std::max(pS[j], 0.0f), 255.0f);
  }
}

// Concatenacion horizontal de 3 imagenes BGR
void manualHConcat(const cv::Mat &a, const cv::Mat &b, const cv::Mat &c,
                   cv::Mat &dst) {
  if (a.empty() || b.empty() || c.empty())
    return;
  int w = a.cols + b.cols + c.cols;
  int h = a.rows;
  if (dst.empty() || dst.cols != w || dst.rows != h || dst.type() != a.type())
    dst = cv::Mat(h, w, a.type());

  for (int i = 0; i < h; ++i) {
    const cv::Vec3b *pA = a.ptr<cv::Vec3b>(i);
    const cv::Vec3b *pB = b.ptr<cv::Vec3b>(i);
    const cv::Vec3b *pC = c.ptr<cv::Vec3b>(i);
    cv::Vec3b *pD = dst.ptr<cv::Vec3b>(i);
    for (int j = 0; j < a.cols; ++j)
      pD[j] = pA[j];
    for (int j = 0; j < b.cols; ++j)
      pD[j + a.cols] = pB[j];
    for (int j = 0; j < c.cols; ++j)
      pD[j + a.cols + b.cols] = pC[j];
  }
}

// Normalizacion de brillo (estiramiento de histograma)
void manualNormalize(const cv::Mat &src, cv::Mat &dst) {
  if (src.empty())
    return;
  if (dst.empty() || dst.size() != src.size() || dst.type() != src.type())
    dst = cv::Mat(src.size(), src.type());

  uchar minVal = 255, maxVal = 0;
  for (int i = 0; i < src.rows; ++i) {
    const uchar *p = src.ptr<uchar>(i);
    for (int j = 0; j < src.cols; ++j) {
      if (p[j] < minVal)
        minVal = p[j];
      if (p[j] > maxVal)
        maxVal = p[j];
    }
  }

  float rango = (float)(maxVal - minVal);
  if (rango < 1.0f)
    rango = 1.0f;

  for (int i = 0; i < src.rows; ++i) {
    const uchar *pS = src.ptr<uchar>(i);
    uchar *pD = dst.ptr<uchar>(i);
    for (int j = 0; j < src.cols; ++j) {
      pD[j] = (uchar)(255.0f * (pS[j] - minVal) / rango);
    }
  }
}

#endif
