# Morfología matemática

Erosión, dilatación, apertura y cierre comparan las regiones blancas y los cambios de intensidad en las representaciones binaria y gris.

## Huella

### Binaria · rectangular 3×3

| Original binaria | Erosión | Dilatación |
|:---:|:---:|:---:|
| ![Huella: original binaria](resultados/huella_binaria.png) | ![Huella: erosión en binaria](resultados/huella_erosion.png) | ![Huella: dilatación en binaria](resultados/huella_dilatacion.png) |

| Apertura | Cierre |
|:---:|:---:|
| ![Huella: apertura en binaria](resultados/huella_apertura.png) | ![Huella: cierre en binaria](resultados/huella_cierre.png) |

La erosión adelgaza las crestas, la dilatación reduce los huecos y el cierre une algunas separaciones pequeñas.

### Escala de grises · rectangular 5×5

| Original en gris | Erosión | Dilatación |
|:---:|:---:|:---:|
| ![Huella: original en gris](resultados/huella_gris.png) | ![Huella: erosión en gris](resultados/huella_gris_erosion.png) | ![Huella: dilatación en gris](resultados/huella_gris_dilatacion.png) |

| Apertura | Cierre |
|:---:|:---:|
| ![Huella: apertura en gris](resultados/huella_gris_apertura.png) | ![Huella: cierre en gris](resultados/huella_gris_cierre.png) |

## Letra

### Binaria · rectangular 3×3

| Original binaria | Erosión | Dilatación |
|:---:|:---:|:---:|
| ![Letra: original binaria](resultados/letra_binaria.png) | ![Letra: erosión en binaria](resultados/letra_erosion.png) | ![Letra: dilatación en binaria](resultados/letra_dilatacion.png) |

| Apertura | Cierre |
|:---:|:---:|
| ![Letra: apertura en binaria](resultados/letra_apertura.png) | ![Letra: cierre en binaria](resultados/letra_cierre.png) |

En la letra, la apertura suaviza salientes del trazo; la dilatación lo engrosa y la erosión lo afina.

### Escala de grises · rectangular 3×3

| Original en gris | Erosión | Dilatación |
|:---:|:---:|:---:|
| ![Letra: original en gris](resultados/letra_gris.png) | ![Letra: erosión en gris](resultados/letra_gris_erosion.png) | ![Letra: dilatación en gris](resultados/letra_gris_dilatacion.png) |

| Apertura | Cierre |
|:---:|:---:|
| ![Letra: apertura en gris](resultados/letra_gris_apertura.png) | ![Letra: cierre en gris](resultados/letra_gris_cierre.png) |

La versión gris coincide con la binaria porque la entrada solo contiene negro y blanco.

## Cat

### Binaria · rectangular 3×3

| Original binaria | Erosión | Dilatación |
|:---:|:---:|:---:|
| ![Cat: original binaria](resultados/cat_binaria.png) | ![Cat: erosión en binaria](resultados/cat_erosion.png) | ![Cat: dilatación en binaria](resultados/cat_dilatacion.png) |

| Apertura | Cierre |
|:---:|:---:|
| ![Cat: apertura en binaria](resultados/cat_apertura.png) | ![Cat: cierre en binaria](resultados/cat_cierre.png) |

### Escala de grises · rectangular 7×7

| Original en gris | Erosión | Dilatación |
|:---:|:---:|:---:|
| ![Cat: original en gris](resultados/cat_gris.png) | ![Cat: erosión en gris](resultados/cat_gris_erosion.png) | ![Cat: dilatación en gris](resultados/cat_gris_dilatacion.png) |

| Apertura | Cierre |
|:---:|:---:|
| ![Cat: apertura en gris](resultados/cat_gris_apertura.png) | ![Cat: cierre en gris](resultados/cat_gris_cierre.png) |

En gris, la erosión extiende las zonas oscuras y la dilatación expande las claras.
