# Entrega 1

## Métrica

Debido a que lo que buscamos es mejorar la performance de una simulación decidimos tomar como métrica el tiempo que nos lleva correr un paso de la simulación, y además para poder normalizar los resultados para diferentes tamaños de landscapes y fuegos lo dividimos por la cantidad de celdas que se están activamente quemando en el paso actual.

Todos los gráficos que presentaremos tendrán `burned cells per second` como métrica principal, corriendo la simulación con 30 partículas aleatorias y 2 veces para cada una de estas.

## Ambiente

Todos los benchmarks fueron corridos en atom.

* `AMD EPYC 7643 48-Core Processor`
* `128 GB de RAM`

### Compiladores

* `g++ (Debian 12.2.0-14) 12.2.0`
* `clang++ Debian clang version 16.0.0`

### Landscapes

| -      | nombre   | tamaño (x,y)|
|  ----- | -------- | --------    |
|Grande: | 2021_865 |(1961,2395)  |
|Mediano:| 1999_25j |(366,344)    |
|Chico:  | 2013_12  |(45,51)      |

## Benchmarks

Las barras de errores es la desviación estándar, las barras son la media de las mediciones.

### -O 0 1 2 3 fast

<img src="https://drive.google.com/uc?id=1hV0CM_-jO92bBlygKUHFneIFvAfA-d0_">

<img src="https://drive.google.com/uc?id=10CuOCW86WXEmeEELj4eMA9VQ16nN6zVL">

<img src="https://drive.google.com/uc?id=1SXoIipC21efhGkvNvg__DuMBnOv-V-5D">

-Ofast es la más rápida, y sus optimizaciones producen márgenes de error aceptables.

En general clang esta bastante a la par de g++ de no ser por -Ofast donde las optimizaciones de g++ son más agresivas, resultando en mejor performance

### - flto fno-rtti fno-threadsafe-statics funroll-loops march=native

Probamos todas estas flags combinadas con -Ofast

<img src="https://drive.google.com/uc?id=1YCE9wZDK1P1XYXNrJvSnaNEkZQxeilyr">

<img src="https://drive.google.com/uc?id=170c6y9FQtSCF6T6-KeUj4LLN-an0fw_1">

<img src="https://drive.google.com/uc?id=18yJDD3t_YNy_CRs6v-zH3cNGbLgkr0v0">

<img src="https://drive.google.com/uc?id=1b3mI9ASNR5-_N-t2UE5zUvbHdcAXm5F7">

<img src="https://drive.google.com/uc?id=1JvB7Z9fiFFVxc2OL91QO7mjUpSSPTORH">

<img src="https://drive.google.com/uc?id=1z1cYxuGaDF6eUp8BLuE1q_Vd_RmgcorE">

-flto es la mejor a pesar de igualmente encontrarse dentro de los margenes de error de la medición

### Algunas combinaciones

<img src="https://drive.google.com/uc?id=18yJDD3t_YNy_CRs6v-zH3cNGbLgkr0v0">

<img src="https://drive.google.com/uc?id=1fQPkXEA0uYQ95H3OlVCxcJ9LrYDCM4ly">

### Conclusión

La mejor combinación para nuestro proyecto es usar g++ con -Ofast -march=native -flto


### Memory leak

