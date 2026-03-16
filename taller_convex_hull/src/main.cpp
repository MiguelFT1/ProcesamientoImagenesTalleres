#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "convex_hull.hpp"

int main(int argc, char** argv) {
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: No se pudo abrir la camara." << std::endl;
        return -1;
    }

    std::cout << "Convex Hull Manual - Deteccion de Movimiento (Frame Differencing)" << std::endl;
    std::cout << "Mueve las manos frente a la camara." << std::endl;
    std::cout << "Presiona 'q' o ESC para salir." << std::endl;

    cv::Mat frame, gray, prevGray, diff, mask;
    const int  AREA_MIN    = 2000;  // px² mínimos por contorno
    const int  THRESH_VAL  = 30;    // sensibilidad al movimiento (mayor = menos sensible)
    const bool SHOW_MASK   = true;  // mostrar ventana de máscara

    // Esperar a que la cámara esté lista (puede tardar algunos frames en arrancar)
    int warmup = 0;
    while (frame.empty() && warmup < 60) {
        cap >> frame;
        warmup++;
        cv::waitKey(30);
    }
    if (frame.empty()) {
        std::cerr << "Error: no se pudo obtener imagen de la camara." << std::endl;
        return -1;
    }
    cv::cvtColor(frame, prevGray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(prevGray, prevGray, cv::Size(11, 11), 0);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7));

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // 1. Convertir a escala de grises y suavizar
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(gray, gray, cv::Size(11, 11), 0);

        // 2. Frame Differencing: diferencia absoluta entre frame actual y anterior
        cv::absdiff(gray, prevGray, diff);

        // 3. Umbralizar para obtener máscara binaria
        cv::threshold(diff, mask, THRESH_VAL, 255, cv::THRESH_BINARY);

        // 4. Morfología: eliminar ruido pequeño y rellenar huecos
        cv::morphologyEx(mask, mask, cv::MORPH_OPEN,  kernel);
        cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);
        cv::dilate(mask, mask, kernel, cv::Point(-1,-1), 1);

        // 5. Encontrar contornos
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // 6. Reunir todos los puntos de contornos grandes en un solo vector
        std::vector<cv::Point> allPoints;
        for (const auto& cnt : contours) {
            if (cv::contourArea(cnt) < AREA_MIN) continue;
            allPoints.insert(allPoints.end(), cnt.begin(), cnt.end());
        }

        // 7. Dibujar resultado
        cv::Mat display = frame.clone();

        if (allPoints.size() >= 3) {
            // --- CONVEX HULL MANUAL (Monotone Chain, sin OpenCV hull) ---
            std::vector<cv::Point> hull = computeConvexHullManual(allPoints);

            // Relleno translúcido
            cv::Mat overlay = frame.clone();
            std::vector<std::vector<cv::Point>> hullPoly = {hull};
            cv::fillPoly(overlay, hullPoly, cv::Scalar(30, 160, 255));
            cv::addWeighted(overlay, 0.20, display, 0.80, 0, display);

            // Contorno del hull (rojo)
            for (size_t j = 0; j < hull.size(); j++) {
                cv::line(display, hull[j], hull[(j + 1) % hull.size()],
                         cv::Scalar(0, 50, 255), 3, cv::LINE_AA);
            }
            // Vértices (amarillo)
            for (const auto& pt : hull) {
                cv::circle(display, pt, 7, cv::Scalar(0, 230, 230), -1, cv::LINE_AA);
            }

            // Info en pantalla
            std::string info = "Hull: " + std::to_string(hull.size()) + " vertices";
            cv::putText(display, info, cv::Point(10, 35),
                        cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(255,255,255), 2);
        } else {
            cv::putText(display, "Mueve las manos...", cv::Point(10, 35),
                        cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(180,180,180), 2);
        }

        cv::imshow("Convex Hull Manual | Movimiento de Manos", display);
        if (SHOW_MASK) cv::imshow("Mascara de Movimiento", mask);

        // 8. Actualizar frame anterior (media ponderada = suavizado temporal)
        cv::addWeighted(gray, 0.4, prevGray, 0.6, 0, prevGray);

        char c = (char)cv::waitKey(30);
        if (c == 27 || c == 'q' || c == 'Q') break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
