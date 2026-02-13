# Taller 1 - Procesamiento de Imágenes / Computación Visual

Este repositorio contiene la implementación del Taller 1 de Procesamiento de Imágenes y Computación Visual.

## Autores
*   **Miguel Flechas**
*   **Andres Castro**
*   **Juan Hurtado**

## Descripción del Proyecto

El proyecto implementa varios algoritmos fundamentales de procesamiento de imágenes utilizando C++ y OpenCV. Las funcionalidades principales incluyen:

1.  **Conversión de Espacios de Color**:
    *   Implementación manual de la conversión RGB a HSV.
    *   Implementación manual de la conversión HSV a RGB.
    *   Modificación de la saturación en el espacio HSV.

2.  **Cuantización de Imágenes**:
    *   Implementación del algoritmo K-Means para reducir la cantidad de colores en una imagen.

3.  **Balance de Blancos**:
    *   Implementación del algoritmo Gray World para corrección de color.

4.  **Corrección Gamma**:
    *   Ajuste de la luminancia de la imagen mediante corrección Gamma con Look-Up Table (LUT).

5.  **Corrección de Viñeteo**:
    *   Algoritmo para corregir el oscurecimiento en las esquinas de la imagen (Vignette Correction).

## Estructura del Proyecto

*   `taller1.cpp`: Archivo fuente principal con la implementación de los ejercicios.
*   `TallerColor_Proyecto/`: Directorio con la configuración del proyecto CMake.
*   `Taller Color.pdf`: Documento con la descripción detallada del taller.

## Requisitos

*   C++ Compiler (GCC/Clang/MSVC)
*   OpenCV
*   CMake

## Cómo compilar y ejecutar

El proyecto utiliza CMake para la construcción. Asegúrate de tener OpenCV instalado y configurado.

```bash
mkdir build
cd build
cmake ..
make
./Taller1
```
