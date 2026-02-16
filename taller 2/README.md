# Taller 2 - Procesamiento de Video en Tiempo Real

## Integrantes
- Miguel Flechas
- Andres Castro
- Juan Hurtado

## Descripcion
Implementacion de algoritmos de procesamiento de video en tiempo real utilizando C++ y OpenCV (solo para captura de camara y visualizacion). Todos los algoritmos de procesamiento de imagen estan implementados manualmente.

## Algoritmos Implementados

### Frame Differencing
Deteccion de movimiento mediante la diferencia absoluta entre un frame de referencia (fondo) y el frame actual. Implementado con iteracion pixel a pixel.

### Running Average
Calculo del promedio movil de los frames usando la formula:
```
avg = alpha * frame_actual + (1 - alpha) * avg_anterior
```

### Funciones Auxiliares Manuales
- **RGB a Escala de Grises**: Conversion manual usando `Y = 0.299R + 0.587G + 0.114B`
- **Redimensionamiento**: Nearest Neighbor manual
- **Normalizacion de Brillo**: Estiramiento de histograma manual
- **Concatenacion Horizontal**: Para el dashboard de visualizacion

## Estructura
```
taller 2/
├── CMakeLists.txt
├── include/
│   └── algorithms.hpp
├── src/
│   ├── main.cpp
│   ├── frame_differencing.cpp
│   └── frame_averaging.cpp
└── build/
```

## Compilacion y Ejecucion
```bash
cd "taller 2/build"
cmake ..
make
./main
```

## Controles
- `b` - Capturar/re-capturar fondo
- `q` - Salir
