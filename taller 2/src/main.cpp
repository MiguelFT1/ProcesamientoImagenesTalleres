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

  cv::Mat frame, gray, grayNorm, background, diff, avg, avgDisplay;
  int threshold = 30;
  float alpha = 0.05f;
  bool bgCaptured = false;

  while (true) {
    cap >> frame;
    if (frame.empty())
      continue;

    manualRGB2Gray(frame, gray);
    manualNormalize(gray, grayNorm);

    if (!bgCaptured) {
      grayNorm.copyTo(background);
      bgCaptured = true;
    }

    manualFrameDifference(background, grayNorm, diff, threshold);
    runningAverage(grayNorm, avg, alpha);
    manualFloat2Uchar(avg, avgDisplay);

    // Dashboard
    cv::Mat resOrig, resDiffGray, resAvgGray;
    cv::Mat resDiffBGR, resAvgBGR;

    manualResize(frame, resOrig, 400, 300);
    manualResize(diff, resDiffGray, 400, 300);
    manualResize(avgDisplay, resAvgGray, 400, 300);

    manualGray2BGR(resDiffGray, resDiffBGR);
    manualGray2BGR(resAvgGray, resAvgBGR);

    cv::Mat dashboard;
    manualHConcat(resOrig, resDiffBGR, resAvgBGR, dashboard);

    cv::putText(dashboard, "Original", cv::Point(10, 25),
                cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
    cv::putText(dashboard, "Diferencia", cv::Point(410, 25),
                cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
    cv::putText(dashboard, "Running Avg", cv::Point(810, 25),
                cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);

    cv::imshow("Taller 2", dashboard);

    char key = (char)cv::waitKey(30);
    if (key == 'q')
      break;
    if (key == 'b') {
      grayNorm.copyTo(background);
    }
  }
  return 0;
}
