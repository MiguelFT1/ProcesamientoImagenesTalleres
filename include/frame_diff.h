#ifndef FRAME_DIFF_H
#define FRAME_DIFF_H

#include <opencv2/opencv.hpp>

class FrameDifferencer {
public:
    FrameDifferencer(double alpha = 0.05, int thresholdValue = 25);
    void processFrame(const cv::Mat& frame, cv::Mat& foregroundMask, cv::Mat& outputFrame);

private:
    cv::Mat background;
    double alpha;
    int thresholdValue;
    bool isFirstFrame;
};

#endif // FRAME_DIFF_H
