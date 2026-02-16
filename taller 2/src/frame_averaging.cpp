#include "algorithms.hpp"
#include <opencv2/opencv.hpp>

int main() {
  cv::VideoCapture cap(0);
  if (!cap.isOpened())
    return -1;

  cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

  // Warmup
  cv::Mat tmp;
  for (int i = 0; i < 60; i++) {
    cap >> tmp;
    cv::waitKey(33);
  }

  cv::Mat frame, gray, grayNorm, avg, avgDisplay;
  float alpha = 0.05f;

  while (true) {
    cap >> frame;
    if (frame.empty())
      continue;

    manualRGB2Gray(frame, gray);
    manualNormalize(gray, grayNorm);
    runningAverage(grayNorm, avg, alpha);
    manualFloat2Uchar(avg, avgDisplay);

    cv::imshow("Running Average", avgDisplay);
    cv::imshow("Original", frame);

    if (cv::waitKey(30) == 'q')
      break;
  }
  return 0;
}
