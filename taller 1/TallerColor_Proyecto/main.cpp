#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

#include "ColorConstancy.hpp"
#include "ColorConversions.hpp"
#include "RadiometricCorrection.hpp"
#include "Segmentation.hpp"

using namespace std;
using namespace cv;

void show_menu() {
  cout << "\n--- Taller de Procesamiento de Imagenes - Color ---" << endl;
  cout << "1. Convertir RGB -> HSV (y mostrar conversion)" << endl;
  cout << "2. Modificar Saturacion (x1.5)" << endl;
  cout << "3. Segmentacion K-Means (Manual)" << endl;
  cout << "4. Constancia de Color (Gray World)" << endl;
  cout << "5. Correccion Gamma (Gamma=0.5)" << endl;
  cout << "6. Correccion Vignette (k=0.5)" << endl;
  cout << "0. Salir" << endl;
  cout << "Seleccione una opcion: ";
}

int main() {
  string image_path;
  cout << "Ingrese la ruta de la imagen (o presione Enter para usar "
          "'images/lena.jpg'): ";
  getline(cin, image_path);

  if (image_path.empty()) {
    image_path = "images/lena.jpg";
  }

  Mat img = imread(image_path);

  if (img.empty()) {
    cout << "Error: No se pudo cargar la imagen en '" << image_path
         << "'. Usando imagen sintetica." << endl;
    img = Mat(400, 600, CV_8UC3);
    // Generar un patron con colores
    for (int i = 0; i < img.rows; ++i) {
      for (int j = 0; j < img.cols; ++j) {
        img.at<Vec3b>(i, j)[0] = (uchar)(i * 255 / img.rows); // B
        img.at<Vec3b>(i, j)[1] = (uchar)(j * 255 / img.cols); // G
        img.at<Vec3b>(i, j)[2] = (uchar)((i + j) % 255);      // R
      }
    }
  }

  imshow("Imagen Original", img);

  int option = -1;
  while (option != 0) {
    show_menu();
    cin >> option;

    if (option == 0)
      break;

    Mat dst;
    switch (option) {
    case 1: {
      ejercicio1_rgb_a_hsv(img, dst);
      imshow("RGB -> HSV (Visualizacion)", dst);
      break;
    }
    case 2: {
      Mat hsv_temp;
      ejercicio1_rgb_a_hsv(img, hsv_temp);
      ejercicio2_modificar_saturacion(hsv_temp);
      ejercicio2_hsv_a_rgb(hsv_temp, dst);
      imshow("Saturacion Aumentada", dst);
      break;
    }
    case 3: {
      int k = 4;
      cout << "Ingrese K para K-Means (defecto 4): ";
      // simple check mainly to consume newline if user inputs
      if (cin.peek() == '\n')
        cin.ignore();
      // actually lets just use fixed or simple input
      string k_str;
      cin >> k_str;
      try {
        k = stoi(k_str);
      } catch (...) {
        k = 4;
      }

      cout << "Ejecutando K-Means (" << k << " clusters)..." << endl;
      ejercicio3_kmeans_manual(img, dst, k);
      imshow("K-Means Segmentation", dst);
      break;
    }
    case 4: {
      ejercicio4_gray_world(img, dst);
      imshow("Gray World", dst);
      break;
    }
    case 5: {
      float gamma = 0.5f;
      ejercicio6_gamma(img, dst, gamma);
      imshow("Gamma Correction (0.5)", dst);
      break;
    }
    case 6: {
      float k_val = 0.5f;
      ejercicio7_vignette(img, dst, k_val);
      imshow("Vignette Correction", dst);
      break;
    }
    default:
      cout << "Opcion no valida." << endl;
    }

    cout << "Presione una tecla en la ventana de imagen para continuar..."
         << endl;
    waitKey(0);
    destroyAllWindows();
    // Re-show original for context
    imshow("Imagen Original", img);
  }

  return 0;
}
