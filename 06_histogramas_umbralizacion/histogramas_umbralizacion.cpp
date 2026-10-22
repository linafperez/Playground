#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <functional>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>
#ifndef RUTA_PROYECTO
#define RUTA_PROYECTO "."
#endif
namespace fs = std::filesystem;
namespace {
std::size_t salidas = 0;
fs::path raiz(int argc, char** argv) {
    if (argc > 2) throw std::runtime_error("Uso: ejecutable [raiz_de_Playground]");
    return fs::absolute(argc == 2 ? fs::u8path(argv[1]) : fs::u8path(RUTA_PROYECTO));
}
std::vector<fs::path> archivos(const fs::path& dir) {
    if (!fs::is_directory(dir)) throw std::runtime_error("No existe: " + dir.u8string());
    std::vector<fs::path> lista;
    std::set<std::string> nombres;
    for (const auto& e : fs::directory_iterator(dir)) {
        if (!e.is_regular_file() || e.path().filename().string().front() == '.') continue;
        if (!cv::haveImageReader(e.path().u8string()))
            throw std::runtime_error("Entrada sin decodificador: " + e.path().u8string());
        if (!nombres.insert(e.path().stem().u8string()).second)
            throw std::runtime_error("Nombres base repetidos en: " + dir.u8string());
        lista.push_back(e.path());
    }
    std::sort(lista.begin(), lista.end());
    if (lista.empty()) throw std::runtime_error("Sin imagenes en: " + dir.u8string());
    return lista;
}
cv::Mat leer(const fs::path& p) {
    cv::Mat m = cv::imread(p.u8string(), cv::IMREAD_UNCHANGED);
    if (m.empty()) throw std::runtime_error("No se pudo leer: " + p.u8string());
    std::cout << p.filename() << ": canales reales=" << m.channels() << std::endl;
    return m;
}
cv::Mat ocho(const cv::Mat& m) {
    if (m.depth() == CV_8U) return m;
    cv::Mat f, r;
    m.convertTo(f, CV_32F);
    cv::normalize(f, r, 0, 255, cv::NORM_MINMAX, CV_8U);
    return r;
}
cv::Mat gris(const cv::Mat& m) {
    cv::Mat g;
    if (m.channels() == 1) return ocho(m);
    cv::cvtColor(m, g, m.channels() == 4 ? cv::COLOR_BGRA2GRAY : cv::COLOR_BGR2GRAY);
    return ocho(g);
}
cv::Mat color(const cv::Mat& m) {
    cv::Mat b;
    if (m.channels() == 1) cv::cvtColor(ocho(m), b, cv::COLOR_GRAY2BGR);
    else if (m.channels() == 4) cv::cvtColor(ocho(m), b, cv::COLOR_BGRA2BGR);
    else b = ocho(m);
    return b;
}
void guardar(const fs::path& p, const cv::Mat& m) {
    if (m.empty()) throw std::runtime_error("Resultado vacio: " + p.u8string());
    if (m.depth() != CV_8U || (m.channels() != 1 && m.channels() != 3))
        throw std::runtime_error("Preparar visualizacion de 8 bits: " + p.u8string());
    fs::create_directories(p.parent_path());
    if (!cv::imwrite(p.u8string(), m)) throw std::runtime_error("Fallo de escritura: " + p.u8string());
    ++salidas;
}
int lote(const fs::path& actividad,
         const std::function<void(const cv::Mat&, const fs::path&)>& procesar) {
    const auto lista = archivos(actividad / "raw");
    int fallos = 0;
    for (const auto& p : lista) {
        try {
            procesar(leer(p), actividad / "resultados" / p.stem());
            std::cout << "Procesada: " << p.filename() << std::endl;
        } catch (const std::exception& e) {
            ++fallos;
            std::cerr << p.filename() << ": " << e.what() << std::endl;
        }
    }
    std::cout << "Entradas: " << lista.size() << "; fallos: " << fallos
              << "; PNG escritos: " << salidas << std::endl;
    return fallos ? 1 : 0;
}
} // namespace
namespace {
cv::Mat histograma(const std::vector<cv::Mat>& canales, const std::vector<cv::Scalar>& colores) {
    cv::Mat grafica(430, 650, CV_8UC3, cv::Scalar(255,255,255));
    const int izq = 80, arriba = 40, ancho = 510, alto = 310;
    cv::line(grafica, {izq, arriba}, {izq, arriba+alto}, cv::Scalar(0,0,0), 1);
    cv::line(grafica, {izq, arriba+alto}, {izq+ancho, arriba+alto}, cv::Scalar(0,0,0), 1);
    for (int i = 0; i <= 4; ++i) {
        const int yy = arriba + alto - alto*i/4;
        cv::line(grafica, {izq, yy}, {izq+ancho, yy}, cv::Scalar(225,225,225));
        cv::putText(grafica, cv::format("%.2f", i/4.), {20, yy+5}, cv::FONT_HERSHEY_SIMPLEX, .45, cv::Scalar(0,0,0), 1, cv::LINE_AA);
    }
    for (int v : {0,64,128,192,255})
        cv::putText(grafica, std::to_string(v), {izq+2*v-10, arriba+alto+22}, cv::FONT_HERSHEY_SIMPLEX, .45, cv::Scalar(0,0,0), 1, cv::LINE_AA);
    cv::putText(grafica, "Frecuencia / maximo de cada canal", {80, 24}, cv::FONT_HERSHEY_SIMPLEX, .55, cv::Scalar(0,0,0), 1, cv::LINE_AA);
    cv::putText(grafica, "Intensidad (0-255)", {230, 405}, cv::FONT_HERSHEY_SIMPLEX, .6, cv::Scalar(0,0,0), 1, cv::LINE_AA);
    for (std::size_t c = 0; c < canales.size(); ++c) {
        cv::Mat hist;
        int bins = 256, indice = 0;
        float rango[] = {0,256}; const float* rangos[] = {rango};
        cv::calcHist(&canales[c], 1, &indice, cv::Mat(), hist, 1, &bins, rangos);
        double maximo; cv::minMaxLoc(hist, nullptr, &maximo);
        hist /= float(std::max(1., maximo));
        for (int i = 1; i < 256; ++i)
            cv::line(grafica, {izq+2*(i-1), arriba+alto-cvRound(alto*hist.at<float>(i-1))},
                {izq+2*i, arriba+alto-cvRound(alto*hist.at<float>(i))}, colores[c], 2, cv::LINE_AA);
    }
    if (canales.size() == 3) {
        for (int c = 0; c < 3; ++c)
            cv::putText(grafica, std::vector<std::string>{"B", "G", "R"}[c],
                {500+35*c, 390}, cv::FONT_HERSHEY_SIMPLEX, .6, colores[c], 2, cv::LINE_AA);
    }
    return grafica;
}
void procesar(const cv::Mat& m, const fs::path& out) {
    cv::Mat g = gris(m), r;
    const bool esColor = out.filename() == "Mango" || out.filename() == "mango";
    if (esColor && m.channels() != 3 && m.channels() != 4)
        throw std::runtime_error("Mango debe contener color BGR");
    guardar(out / "entrada/original.png", esColor ? color(m) : g);
    guardar(out / "entrada/gris.png", g);
    std::cout << out.filename() << ": representacion=" << (esColor ? "BGR y gris" : "gris") << std::endl;
    guardar(out / "histogramas/gris_normalizado.png", histograma({g}, {cv::Scalar(20,20,20)}));
    if (esColor) {
        std::vector<cv::Mat> canales; cv::split(color(m), canales);
        std::vector<cv::Scalar> colores = {{220,40,0}, {0,150,0}, {0,0,220}};
        const std::vector<std::string> nombres = {"B", "G", "R"};
        for (int i = 0; i < 3; ++i)
            guardar(out / "histogramas" / (nombres[i]+"_normalizado.png"), histograma({canales[i]}, {colores[i]}));
        guardar(out / "histogramas/BGR_normalizado.png", histograma(canales, colores));
        cv::Mat ycrcb, ec;
        cv::cvtColor(color(m), ycrcb, cv::COLOR_BGR2YCrCb); cv::split(ycrcb, canales);
        cv::equalizeHist(canales[0], canales[0]); cv::merge(canales, ycrcb);
        cv::cvtColor(ycrcb, ec, cv::COLOR_YCrCb2BGR);
        guardar(out / "ecualizacion/color_luminancia.png", ec);
    } else {
        // Retira solo salidas de color obsoletas generadas por versiones anteriores.
        for (const auto& nombre : {"B_normalizado.png", "G_normalizado.png", "R_normalizado.png", "BGR_normalizado.png"})
            fs::remove(out / "histogramas" / nombre);
        fs::remove(out / "ecualizacion/color_luminancia.png");
    }
    cv::equalizeHist(g, r); guardar(out / "ecualizacion/gris.png", r);
    guardar(out / "histogramas/gris_ecualizado_normalizado.png", histograma({r}, {cv::Scalar(20,20,20)}));
    cv::threshold(g, r, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU); guardar(out / "umbrales/otsu.png", r);
    int bloque = std::min(31, std::min(g.rows, g.cols)); if (bloque%2 == 0) --bloque;
    if (bloque < 3) throw std::runtime_error("Imagen demasiado pequena para umbral adaptativo");
    cv::adaptiveThreshold(g, r, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, bloque, 5);
    guardar(out / "umbrales/adaptativa_media.png", r);
    cv::adaptiveThreshold(g, r, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, bloque, 5);
    guardar(out / "umbrales/adaptativa_gaussiana.png", r);
    for (const auto& par : std::vector<std::pair<int, std::string>>{
        {cv::THRESH_BINARY,"binaria_127"}, {cv::THRESH_BINARY_INV,"binaria_inversa_127"},
        {cv::THRESH_TRUNC,"truncamiento_127"}, {cv::THRESH_TOZERO,"a_cero_127"},
        {cv::THRESH_TOZERO_INV,"a_cero_inversa_127"}}) {
        cv::threshold(g, r, 127, 255, par.first); guardar(out / "umbrales" / (par.second+".png"), r);
    }
}
}
int main(int argc, char** argv) {
    try { return lote(raiz(argc, argv) / "06_histogramas_umbralizacion", procesar); }
    catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
