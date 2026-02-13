#include "Segmentation.hpp"

using namespace cv;
using namespace std;

double distancia_euclidiana(const Pixel &p1, const Pixel &p2) {
  return sqrt(pow(p1.r - p2.r, 2) + pow(p1.g - p2.g, 2) + pow(p1.b - p2.b, 2));
}

void ejercicio3_kmeans_manual(const Mat &src, Mat &dst, int K) {
  if (src.empty())
    return;
  int max_iters = 20;
  int rows = src.rows;
  int cols = src.cols;
  dst = src.clone();

  // 1. Inicialización aleatoria
  vector<Pixel> centroids(K);
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis_row(0, rows - 1);
  uniform_int_distribution<> dis_col(0, cols - 1);

  for (int k = 0; k < K; ++k) {
    Vec3b p = src.at<Vec3b>(dis_row(gen), dis_col(gen));
    centroids[k] = {(double)p[2], (double)p[1], (double)p[0]}; // RGB
  }

  Mat labels(rows, cols, CV_32S);

  for (int iter = 0; iter < max_iters; ++iter) {
    // 2. Asignación
    bool changed = false;

    vector<Pixel> sum_centroids(K, {0, 0, 0});
    vector<int> count_centroids(K, 0);

    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        Vec3b p_val = src.at<Vec3b>(i, j);
        Pixel p = {(double)p_val[2], (double)p_val[1], (double)p_val[0]};

        double min_dist = 1e9;
        int best_k = -1;

        for (int k = 0; k < K; ++k) {
          double d = distancia_euclidiana(p, centroids[k]);
          if (d < min_dist) {
            min_dist = d;
            best_k = k;
          }
        }

        if (labels.at<int>(i, j) != best_k) {
          labels.at<int>(i, j) = best_k;
          changed = true;
        }

        sum_centroids[best_k].r += p.r;
        sum_centroids[best_k].g += p.g;
        sum_centroids[best_k].b += p.b;
        count_centroids[best_k]++;
      }
    }

    if (!changed)
      break;

    // 3. Actualización
    for (int k = 0; k < K; ++k) {
      if (count_centroids[k] > 0) {
        centroids[k].r = sum_centroids[k].r / count_centroids[k];
        centroids[k].g = sum_centroids[k].g / count_centroids[k];
        centroids[k].b = sum_centroids[k].b / count_centroids[k];
      } else {
        Vec3b p = src.at<Vec3b>(dis_row(gen), dis_col(gen));
        centroids[k] = {(double)p[2], (double)p[1], (double)p[0]};
      }
    }
  }

  // Generar imagen cuantizada
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      int k = labels.at<int>(i, j);
      dst.at<Vec3b>(i, j)[0] = static_cast<uchar>(centroids[k].b);
      dst.at<Vec3b>(i, j)[1] = static_cast<uchar>(centroids[k].g);
      dst.at<Vec3b>(i, j)[2] = static_cast<uchar>(centroids[k].r);
    }
  }
}
