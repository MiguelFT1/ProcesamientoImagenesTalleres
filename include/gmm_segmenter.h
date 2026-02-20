#ifndef GMM_SEGMENTER_H
#define GMM_SEGMENTER_H

#include <opencv2/opencv.hpp>
#include <opencv2/video/background_segm.hpp>

class GMMSegmenter {
public:
  GMMSegmenter();
  void processFrame(const cv::Mat &frame, cv::Mat &foregroundMask,
                    cv::Mat &outputFrame);
  cv::Mat getBackgroundImage() const;

private:
  cv::Ptr<cv::BackgroundSubtractorMOG2> pMOG2;
};

#endif // GMM_SEGMENTER_H
