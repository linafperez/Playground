# Filtros espaciales

Los filtros de suavizado reducen variaciones locales, los de enfoque realzan detalles y los operadores de bordes resaltan cambios de intensidad.

## Caballo

### Suavizado

| Original | Caja 3×3 | Gaussiano 5×5 |
|:---:|:---:|:---:|
| ![Caballo: original](resultados/caballo_original.jpg) | ![Caballo: filtro de caja](resultados/caballo_caja.jpg) | ![Caballo: filtro gaussiano](resultados/caballo_gaussiano.jpg) |

| Mediana 5×5 | Mínimo 5×5 | Máximo 5×5 |
|:---:|:---:|:---:|
| ![Caballo: filtro de mediana](resultados/caballo_mediana.jpg) | ![Caballo: filtro mínimo](resultados/caballo_minimo.jpg) | ![Caballo: filtro máximo](resultados/caballo_maximo.jpg) |

El suavizado reduce variaciones locales; mínimo y máximo expanden regiones oscuras y claras, respectivamente.

### Enfoque

| Original | Enfoque | Máscara de realce |
|:---:|:---:|:---:|
| ![Caballo: original](resultados/caballo_original.jpg) | ![Caballo: filtro de enfoque](resultados/caballo_enfoque.jpg) | ![Caballo: máscara de realce](resultados/caballo_mascara_realce.jpg) |

El enfoque acentúa las transiciones y la máscara de realce resalta detalles.

### Detección de bordes

| Sobel X | Sobel Y | Magnitud de Sobel |
|:---:|:---:|:---:|
| ![Caballo: Sobel X](resultados/caballo_sobel_x.png) | ![Caballo: Sobel Y](resultados/caballo_sobel_y.png) | ![Caballo: magnitud de Sobel](resultados/caballo_sobel.png) |

| Prewitt | Roberts | Laplaciano |
|:---:|:---:|:---:|
| ![Caballo: Prewitt](resultados/caballo_prewitt.png) | ![Caballo: Roberts](resultados/caballo_roberts.png) | ![Caballo: Laplaciano](resultados/caballo_laplaciano.png) |

| Original | Canny |
|:---:|:---:|
| ![Caballo: original](resultados/caballo_original.jpg) | ![Caballo: Canny](resultados/caballo_canny.png) |

Las derivadas muestran cambios de intensidad; Canny deja contornos binarios.

## Gato

### Suavizado

| Original | Caja 3×3 | Gaussiano 5×5 |
|:---:|:---:|:---:|
| ![Gato: original](resultados/gato_original.jpg) | ![Gato: filtro de caja](resultados/gato_caja.jpg) | ![Gato: filtro gaussiano](resultados/gato_gaussiano.jpg) |

| Mediana 5×5 | Mínimo 5×5 | Máximo 5×5 |
|:---:|:---:|:---:|
| ![Gato: filtro de mediana](resultados/gato_mediana.jpg) | ![Gato: filtro mínimo](resultados/gato_minimo.jpg) | ![Gato: filtro máximo](resultados/gato_maximo.jpg) |

### Enfoque

| Original | Enfoque | Máscara de realce |
|:---:|:---:|:---:|
| ![Gato: original](resultados/gato_original.jpg) | ![Gato: filtro de enfoque](resultados/gato_enfoque.jpg) | ![Gato: máscara de realce](resultados/gato_mascara_realce.jpg) |

### Detección de bordes

| Sobel X | Sobel Y | Magnitud de Sobel |
|:---:|:---:|:---:|
| ![Gato: Sobel X](resultados/gato_sobel_x.png) | ![Gato: Sobel Y](resultados/gato_sobel_y.png) | ![Gato: magnitud de Sobel](resultados/gato_sobel.png) |

| Prewitt | Roberts | Laplaciano |
|:---:|:---:|:---:|
| ![Gato: Prewitt](resultados/gato_prewitt.png) | ![Gato: Roberts](resultados/gato_roberts.png) | ![Gato: Laplaciano](resultados/gato_laplaciano.png) |

| Original | Canny |
|:---:|:---:|
| ![Gato: original](resultados/gato_original.jpg) | ![Gato: Canny](resultados/gato_canny.png) |

## Perro

### Suavizado

| Original | Caja 3×3 | Gaussiano 5×5 |
|:---:|:---:|:---:|
| ![Perro: original](resultados/perro_original.jpg) | ![Perro: filtro de caja](resultados/perro_caja.jpg) | ![Perro: filtro gaussiano](resultados/perro_gaussiano.jpg) |

| Mediana 5×5 | Mínimo 5×5 | Máximo 5×5 |
|:---:|:---:|:---:|
| ![Perro: filtro de mediana](resultados/perro_mediana.jpg) | ![Perro: filtro mínimo](resultados/perro_minimo.jpg) | ![Perro: filtro máximo](resultados/perro_maximo.jpg) |

### Enfoque

| Original | Enfoque | Máscara de realce |
|:---:|:---:|:---:|
| ![Perro: original](resultados/perro_original.jpg) | ![Perro: filtro de enfoque](resultados/perro_enfoque.jpg) | ![Perro: máscara de realce](resultados/perro_mascara_realce.jpg) |

### Detección de bordes

| Sobel X | Sobel Y | Magnitud de Sobel |
|:---:|:---:|:---:|
| ![Perro: Sobel X](resultados/perro_sobel_x.png) | ![Perro: Sobel Y](resultados/perro_sobel_y.png) | ![Perro: magnitud de Sobel](resultados/perro_sobel.png) |

| Prewitt | Roberts | Laplaciano |
|:---:|:---:|:---:|
| ![Perro: Prewitt](resultados/perro_prewitt.png) | ![Perro: Roberts](resultados/perro_roberts.png) | ![Perro: Laplaciano](resultados/perro_laplaciano.png) |

| Original | Canny |
|:---:|:---:|
| ![Perro: original](resultados/perro_original.jpg) | ![Perro: Canny](resultados/perro_canny.png) |

## Vacas

### Suavizado

| Original | Caja 3×3 | Gaussiano 5×5 |
|:---:|:---:|:---:|
| ![Vacas: original](resultados/vacas_original.jpg) | ![Vacas: filtro de caja](resultados/vacas_caja.jpg) | ![Vacas: filtro gaussiano](resultados/vacas_gaussiano.jpg) |

| Mediana 5×5 | Mínimo 5×5 | Máximo 5×5 |
|:---:|:---:|:---:|
| ![Vacas: filtro de mediana](resultados/vacas_mediana.jpg) | ![Vacas: filtro mínimo](resultados/vacas_minimo.jpg) | ![Vacas: filtro máximo](resultados/vacas_maximo.jpg) |

### Enfoque

| Original | Enfoque | Máscara de realce |
|:---:|:---:|:---:|
| ![Vacas: original](resultados/vacas_original.jpg) | ![Vacas: filtro de enfoque](resultados/vacas_enfoque.jpg) | ![Vacas: máscara de realce](resultados/vacas_mascara_realce.jpg) |

### Detección de bordes

| Sobel X | Sobel Y | Magnitud de Sobel |
|:---:|:---:|:---:|
| ![Vacas: Sobel X](resultados/vacas_sobel_x.png) | ![Vacas: Sobel Y](resultados/vacas_sobel_y.png) | ![Vacas: magnitud de Sobel](resultados/vacas_sobel.png) |

| Prewitt | Roberts | Laplaciano |
|:---:|:---:|:---:|
| ![Vacas: Prewitt](resultados/vacas_prewitt.png) | ![Vacas: Roberts](resultados/vacas_roberts.png) | ![Vacas: Laplaciano](resultados/vacas_laplaciano.png) |

| Original | Canny |
|:---:|:---:|
| ![Vacas: original](resultados/vacas_original.jpg) | ![Vacas: Canny](resultados/vacas_canny.png) |
