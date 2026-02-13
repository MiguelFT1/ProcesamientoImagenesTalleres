#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

using namespace cv;
using namespace std;

// Estructura para K-Means
struct Pixel {
    double r, g, b;
};

// Distancia Euclidiana entre dos colores
double distancia_euclidiana(const Pixel& p1, const Pixel& p2) {
    return sqrt(pow(p1.r - p2.r, 2) + pow(p1.g - p2.g, 2) + pow(p1.b - p2.b, 2));
}

// Ejercicio 1: Convertir RGB a HSV manualmente
// H: [0, 180], S: [0, 255], V: [0, 255]
void ejercicio1_rgb_a_hsv(const Mat& src, Mat& dst) {
    dst = Mat::zeros(src.size(), CV_8UC3);

    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            Vec3b pixel = src.at<Vec3b>(i, j);
            double b = pixel[0] / 255.0;
            double g = pixel[1] / 255.0;
            double r = pixel[2] / 255.0;

            double cmax = max({r, g, b});
            double cmin = min({r, g, b});
            double delta = cmax - cmin;

            double h = 0.0;
            if (delta == 0) {
                h = 0;
            } else if (cmax == r) {
                h = 60 * fmod(((g - b) / delta), 6);
            } else if (cmax == g) {
                h = 60 * (((b - r) / delta) + 2);
            } else if (cmax == b) {
                h = 60 * (((r - g) / delta) + 4);
            }

            if (h < 0) h += 360;

            double s = (cmax == 0) ? 0 : (delta / cmax);
            double v = cmax;

            // Escalamiento para OpenCV: H/2 [0-180], S*255 [0-255], V*255 [0-255]
            dst.at<Vec3b>(i, j)[0] = static_cast<uchar>(h / 2);
            dst.at<Vec3b>(i, j)[1] = static_cast<uchar>(s * 255);
            dst.at<Vec3b>(i, j)[2] = static_cast<uchar>(v * 255);
        }
    }
}

// Ejercicio 2a: Convertir HSV a RGB manualmente
void ejercicio2_hsv_a_rgb(const Mat& src, Mat& dst) {
    dst = Mat::zeros(src.size(), CV_8UC3);

    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            Vec3b pixel = src.at<Vec3b>(i, j);
            double h = pixel[0] * 2.0; // [0, 360]
            double s = pixel[1] / 255.0; // [0, 1]
            double v = pixel[2] / 255.0; // [0, 1]

            double c = v * s;
            double x = c * (1 - abs(fmod(h / 60.0, 2) - 1));
            double m = v - c;

            double r_prime = 0, g_prime = 0, b_prime = 0;

            if (0 <= h && h < 60) {
                r_prime = c; g_prime = x; b_prime = 0;
            } else if (60 <= h && h < 120) {
                r_prime = x; g_prime = c; b_prime = 0;
            } else if (120 <= h && h < 180) {
                r_prime = 0; g_prime = c; b_prime = x;
            } else if (180 <= h && h < 240) {
                r_prime = 0; g_prime = x; b_prime = c;
            } else if (240 <= h && h < 300) {
                r_prime = x; g_prime = 0; b_prime = c;
            } else if (300 <= h && h < 360) {
                r_prime = c; g_prime = 0; b_prime = x;
            }

            dst.at<Vec3b>(i, j)[2] = static_cast<uchar>((r_prime + m) * 255);
            dst.at<Vec3b>(i, j)[1] = static_cast<uchar>((g_prime + m) * 255);
            dst.at<Vec3b>(i, j)[0] = static_cast<uchar>((b_prime + m) * 255);
        }
    }
}

// Ejercicio 2b: Modificar saturación (x1.5)
void ejercicio2_modificar_saturacion(Mat& img) {
    // Asume entrada HSV.
    // Si la entrada es RGB, se deberia convertir a HSV primero, modificar S, y volver a RGB.
    // Pero la instruccion dice "En ejercicio2_modificar_saturacion(), aumenta la saturación S..."
    // Asumiremos que trabajamos sobre una imagen HSV para simplicidad o convertiremos internamente.
    // Dado que el flujo logico suele ser RGB -> HSV -> Modificar -> RGB:
    // Voy a asumir que 'img' TIENE que estar en HSV para que esto tenga sentido directo,
    // o voy a hacer la conversión completa aquí para que sea una función standalone segura.
    // El enunciado dice: "Implementa la lógica inversa para el ejercicio2... Modificación de saturación..."
    // Hare la funcion que *toma* una imagen HSV y la modifica in-place, ya que es lo más eficiente modularmente.
    
    for (int i = 0; i < img.rows; ++i) {
        for (int j = 0; j < img.cols; ++j) {
            // Canal 1 es Saturación en HSV OpenCV
            int s = img.at<Vec3b>(i, j)[1];
            s = static_cast<int>(s * 1.5);
            if (s > 255) s = 255; // Clamp
            img.at<Vec3b>(i, j)[1] = static_cast<uchar>(s);
        }
    }
}

// Ejercicio 3: K-Means Manual
void ejercicio3_kmeans_manual(const Mat& src, Mat& dst, int K) {
    int max_iters = 20;
    int rows = src.rows;
    int cols = src.cols;
    dst = src.clone();

    // 1. Inicialización aleatoria de centroides
    vector<Pixel> centroids(K);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis_row(0, rows - 1);
    uniform_int_distribution<> dis_col(0, cols - 1);

    for (int k = 0; k < K; ++k) {
        Vec3b p = src.at<Vec3b>(dis_row(gen), dis_col(gen));
        centroids[k] = { (double)p[2], (double)p[1], (double)p[0] }; // RGB
    }

    // Etiquetas para cada pixel
    Mat labels(rows, cols, CV_32S);

    for (int iter = 0; iter < max_iters; ++iter) {
        // 2. Asignación
        bool changed = false;
        
        // Acumuladores para recalcular centroides
        vector<Pixel> sum_centroids(K, {0, 0, 0});
        vector<int> count_centroids(K, 0);

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                Vec3b p_val = src.at<Vec3b>(i, j);
                Pixel p = { (double)p_val[2], (double)p_val[1], (double)p_val[0] };
                
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
                    // Aunque no usamos 'changed' para parar temprano explicitamente segun instrucciones ("hasta convergencia... o max 20"), es bueno saber.
                    // Strictamente el loop va hasta convergencia O max_iters.
                    labels.at<int>(i, j) = best_k;
                    changed = true;
                }

                sum_centroids[best_k].r += p.r;
                sum_centroids[best_k].g += p.g;
                sum_centroids[best_k].b += p.b;
                count_centroids[best_k]++;
            }
        }

        if (!changed) break;

        // 3. Actualización
        for (int k = 0; k < K; ++k) {
            if (count_centroids[k] > 0) {
                centroids[k].r = sum_centroids[k].r / count_centroids[k];
                centroids[k].g = sum_centroids[k].g / count_centroids[k];
                centroids[k].b = sum_centroids[k].b / count_centroids[k];
            } else {
                 // Re-inicializar si un cluster queda vacio (opcional pero robusto)
                 Vec3b p = src.at<Vec3b>(dis_row(gen), dis_col(gen));
                 centroids[k] = { (double)p[2], (double)p[1], (double)p[0] };
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

// Ejercicio 4: Gray World
void ejercicio4_gray_world(const Mat& src, Mat& dst) {
    dst = Mat::zeros(src.size(), CV_8UC3);
    double sum_r = 0, sum_g = 0, sum_b = 0;
    int N = src.rows * src.cols;

    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            Vec3b p = src.at<Vec3b>(i, j);
            sum_b += p[0];
            sum_g += p[1];
            sum_r += p[2];
        }
    }

    double avg_r = sum_r / N;
    double avg_g = sum_g / N;
    double avg_b = sum_b / N;
    double gray_avg = (avg_r + avg_g + avg_b) / 3.0;

    double scale_r = gray_avg / avg_r;
    double scale_g = gray_avg / avg_g;
    double scale_b = gray_avg / avg_b;

    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            Vec3b p = src.at<Vec3b>(i, j);
            double new_b = p[0] * scale_b;
            double new_g = p[1] * scale_g;
            double new_r = p[2] * scale_r;

            dst.at<Vec3b>(i, j)[0] = saturate_cast<uchar>(new_b);
            dst.at<Vec3b>(i, j)[1] = saturate_cast<uchar>(new_g);
            dst.at<Vec3b>(i, j)[2] = saturate_cast<uchar>(new_r);
        }
    }
}

// Ejercicio 6: Corrección Gamma con LUT
void ejercicio6_gamma(const Mat& src, Mat& dst, float gamma) {
    dst = Mat::zeros(src.size(), CV_8UC3);
    
    // Crear LUT
    unsigned char lut[256];
    for (int i = 0; i < 256; ++i) {
        lut[i] = saturate_cast<uchar>(255.0 * pow(i / 255.0, gamma));
    }

    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            for (int c = 0; c < 3; ++c) {
                dst.at<Vec3b>(i, j)[c] = lut[src.at<Vec3b>(i, j)[c]];
            }
        }
    }
}

// Ejercicio 7: Vignette Correction
// f = 1 / (1 - k * d_norm^2)
void ejercicio7_vignette(const Mat& src, Mat& dst, float k_factor) {
    dst = Mat::zeros(src.size(), CV_8UC3);
    int cx = src.cols / 2;
    int cy = src.rows / 2;
    double max_dist = sqrt(cx * cx + cy * cy);

    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            double dist = sqrt(pow(j - cx, 2) + pow(i - cy, 2));
            double d_norm = dist / max_dist;
            
            // Formula: f = 1 / (1 - k * d^2)
            // Cuidado con division por cero si k=1 y d_norm=1 -> 1-1=0.
            // Asumiremos k < 1 para estabilidad, o clamp.
            double denominator = 1.0 - k_factor * pow(d_norm, 2);
            if (denominator < 1e-5) denominator = 1e-5; 

            double f = 1.0 / denominator;

            for (int c = 0; c < 3; ++c) {
                dst.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(src.at<Vec3b>(i, j)[c] * f);
            }
        }
    }
}

int main() {
    // 1. Obtener una imagen
    // Intentamos cargar "lena.jpg" o generamos un gradiente si no existe.
    Mat img = imread("lena.jpg");
    if (img.empty()) {
        cout << "No se encontro 'lena.jpg', generando imagen de prueba..." << endl;
        img = Mat(400, 600, CV_8UC3);
        // Generar un patron con colores
        for(int i=0; i<img.rows; ++i) {
            for(int j=0; j<img.cols; ++j) {
                img.at<Vec3b>(i,j)[0] = (uchar)(i * 255 / img.rows); // B
                img.at<Vec3b>(i,j)[1] = (uchar)(j * 255 / img.cols); // G
                img.at<Vec3b>(i,j)[2] = (uchar)((i+j) % 255);        // R
            }
        }
        // Añadir algo de "viñeteo artificial" para probar la corrección?
        // O simplemente usar la imagen tal cual.
    }
    
    imshow("Original", img);

    // --- Ejercicio 1 & 2: RGB <-> HSV ---
    Mat img_hsv, img_rgb_recovered, img_sat_boost;
    
    ejercicio1_rgb_a_hsv(img, img_hsv);
    imshow("HSV Manual", img_hsv); // Note: HSV displayed as RGB looks weird, expected.

    // Aumentar saturacion en HSV
    Mat img_hsv_sat = img_hsv.clone();
    ejercicio2_modificar_saturacion(img_hsv_sat);
    
    // Vuelta a RGB
    ejercicio2_hsv_a_rgb(img_hsv_sat, img_sat_boost);
    imshow("Saturacion Aumentada (HSV->RGB)", img_sat_boost);

    // Comprobacion vuelta normal
    ejercicio2_hsv_a_rgb(img_hsv, img_rgb_recovered);
    // imshow("Recuperada RGB (De HSV intacto)", img_rgb_recovered);

    // --- Ejercicio 3: K-Means ---
    Mat img_kmeans;
    cout << "Ejecutando K-Means (K=4)..." << endl;
    ejercicio3_kmeans_manual(img, img_kmeans, 4);
    imshow("K-Means (K=4)", img_kmeans);

    // --- Ejercicio 4: Gray World ---
    Mat img_gray_world;
    // Para probar esto bien, podriamos teñir la imagen original artificialmente primero
    Mat img_tinted = img.clone();
    // Teñir de azul
    for(int i=0; i<img_tinted.rows; ++i)
        for(int j=0; j<img_tinted.cols; ++j)
            img_tinted.at<Vec3b>(i,j)[0] = saturate_cast<uchar>(img_tinted.at<Vec3b>(i,j)[0] * 1.5);
    
    imshow("Teñida (Artificial)", img_tinted);
    ejercicio4_gray_world(img_tinted, img_gray_world);
    imshow("Gray World Corrected", img_gray_world);

    // --- Ejercicio 6: Gamma ---
    Mat img_gamma;
    float gamma = 2.2f; // Oscurecer medios tonos si > 1, pero inverso? Si gamma > 1 formula pow(x, gamma) oscurece.
    // La formula es 255 * (in/255)^gamma.
    // Gamma tipico de monitor es 2.2. Correccion gamma suele ser 1/2.2 ~ 0.45.
    // Probaremos con 0.5 para aclarar.
    ejercicio6_gamma(img, img_gamma, 0.5f);
    imshow("Gamma Corrected (0.5)", img_gamma);

    // --- Ejercicio 7: Vignette Correction ---
    // Simulamos una imagen con viñeteo para corregirla? 
    // O aplicamos la formula tal cual. La formula dada es corrección (f > 1 en esquinas).
    // Si aplicamos f > 1 a una imagen plana, las esquinas se veran blancas.
    // Vamos a aplicarlo con un k pequeño.
    Mat img_vignette;
    ejercicio7_vignette(img, img_vignette, 0.5f); 
    imshow("Vignette Correction applied", img_vignette);

    cout << "Proceso completado. Presione una tecla en las ventanas para salir." << endl;
    waitKey(0);
    return 0;
}
