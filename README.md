# Simulaciones de incendios

## Modelo de la simulación

   En este proyecto se hacen simulaciones de incendios parametrizadas. La parametrización ocurre de la siguiente manera:

   El terreno esta dividido por celdas (que podrían también llamarse pixeles) y para cada celda se tienen los siguientes datos:

```
burnable ∈ {True, False}
vegetation ∈ {scrub, subalpine, wet, dry}
fwi ∈ ℝ
aspect ∈ [-1, 1]
wind ∈ [0, 2π]
elevation ∈ ℝ
```

   Y para ejecutar la simulación se pasan los siguientes parámetros:

```
β_subalpine ∈ ℝ
β_wet ∈ ℝ
β_dry ∈ ℝ
β_fwi ∈ (0, ∞)
β_aspect ∈ (0, ∞)
β_wind ∈ (0, ∞)
β_elevation(-∞, 0)
β_slope ∈ (0, ∞)
```

   Con eso, la simulación se va ejecutando por pasos, en cada paso hay un conjunto de celdas que son las que se están quemando en ese paso y para cada una de ellas, se calcula la probabilidad de que cada uno de sus vecinos que aun no se quemaron se contagie, usando los datos de la celda, las de la celda vecina y los parámetros.

   La fórmulas exactas de la probabilidad se pueden ver en la función `spread_probability` de `spread_functions.cpp`.

   Luego de calcular cada probabilidad se decide si se quema o no con una variable aleatoria bernoulli.

### Ejemplo

   A continuación se pueden ver imágenes de los primeros 9 pasos en una simulación:

| ![frame0.png](./imgs/frame0.png) | ![frame1.png](./imgs/frame1.png) | ![frame2.png](./imgs/frame2.png) |
| -------------------------------- | -------------------------------- | -------------------------------- |
| ![frame3.png](./imgs/frame3.png) | ![frame4.png](./imgs/frame4.png) | ![frame5.png](./imgs/frame5.png) |
| ![frame6.png](./imgs/frame6.png) | ![frame7.png](./imgs/frame7.png) | ![frame8.png](./imgs/frame8.png) |

## Objetivos

   Hay varios motivos para los cuales se usan las simulaciones:

1) Dados los parámetros, obtener animaciones bonitas de como se desparrama un incendio.

2) Dados los parámetros, calcular la probabilidad de que cada celda se queme haciendo muchas simulaciones.

3) Estimar una función que diga que tan realistas son los incendios producidos por cada combinación de parámetros, usando datos de incendios reales. Esto también haciendo muchas simulaciones.
