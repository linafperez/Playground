# Histogramas y umbralización

Los histogramas describen la distribución del color o la intensidad; la ecualización modifica el contraste y los umbrales separan regiones claras y oscuras.

## Mango

### Histogramas y ecualización

| Original | Histograma |
|:---:|:---:|
| ![Mango: original a color](resultados/Mango_original.jpg) | ![Mango: histograma BGR](resultados/Mango_histograma.png) |

| Histograma azul | Histograma verde | Histograma rojo |
|:---:|:---:|:---:|
| ![Mango: histograma del canal azul](resultados/Mango_histograma_B.png) | ![Mango: histograma del canal verde](resultados/Mango_histograma_G.png) | ![Mango: histograma del canal rojo](resultados/Mango_histograma_R.png) |

| Original a color | Ecualización de luminancia |
|:---:|:---:|
| ![Mango: original a color](resultados/Mango_original.jpg) | ![Mango: ecualización de luminancia](resultados/Mango_ecualizada_color.jpg) |

| Escala de grises | Ecualizada en gris |
|:---:|:---:|
| ![Mango: escala de grises](resultados/Mango_gris.png) | ![Mango: ecualización en gris](resultados/Mango_ecualizada.png) |

| Histograma de intensidad original | Histograma ecualizado |
|:---:|:---:|
| ![Mango: histograma de intensidad](resultados/Mango_histograma_gris.png) | ![Mango: histograma ecualizado](resultados/Mango_histograma_ecualizado.png) |

### Umbralización

| Original en gris | Otsu | Adaptativa por media |
|:---:|:---:|:---:|
| ![Mango: escala de grises](resultados/Mango_gris.png) | ![Mango: Otsu](resultados/Mango_otsu.png) | ![Mango: umbralización adaptativa por media](resultados/Mango_adaptativa_media.png) |

| Original en gris | Adaptativa gaussiana |
|:---:|:---:|
| ![Mango: escala de grises](resultados/Mango_gris.png) | ![Mango: umbralización adaptativa gaussiana](resultados/Mango_adaptativa_gaussiana.png) |

## Camaraman

### Histogramas y ecualización

| Original | Histograma |
|:---:|:---:|
| ![Camaraman: original en gris](resultados/camaraman_original.png) | ![Camaraman: histograma de intensidad](resultados/camaraman_histograma.png) |

| Original | Ecualizada en gris |
|:---:|:---:|
| ![Camaraman: original en gris](resultados/camaraman_original.png) | ![Camaraman: ecualización en gris](resultados/camaraman_ecualizada.png) |

| Histograma de intensidad original | Histograma ecualizado |
|:---:|:---:|
| ![Camaraman: histograma de intensidad](resultados/camaraman_histograma.png) | ![Camaraman: histograma ecualizado](resultados/camaraman_histograma_ecualizado.png) |

### Umbralización

| Original en gris | Otsu | Adaptativa por media |
|:---:|:---:|:---:|
| ![Camaraman: original en gris](resultados/camaraman_original.png) | ![Camaraman: Otsu](resultados/camaraman_otsu.png) | ![Camaraman: umbralización adaptativa por media](resultados/camaraman_adaptativa_media.png) |

| Original en gris | Adaptativa gaussiana |
|:---:|:---:|
| ![Camaraman: original en gris](resultados/camaraman_original.png) | ![Camaraman: umbralización adaptativa gaussiana](resultados/camaraman_adaptativa.png) |

## Coins

### Histogramas y ecualización

| Original | Histograma |
|:---:|:---:|
| ![Coins: original en gris](resultados/coins_original.png) | ![Coins: histograma de intensidad](resultados/coins_histograma.png) |

| Original | Ecualizada en gris |
|:---:|:---:|
| ![Coins: original en gris](resultados/coins_original.png) | ![Coins: ecualización en gris](resultados/coins_ecualizada.png) |

| Histograma de intensidad original | Histograma ecualizado |
|:---:|:---:|
| ![Coins: histograma de intensidad](resultados/coins_histograma.png) | ![Coins: histograma ecualizado](resultados/coins_histograma_ecualizado.png) |

El marco blanco concentra gran parte del histograma; tras la ecualización, las monedas quedan más oscuras y aparece más textura en el fondo.

### Umbralización

| Original en gris | Otsu | Adaptativa por media |
|:---:|:---:|:---:|
| ![Coins: original en gris](resultados/coins_original.png) | ![Coins: Otsu](resultados/coins_otsu.png) | ![Coins: umbralización adaptativa por media](resultados/coins_adaptativa.png) |

| Original en gris | Adaptativa gaussiana |
|:---:|:---:|
| ![Coins: original en gris](resultados/coins_original.png) | ![Coins: umbralización adaptativa gaussiana](resultados/coins_adaptativa_gaussiana.png) |

Otsu forma regiones más uniformes; los métodos adaptativos conservan detalles locales y también resaltan textura.
