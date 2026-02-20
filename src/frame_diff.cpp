#include "frame_diff.h"
#include <vector>

FrameDifferencer::FrameDifferencer(double alpha, int thresholdValue)
    : alpha(alpha), thresholdValue(thresholdValue), isFirstFrame(true) {}

void FrameDifferencer::processFrame(const cv::Mat &frame,
                                    cv::Mat &foregroundMask,
                                    cv::Mat &outputFrame) {
  cv::Mat grayFrame;
  cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
  cv::GaussianBlur(grayFrame, grayFrame, cv::Size(5, 5), 0);

  if (isFirstFrame) {
    grayFrame.convertTo(background, CV_32F);
    isFirstFrame = false;
    foregroundMask = cv::Mat::zeros(frame.size(), CV_8UC1);
    outputFrame = frame.clone();
    return;
  }

  cv::Mat currentGray32F;
  grayFrame.convertTo(currentGray32F, CV_32F);

  cv::Mat diff32F, diff8U;
  cv::absdiff(background, currentGray32F, diff32F);
  diff32F.convertTo(diff8U, CV_8U);

  cv::threshold(diff8U, foregroundMask, thresholdValue, 255, cv::THRESH_BINARY);

  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
  cv::morphologyEx(foregroundMask, foregroundMask, cv::MORPH_OPEN, kernel);
  cv::morphologyEx(foregroundMask, foregroundMask, cv::MORPH_CLOSE, kernel);

  outputFrame = frame.clone();

  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(foregroundMask, contours, cv::RETR_EXTERNAL,
                   cv::CHAIN_APPROX_SIMPLE);

  for (const auto &contour : contours) {
    if (cv::contourArea(contour) > 500) { // Filter out small contours
      cv::Rect boundingBox = cv::boundingRect(contour);
      cv::rectangle(outputFrame, boundingBox, cv::Scalar(0, 255, 0), 2);
    }
  }

  // Update background adaptively: B = alpha * I + (1 - alpha) * B
  // We pass currentGray32F and background in correct argument order for
  // addWeighted
  cv::addWeighted(currentGray32F, alpha, background, 1.0 - alpha, 0.0,
                  background);
}
