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

  cv::Mat frame, gray, grayNorm, background, diff;
  int threshold = 30;

  while (true) {
    cap >> frame;
    if (frame.empty())
      continue;

    manualRGB2Gray(frame, gray);
    manualNormalize(gray, grayNorm);

    if (!background.empty()) {
      manualFrameDifference(background, grayNorm, diff, threshold);
      cv::imshow("Diferencia", diff);
    }

    cv::imshow("Original", frame);

    char key = (char)cv::waitKey(30);
    if (key == 'q')
      break;
    if (key == 'b') {
      grayNorm.copyTo(background);
    }
  }
  return 0;
}
