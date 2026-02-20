# Taller 3: Procesamiento de Imágenes - Segmentación de Rostros y Movimiento

**Integrantes:**
- Miguel Flechas
- Andres Castro
- Juan Hurtado

## Descripción del Proyecto
Este proyecto implementa dos técnicas de segmentación de movimiento espacial en tiempo real utilizando C++ y OpenCV:
1. **Parte A: Diferenciación de Cuadros (Frame Differencing):** Implementa la sustracción del fondo actual con un fondo base actualizado de forma adaptativa.
2. **Parte B: Modelos de Mezcla Gaussiano (GMM):** Utiliza el algoritmo MOG2 (Mixture of Gaussians) para separar el fondo de objetos en movimiento, con eliminación adicional de sombras.

## Requisitos
- **C++17** o superior.
- **OpenCV 4.x**.
- **CMake 3.10** o superior.

## Compilación y Ejecución
1. Crear el directorio build y compilar:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```
2. Ejecutar Parte A (Frame Differencing):
   ```bash
   ./parte_a
   ```
3. Ejecutar Parte B (GMM - MOG2):
   ```bash
   ./parte_b
   ```
