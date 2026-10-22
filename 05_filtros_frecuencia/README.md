# Filtros en frecuencia

La transformada de Fourier y las máscaras espectrales permiten filtrar las frecuencias de una imagen y reconstruirla con distintos niveles de detalle.

## Lehar

| Original | Espectro de Fourier |
|:---:|:---:|
| ![Lehar: original en gris](resultados/lehar_original.png) | ![Lehar: espectro de Fourier](resultados/lehar_espectro.png) |

| Máscara | Vista de la máscara | Reconstrucción |
|:---:|:---:|:---:|
| mask1: disco pequeño | ![Máscara 1](resultados/mask1.png) | ![Lehar: reconstrucción con máscara 1](resultados/lehar_mask1.png) |
| mask2: disco grande | ![Máscara 2](resultados/mask2.png) | ![Lehar: reconstrucción con máscara 2](resultados/lehar_mask2.png) |
| mask3: cuadrado pequeño | ![Máscara 3](resultados/mask3.png) | ![Lehar: reconstrucción con máscara 3](resultados/lehar_mask3.png) |
| mask4: cuadrado grande | ![Máscara 4](resultados/mask4.png) | ![Lehar: reconstrucción con máscara 4](resultados/lehar_mask4.png) |

## Lena

| Original | Espectro de Fourier |
|:---:|:---:|
| ![Lena: original en gris](resultados/lena_original.png) | ![Lena: espectro de Fourier](resultados/lena_espectro.png) |

| Máscara | Vista de la máscara | Reconstrucción |
|:---:|:---:|:---:|
| mask1: disco pequeño | ![Máscara 1](resultados/mask1.png) | ![Lena: reconstrucción con máscara 1](resultados/lena_mask1.png) |
| mask2: disco grande | ![Máscara 2](resultados/mask2.png) | ![Lena: reconstrucción con máscara 2](resultados/lena_mask2.png) |
| mask3: cuadrado pequeño | ![Máscara 3](resultados/mask3.png) | ![Lena: reconstrucción con máscara 3](resultados/lena_mask3.png) |
| mask4: cuadrado grande | ![Máscara 4](resultados/mask4.png) | ![Lena: reconstrucción con máscara 4](resultados/lena_mask4.png) |

## Peppers

| Original | Espectro de Fourier |
|:---:|:---:|
| ![Peppers: original en gris](resultados/peppers_original.png) | ![Peppers: espectro de Fourier](resultados/peppers_espectro.png) |

| Máscara | Vista de la máscara | Reconstrucción |
|:---:|:---:|:---:|
| mask1: disco pequeño | ![Máscara 1](resultados/mask1.png) | ![Peppers: reconstrucción con máscara 1](resultados/peppers_mask1.png) |
| mask2: disco grande | ![Máscara 2](resultados/mask2.png) | ![Peppers: reconstrucción con máscara 2](resultados/peppers_mask2.png) |
| mask3: cuadrado pequeño | ![Máscara 3](resultados/mask3.png) | ![Peppers: reconstrucción con máscara 3](resultados/peppers_mask3.png) |
| mask4: cuadrado grande | ![Máscara 4](resultados/mask4.png) | ![Peppers: reconstrucción con máscara 4](resultados/peppers_mask4.png) |

Las máscaras pequeñas suavizan más la imagen; las grandes conservan mejor los detalles.

## Sin1

| Original | Espectro de Fourier |
|:---:|:---:|
| ![Sin1: original en gris](resultados/sin1_original.png) | ![Sin1: espectro de Fourier](resultados/sin1_espectro.png) |

| Máscara | Vista de la máscara | Reconstrucción |
|:---:|:---:|:---:|
| mask1: disco pequeño | ![Máscara 1](resultados/mask1.png) | ![Sin1: reconstrucción con máscara 1](resultados/sin1_mask1.png) |
| mask2: disco grande | ![Máscara 2](resultados/mask2.png) | ![Sin1: reconstrucción con máscara 2](resultados/sin1_mask2.png) |
| mask3: cuadrado pequeño | ![Máscara 3](resultados/mask3.png) | ![Sin1: reconstrucción con máscara 3](resultados/sin1_mask3.png) |
| mask4: cuadrado grande | ![Máscara 4](resultados/mask4.png) | ![Sin1: reconstrucción con máscara 4](resultados/sin1_mask4.png) |

Las cuatro máscaras conservan el patrón principal de franjas.
