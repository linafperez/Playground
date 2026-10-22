cmake_minimum_required(VERSION 3.16)
project(vision_artificial_academica VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

find_package(OpenCV REQUIRED COMPONENTS core imgcodecs imgproc calib3d)

function(agregar_actividad nombre carpeta)
    add_executable(${nombre} "${carpeta}/${nombre}.cpp")
    target_include_directories(${nombre} PRIVATE ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(${nombre} PRIVATE ${OpenCV_LIBS})
    # Permite ejecutar desde cualquier directorio sin depender del directorio actual.
    target_compile_definitions(${nombre} PRIVATE RUTA_PROYECTO="${CMAKE_CURRENT_SOURCE_DIR}")
    if(MSVC)
        target_compile_options(${nombre} PRIVATE /W4 /utf-8)
    else()
        target_compile_options(${nombre} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

agregar_actividad(lectura_imagenes 01_lectura_imagenes)
agregar_actividad(transformaciones_geometricas 02_transformaciones_geometricas)
agregar_actividad(calibracion_camara 03_calibracion_camara)
agregar_actividad(filtros_espaciales 04_filtros_espaciales)
agregar_actividad(filtros_frecuencia 05_filtros_frecuencia)
agregar_actividad(histogramas_umbralizacion 06_histogramas_umbralizacion)
agregar_actividad(morfologia_matematica 07_morfologia_matematica)

