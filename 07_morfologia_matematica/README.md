# Morfología matemática

Erosión, dilatación, apertura y cierre comparan las regiones blancas y los cambios de intensidad en las representaciones binaria y gris.

## Huella

### Binaria · rectangular 3×3

| Original binaria | Erosión | Dilatación |
|:---:|:---:|:---:|
| ![Huella: original binaria](figuras/huella_binaria.png) | ![Huella: erosión en binaria](figuras/huella_erosion.png) | ![Huella: dilatación en binaria](figuras/huella_dilatacion.png) |

| Apertura | Cierre |
|:---:|:---:|
| ![Huella: apertura en binaria](figuras/huella_apertura.png) | ![Huella: cierre en binaria](figuras/huella_cierre.png) |

La erosión adelgaza las crestas, la dilatación reduce los huecos y el cierre une algunas separaciones pequeñas.

### Escala de grises · rectangular 5×5

| Original en gris | Erosión | Dilatación |
|:---:|:---:|:---:|
| ![Huella: original en gris](figuras/huella_gris.png) | ![Huella: erosión en gris](figuras/huella_gris_erosion.png) | ![Huella: dilatación en gris](figuras/huella_gris_dilatacion.png) |

| Apertura | Cierre |
|:---:|:---:|
| ![Huella: apertura en gris](figuras/huella_gris_apertura.png) | ![Huella: cierre en gris](figuras/huella_gris_cierre.png) |

## Letra

### Binaria · rectangular 3×3

| Original binaria | Erosión | Dilatación |
|:---:|:---:|:---:|
| ![Letra: original binaria](figuras/letra_binaria.png) | ![Letra: erosión en binaria](figuras/letra_erosion.png) | ![Letra: dilatación en binaria](figuras/letra_dilatacion.png) |

| Apertura | Cierre |
|:---:|:---:|
| ![Letra: apertura en binaria](figuras/letra_apertura.png) | ![Letra: cierre en binaria](figuras/letra_cierre.png) |

En la letra, la apertura suaviza salientes del trazo; la dilatación lo engrosa y la erosión lo afina.

### Escala de grises · rectangular 3×3

| Original en gris | Erosión | Dilatación |
|:---:|:---:|:---:|
| ![Letra: original en gris](figuras/letra_gris.png) | ![Letra: erosión en gris](figuras/letra_gris_erosion.png) | ![Letra: dilatación en gris](figuras/letra_gris_dilatacion.png) |

| Apertura | Cierre |
|:---:|:---:|
| ![Letra: apertura en gris](figuras/letra_gris_apertura.png) | ![Letra: cierre en gris](figuras/letra_gris_cierre.png) |

La versión gris coincide con la binaria porque la entrada solo contiene negro y blanco.

## Cat

### Binaria · rectangular 3×3

| Original binaria | Erosión | Dilatación |
|:---:|:---:|:---:|
| ![Cat: original binaria](figuras/cat_binaria.png) | ![Cat: erosión en binaria](figuras/cat_erosion.png) | ![Cat: dilatación en binaria](figuras/cat_dilatacion.png) |

| Apertura | Cierre |
|:---:|:---:|
| ![Cat: apertura en binaria](figuras/cat_apertura.png) | ![Cat: cierre en binaria](figuras/cat_cierre.png) |

### Escala de grises · rectangular 7×7

| Original en gris | Erosión | Dilatación |
|:---:|:---:|:---:|
| ![Cat: original en gris](figuras/cat_gris.png) | ![Cat: erosión en gris](figuras/cat_gris_erosion.png) | ![Cat: dilatación en gris](figuras/cat_gris_dilatacion.png) |

| Apertura | Cierre |
|:---:|:---:|
| ![Cat: apertura en gris](figuras/cat_gris_apertura.png) | ![Cat: cierre en gris](figuras/cat_gris_cierre.png) |

En gris, la erosión extiende las zonas oscuras y la dilatación expande las claras.
