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
    return m;
}
void guardar(const fs::path& p, const cv::Mat& m) {
    if (m.empty()) throw std::runtime_error("Resultado vacio: " + p.u8string());
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
cv::Mat transformar(const cv::Mat& m, cv::Mat h, bool perspectiva = false) {
    std::vector<cv::Point2f> vertices = {{0, 0}, {float(m.cols), 0},
        {float(m.cols), float(m.rows)}, {0, float(m.rows)}}, destino;
    if (perspectiva) cv::perspectiveTransform(vertices, destino, h);
    else cv::transform(vertices, destino, h);
    // Incluye el origen: una traslacion positiva conserva el margen desplazado.
    double minx = 0, miny = 0, maxx = 0, maxy = 0;
    for (auto p : destino) {
        minx = std::min(minx, double(p.x)); miny = std::min(miny, double(p.y));
        maxx = std::max(maxx, double(p.x)); maxy = std::max(maxy, double(p.y));
    }
    cv::Mat t = (cv::Mat_<double>(3, 3) << 1, 0, -std::floor(minx),
        0, 1, -std::floor(miny), 0, 0, 1);
    cv::Mat h3 = cv::Mat::eye(3, 3, CV_64F);
    h.copyTo(h3(cv::Rect(0, 0, 3, perspectiva ? 3 : 2)));
    cv::Mat ajustada = t * h3, r;
    cv::Size tam(int(std::ceil(maxx) - std::floor(minx)), int(std::ceil(maxy) - std::floor(miny)));
    if (perspectiva) cv::warpPerspective(m, r, ajustada, tam, cv::INTER_LINEAR);
    else cv::warpAffine(m, r, ajustada(cv::Rect(0, 0, 3, 2)), tam, cv::INTER_LINEAR);
    return r;
}
void procesar(const cv::Mat& m, const fs::path& out) {
    const double w = m.cols, h = m.rows;
    const cv::Point2d desplazamiento(0.22 * w, 0.16 * h);
    cv::Mat t = (cv::Mat_<double>(2, 3) << 1, 0, desplazamiento.x, 0, 1, desplazamiento.y);
    guardar(out / "afines/traslacion.png", transformar(m, t));
    guardar(out / "afines/rotacion_30.png", transformar(m,
        cv::getRotationMatrix2D(cv::Point2f(float(w / 2), float(h / 2)), 30, 1)));
    guardar(out / "afines/escala_075_125.png", transformar(m,
        (cv::Mat_<double>(2, 3) << .75, 0, 0, 0, 1.25, 0)));
    cv::Mat r;
    cv::flip(m, r, 1); guardar(out / "reflexiones/horizontal.png", r);
    cv::flip(m, r, 0); guardar(out / "reflexiones/vertical.png", r);
    guardar(out / "afines/cizalla_horizontal.png", transformar(m,
        (cv::Mat_<double>(2, 3) << 1, .3, 0, 0, 1, 0)));
    guardar(out / "afines/cizalla_vertical.png", transformar(m,
        (cv::Mat_<double>(2, 3) << 1, 0, 0, .25, 1, 0)));
    std::vector<cv::Point2f> a = {{0, 0}, {float(w), 0}, {0, float(h)}};
    std::vector<cv::Point2f> b = {{float(.10*w), float(.1*h)},
        {float(.9*w), float(.2*h)}, {float(.2*w), float(.95*h)}};
    guardar(out / "afines/tres_puntos.png", transformar(m, cv::getAffineTransform(a, b)));
    a = {{0, 0}, {float(w), 0}, {float(w), float(h)}, {0, float(h)}};
    b = {{float(.15*w), float(.05*h)}, {float(.85*w), float(.12*h)},
        {float(.98*w), float(.95*h)}, {float(.03*w), float(.85*h)}};
    guardar(out / "perspectiva/cuatro_puntos.png", transformar(m, cv::getPerspectiveTransform(a, b), true));
}
}
int main(int argc, char** argv) {
    try { return lote(raiz(argc, argv) / "02_transformaciones_geometricas", procesar); }
    catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
