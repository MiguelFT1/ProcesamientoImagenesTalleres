#include "frame_diff.h"
#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv) {
  cv::VideoCapture cap(1);
  if (!cap.isOpened()) {
    std::cerr << "Error: No se pudo abrir la cámara." << std::endl;
    return -1;
  }

  FrameDifferencer differencer(0.05, 30);
  cv::Mat frame, mask, output;

  std::cout << "Presione 'q' o 'ESC' para salir." << std::endl;

  while (true) {
    cap >> frame;
    if (frame.empty())
      break;

    differencer.processFrame(frame, mask, output);

    cv::imshow("Original + Cajas Delimitadoras", output);
    cv::imshow("Mascara de Diferencia", mask);

    char key = (char)cv::waitKey(30);
    if (key == 'q' || key == 27)
      break;
  }

  cap.release();
  cv::destroyAllWindows();
  return 0;
}
