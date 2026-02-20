#include "gmm_segmenter.h"
#include <vector>

GMMSegmenter::GMMSegmenter() { pMOG2 = cv::createBackgroundSubtractorMOG2(); }

void GMMSegmenter::processFrame(const cv::Mat &frame, cv::Mat &foregroundMask,
                                cv::Mat &outputFrame) {
  pMOG2->apply(frame, foregroundMask);

  // Apply additional threshold to remove shadows (which have value 127 in MOG2)
  cv::threshold(foregroundMask, foregroundMask, 200, 255, cv::THRESH_BINARY);

  outputFrame = frame.clone();

  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(foregroundMask, contours, cv::RETR_EXTERNAL,
                   cv::CHAIN_APPROX_SIMPLE);

  for (const auto &contour : contours) {
    if (cv::contourArea(contour) > 500) { // Filter out small contours
      cv::Rect boundingBox = cv::boundingRect(contour);
      cv::rectangle(outputFrame, boundingBox, cv::Scalar(0, 0, 255), 2);
    }
  }
}

cv::Mat GMMSegmenter::getBackgroundImage() const {
  cv::Mat bgImage;
  pMOG2->getBackgroundImage(bgImage);
  return bgImage;
}
