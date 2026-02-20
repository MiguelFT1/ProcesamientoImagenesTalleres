#include "gmm_segmenter.h"
#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv) {
  cv::VideoCapture cap(1);
  if (!cap.isOpened()) {
    std::cerr << "Error: No se pudo abrir la cámara." << std::endl;
    return -1;
  }

  GMMSegmenter segmenter;
  cv::Mat frame, mask, output;

  std::cout << "Presione 'q' o 'ESC' para salir." << std::endl;

  while (true) {
    cap >> frame;
    if (frame.empty())
      break;

    segmenter.processFrame(frame, mask, output);

    cv::imshow("Original + Cajas Delimitadoras", output);
    cv::imshow("Mascara GMM", mask);

    cv::Mat bgImage = segmenter.getBackgroundImage();
    if (!bgImage.empty()) {
      cv::imshow("Fondo Reconstruido", bgImage);
    }

    char key = (char)cv::waitKey(30);
    if (key == 'q' || key == 27)
      break;
  }

  cap.release();
  cv::destroyAllWindows();
  return 0;
}
