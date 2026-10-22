#!/usr/bin/env bash
set -euo pipefail
raiz="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cmake -S "$raiz" -B "$raiz/build" -DCMAKE_BUILD_TYPE=Release
cmake --build "$raiz/build" --parallel 4
for actividad in lectura_imagenes transformaciones_geometricas calibracion_camara filtros_espaciales filtros_frecuencia histogramas_umbralizacion morfologia_matematica; do
    "$raiz/build/bin/$actividad" "$raiz"
done
