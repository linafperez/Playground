# Filtros en frecuencia

La transformada de Fourier y las máscaras espectrales permiten filtrar las frecuencias de una imagen y reconstruirla con distintos niveles de detalle.

## Lehar

| Original | Espectro de Fourier |
|:---:|:---:|
| ![Lehar: original en gris](figuras/lehar_original.png) | ![Lehar: espectro de Fourier](figuras/lehar_espectro.png) |

| Máscara | Vista de la máscara | Reconstrucción |
|:---:|:---:|:---:|
| mask1: disco pequeño | ![Máscara 1](figuras/mask1.png) | ![Lehar: reconstrucción con máscara 1](figuras/lehar_mask1.png) |
| mask2: disco grande | ![Máscara 2](figuras/mask2.png) | ![Lehar: reconstrucción con máscara 2](figuras/lehar_mask2.png) |
| mask3: cuadrado pequeño | ![Máscara 3](figuras/mask3.png) | ![Lehar: reconstrucción con máscara 3](figuras/lehar_mask3.png) |
| mask4: cuadrado grande | ![Máscara 4](figuras/mask4.png) | ![Lehar: reconstrucción con máscara 4](figuras/lehar_mask4.png) |

## Lena

| Original | Espectro de Fourier |
|:---:|:---:|
| ![Lena: original en gris](figuras/lena_original.png) | ![Lena: espectro de Fourier](figuras/lena_espectro.png) |

| Máscara | Vista de la máscara | Reconstrucción |
|:---:|:---:|:---:|
| mask1: disco pequeño | ![Máscara 1](figuras/mask1.png) | ![Lena: reconstrucción con máscara 1](figuras/lena_mask1.png) |
| mask2: disco grande | ![Máscara 2](figuras/mask2.png) | ![Lena: reconstrucción con máscara 2](figuras/lena_mask2.png) |
| mask3: cuadrado pequeño | ![Máscara 3](figuras/mask3.png) | ![Lena: reconstrucción con máscara 3](figuras/lena_mask3.png) |
| mask4: cuadrado grande | ![Máscara 4](figuras/mask4.png) | ![Lena: reconstrucción con máscara 4](figuras/lena_mask4.png) |

## Peppers

| Original | Espectro de Fourier |
|:---:|:---:|
| ![Peppers: original en gris](figuras/peppers_original.png) | ![Peppers: espectro de Fourier](figuras/peppers_espectro.png) |

| Máscara | Vista de la máscara | Reconstrucción |
|:---:|:---:|:---:|
| mask1: disco pequeño | ![Máscara 1](figuras/mask1.png) | ![Peppers: reconstrucción con máscara 1](figuras/peppers_mask1.png) |
| mask2: disco grande | ![Máscara 2](figuras/mask2.png) | ![Peppers: reconstrucción con máscara 2](figuras/peppers_mask2.png) |
| mask3: cuadrado pequeño | ![Máscara 3](figuras/mask3.png) | ![Peppers: reconstrucción con máscara 3](figuras/peppers_mask3.png) |
| mask4: cuadrado grande | ![Máscara 4](figuras/mask4.png) | ![Peppers: reconstrucción con máscara 4](figuras/peppers_mask4.png) |

Las máscaras pequeñas suavizan más la imagen; las grandes conservan mejor los detalles.

## Sin1

| Original | Espectro de Fourier |
|:---:|:---:|
| ![Sin1: original en gris](figuras/sin1_original.png) | ![Sin1: espectro de Fourier](figuras/sin1_espectro.png) |

| Máscara | Vista de la máscara | Reconstrucción |
|:---:|:---:|:---:|
| mask1: disco pequeño | ![Máscara 1](figuras/mask1.png) | ![Sin1: reconstrucción con máscara 1](figuras/sin1_mask1.png) |
| mask2: disco grande | ![Máscara 2](figuras/mask2.png) | ![Sin1: reconstrucción con máscara 2](figuras/sin1_mask2.png) |
| mask3: cuadrado pequeño | ![Máscara 3](figuras/mask3.png) | ![Sin1: reconstrucción con máscara 3](figuras/sin1_mask3.png) |
| mask4: cuadrado grande | ![Máscara 4](figuras/mask4.png) | ![Sin1: reconstrucción con máscara 4](figuras/sin1_mask4.png) |

Las cuatro máscaras conservan el patrón principal de franjas.
