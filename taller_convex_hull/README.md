# Taller: Detección de Movimiento con Convex Hull Manual

**Universidad:** Procesamiento de Imágenes  
**Taller:** Convex Hull Manual + Detección de Movimiento con Cámara  
**Fecha:** Marzo 2026

## Autores

| Nombre | Rol |
|--------|-----|
| Miguel Flechas | Desarrollo e implementación |
| Juan Hurtado | Desarrollo e implementación |
| Andres Castro | Desarrollo e implementación |

---

## Descripción del Proyecto

Este proyecto implementa un sistema de **detección de movimiento en tiempo real** utilizando la cámara web, donde el área de movimiento detectado se delimita con un **Convex Hull calculado completamente a mano**, sin usar las funciones de convex hull de OpenCV.

El objetivo principal es demostrar el entendimiento matemático del algoritmo de envolvente convexa mediante su implementación desde cero en C++.

---

## Fundamento Teórico

### ¿Qué es el Convex Hull?

El **Convex Hull** (envolvente convexa) de un conjunto de puntos es el polígono convexo más pequeño que contiene todos los puntos del conjunto. Visualmente, es como si se lanzara una liga elástica alrededor de todos los puntos: la forma que toma la liga es el Convex Hull.

### Algoritmo Implementado: Monotone Chain (Andrew's Algorithm)

Se utilizó el algoritmo de **Cadena Monotónica de Andrew** (`O(n log n)`) por su eficiencia y precisión. El algoritmo funciona así:

1. **Ordenar** los puntos lexicográficamente (primero por X, luego por Y).
2. **Construir el Lower Hull** (parte inferior):
   - Iterar los puntos de izquierda a derecha.
   - Para cada punto, eliminar el último punto del hull si genera un giro en sentido horario (o es colineal), usando el **producto vectorial cruzado**.
3. **Construir el Upper Hull** (parte superior):
   - Iterar los puntos de derecha a izquierda con la misma lógica.
4. **Unir** ambas partes para formar el polígono convexo cerrado.

#### Producto Vectorial Cruzado

La clave del algoritmo es la función `crossProduct(O, A, B)`:

```
cross(O, A, B) = (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x)
```

- Si el resultado es **positivo** → giro anti-horario (punto válido para el hull)
- Si el resultado es **negativo o cero** → giro horario o colineal (punto descartado)

### Detección de Movimiento: Frame Differencing

Para detectar movimiento sin depender de algoritmos complejos de aprendizaje automático, se usó **diferencia de frames** (`Frame Differencing`):

1. Convertir el frame actual y el frame anterior a escala de grises.
2. Calcular la diferencia absoluta píxel a píxel: `diff = |frame_actual - frame_anterior|`
3. Umbralizar la diferencia para obtener una máscara binaria (blanco = movimiento, negro = estático).
4. Aplicar operaciones morfológicas (apertura y cierre) para eliminar ruido.
5. Encontrar contornos en la máscara y filtrar los de área pequeña.
6. Reunir todos los puntos de contornos significativos en un solo vector.
7. Calcular el **Convex Hull manual** sobre ese vector de puntos.

---

## Estructura del Proyecto

```
taller_convex_hull/
├── CMakeLists.txt          # Configuración de compilación con CMake
├── README.md               # Esta documentación
├── include/
│   └── convex_hull.hpp     # Declaración de la función de Convex Hull manual
├── src/
│   ├── convex_hull.cpp     # Implementación del algoritmo Monotone Chain
│   └── main.cpp            # Pipeline principal: cámara, detección, visualización
└── build/                  # Directorio de compilación (generado por CMake)
```

---

## Dependencias

| Dependencia | Uso permitido |
|-------------|---------------|
| **OpenCV** | Captura de cámara, operaciones morfológicas, umbralización, `findContours`, visualización |
| **OpenCV** ❌ | `cv::convexHull()` — **NO se usa**. El hull se calcula manualmente. |
| **C++ STL** | `std::sort`, `std::vector` |

---

## Compilación

```bash
# Crear directorio de build
mkdir -p build && cd build

# Configurar con CMake
cmake ..

# Compilar
make

# (Solo en macOS) Firmar para acceso a cámara
codesign --force --deep -s - ./ConvexHullMotionDetection
```

**Requisitos:**
- CMake ≥ 3.10
- OpenCV 4.x (instalado vía Homebrew en macOS: `brew install opencv`)
- Compilador C++17 compatible (AppleClang, GCC 9+)

---

## Ejecución

```bash
./build/ConvexHullMotionDetection
```

- **`q` o `ESC`**: Salir de la aplicación.
- Mueve las manos frente a la cámara. El Convex Hull (contorno rojo con relleno azul semitransparente) aparecerá sobre las zonas en movimiento.

---

## Visualización

| Elemento | Color | Descripción |
|----------|-------|-------------|
| Contorno del Hull | 🔴 Rojo | Aristas del Convex Hull calculado a mano |
| Vértices del Hull | 🟡 Amarillo | Puntos extremos que definen el hull |
| Relleno | 🔵 Azul translúcido | Área interior del hull |
| Texto | ⬜ Blanco | Número de vértices del hull actual |

**Ventana secundaria**: muestra la máscara binaria de movimiento en escala de grises.

---

## Resultados y Observaciones

- El algoritmo **Monotone Chain** genera correctamente el polígono convexo mínimo sobre el conjunto de puntos de movimiento.
- El **Frame Differencing** es efectivo para movimientos de manos rápidos. Movimientos muy lentos pueden no detectarse debido al umbral de diferencia.
- La variante de **media ponderada temporal** del frame anterior (`alpha = 0.4`) permite adaptarse gradualmente a cambios de iluminación sin perder reactividad.

---

## Referencias

- Andrew, A. M. (1979). *Another efficient algorithm for convex hulls in two dimensions.* Information Processing Letters.
- OpenCV Documentation: [Background Subtraction](https://docs.opencv.org/4.x/d1/dc5/tutorial_background_subtraction.html)
- Bradski, G., & Kaehler, A. (2008). *Learning OpenCV*. O'Reilly Media.
