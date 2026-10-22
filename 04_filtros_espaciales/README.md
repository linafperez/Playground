# Filtros espaciales

Los filtros de suavizado reducen variaciones locales, los de enfoque realzan detalles y los operadores de bordes resaltan cambios de intensidad.

## Caballo

### Suavizado

| Original | Caja 3×3 | Gaussiano 5×5 |
|:---:|:---:|:---:|
| ![Caballo: original](figuras/caballo_original.jpg) | ![Caballo: filtro de caja](figuras/caballo_caja.jpg) | ![Caballo: filtro gaussiano](figuras/caballo_gaussiano.jpg) |

| Mediana 5×5 | Mínimo 5×5 | Máximo 5×5 |
|:---:|:---:|:---:|
| ![Caballo: filtro de mediana](figuras/caballo_mediana.jpg) | ![Caballo: filtro mínimo](figuras/caballo_minimo.jpg) | ![Caballo: filtro máximo](figuras/caballo_maximo.jpg) |

El suavizado reduce variaciones locales; mínimo y máximo expanden regiones oscuras y claras, respectivamente.

### Enfoque

| Original | Enfoque | Máscara de realce |
|:---:|:---:|:---:|
| ![Caballo: original](figuras/caballo_original.jpg) | ![Caballo: filtro de enfoque](figuras/caballo_enfoque.jpg) | ![Caballo: máscara de realce](figuras/caballo_mascara_realce.jpg) |

El enfoque acentúa las transiciones y la máscara de realce resalta detalles.

### Detección de bordes

| Sobel X | Sobel Y | Magnitud de Sobel |
|:---:|:---:|:---:|
| ![Caballo: Sobel X](figuras/caballo_sobel_x.png) | ![Caballo: Sobel Y](figuras/caballo_sobel_y.png) | ![Caballo: magnitud de Sobel](figuras/caballo_sobel.png) |

| Prewitt | Roberts | Laplaciano |
|:---:|:---:|:---:|
| ![Caballo: Prewitt](figuras/caballo_prewitt.png) | ![Caballo: Roberts](figuras/caballo_roberts.png) | ![Caballo: Laplaciano](figuras/caballo_laplaciano.png) |

| Original | Canny |
|:---:|:---:|
| ![Caballo: original](figuras/caballo_original.jpg) | ![Caballo: Canny](figuras/caballo_canny.png) |

Las derivadas muestran cambios de intensidad; Canny deja contornos binarios.

## Gato

### Suavizado

| Original | Caja 3×3 | Gaussiano 5×5 |
|:---:|:---:|:---:|
| ![Gato: original](figuras/gato_original.jpg) | ![Gato: filtro de caja](figuras/gato_caja.jpg) | ![Gato: filtro gaussiano](figuras/gato_gaussiano.jpg) |

| Mediana 5×5 | Mínimo 5×5 | Máximo 5×5 |
|:---:|:---:|:---:|
| ![Gato: filtro de mediana](figuras/gato_mediana.jpg) | ![Gato: filtro mínimo](figuras/gato_minimo.jpg) | ![Gato: filtro máximo](figuras/gato_maximo.jpg) |

### Enfoque

| Original | Enfoque | Máscara de realce |
|:---:|:---:|:---:|
| ![Gato: original](figuras/gato_original.jpg) | ![Gato: filtro de enfoque](figuras/gato_enfoque.jpg) | ![Gato: máscara de realce](figuras/gato_mascara_realce.jpg) |

### Detección de bordes

| Sobel X | Sobel Y | Magnitud de Sobel |
|:---:|:---:|:---:|
| ![Gato: Sobel X](figuras/gato_sobel_x.png) | ![Gato: Sobel Y](figuras/gato_sobel_y.png) | ![Gato: magnitud de Sobel](figuras/gato_sobel.png) |

| Prewitt | Roberts | Laplaciano |
|:---:|:---:|:---:|
| ![Gato: Prewitt](figuras/gato_prewitt.png) | ![Gato: Roberts](figuras/gato_roberts.png) | ![Gato: Laplaciano](figuras/gato_laplaciano.png) |

| Original | Canny |
|:---:|:---:|
| ![Gato: original](figuras/gato_original.jpg) | ![Gato: Canny](figuras/gato_canny.png) |

## Perro

### Suavizado

| Original | Caja 3×3 | Gaussiano 5×5 |
|:---:|:---:|:---:|
| ![Perro: original](figuras/perro_original.jpg) | ![Perro: filtro de caja](figuras/perro_caja.jpg) | ![Perro: filtro gaussiano](figuras/perro_gaussiano.jpg) |

| Mediana 5×5 | Mínimo 5×5 | Máximo 5×5 |
|:---:|:---:|:---:|
| ![Perro: filtro de mediana](figuras/perro_mediana.jpg) | ![Perro: filtro mínimo](figuras/perro_minimo.jpg) | ![Perro: filtro máximo](figuras/perro_maximo.jpg) |

### Enfoque

| Original | Enfoque | Máscara de realce |
|:---:|:---:|:---:|
| ![Perro: original](figuras/perro_original.jpg) | ![Perro: filtro de enfoque](figuras/perro_enfoque.jpg) | ![Perro: máscara de realce](figuras/perro_mascara_realce.jpg) |

### Detección de bordes

| Sobel X | Sobel Y | Magnitud de Sobel |
|:---:|:---:|:---:|
| ![Perro: Sobel X](figuras/perro_sobel_x.png) | ![Perro: Sobel Y](figuras/perro_sobel_y.png) | ![Perro: magnitud de Sobel](figuras/perro_sobel.png) |

| Prewitt | Roberts | Laplaciano |
|:---:|:---:|:---:|
| ![Perro: Prewitt](figuras/perro_prewitt.png) | ![Perro: Roberts](figuras/perro_roberts.png) | ![Perro: Laplaciano](figuras/perro_laplaciano.png) |

| Original | Canny |
|:---:|:---:|
| ![Perro: original](figuras/perro_original.jpg) | ![Perro: Canny](figuras/perro_canny.png) |

## Vacas

### Suavizado

| Original | Caja 3×3 | Gaussiano 5×5 |
|:---:|:---:|:---:|
| ![Vacas: original](figuras/vacas_original.jpg) | ![Vacas: filtro de caja](figuras/vacas_caja.jpg) | ![Vacas: filtro gaussiano](figuras/vacas_gaussiano.jpg) |

| Mediana 5×5 | Mínimo 5×5 | Máximo 5×5 |
|:---:|:---:|:---:|
| ![Vacas: filtro de mediana](figuras/vacas_mediana.jpg) | ![Vacas: filtro mínimo](figuras/vacas_minimo.jpg) | ![Vacas: filtro máximo](figuras/vacas_maximo.jpg) |

### Enfoque

| Original | Enfoque | Máscara de realce |
|:---:|:---:|:---:|
| ![Vacas: original](figuras/vacas_original.jpg) | ![Vacas: filtro de enfoque](figuras/vacas_enfoque.jpg) | ![Vacas: máscara de realce](figuras/vacas_mascara_realce.jpg) |

### Detección de bordes

| Sobel X | Sobel Y | Magnitud de Sobel |
|:---:|:---:|:---:|
| ![Vacas: Sobel X](figuras/vacas_sobel_x.png) | ![Vacas: Sobel Y](figuras/vacas_sobel_y.png) | ![Vacas: magnitud de Sobel](figuras/vacas_sobel.png) |

| Prewitt | Roberts | Laplaciano |
|:---:|:---:|:---:|
| ![Vacas: Prewitt](figuras/vacas_prewitt.png) | ![Vacas: Roberts](figuras/vacas_roberts.png) | ![Vacas: Laplaciano](figuras/vacas_laplaciano.png) |

| Original | Canny |
|:---:|:---:|
| ![Vacas: original](figuras/vacas_original.jpg) | ![Vacas: Canny](figuras/vacas_canny.png) |
