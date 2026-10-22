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
    std::cout << p.filename() << ": canales reales=" << m.channels()
              << "; profundidad=" << m.depth() << std::endl;
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
void guardar(const fs::path& p, const cv::Mat& m) {
    if (m.empty()) throw std::runtime_error("Resultado vacio: " + p.u8string());
    if ((m.depth() != CV_8U && m.depth() != CV_16U) ||
        (m.channels() != 1 && m.channels() != 3 && m.channels() != 4))
        throw std::runtime_error("Preparar profundidad/canales antes de guardar: " + p.u8string());
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
void procesar(const cv::Mat& m, const fs::path& out) {
    guardar(out / "lectura/original.png", m);
    guardar(out / "conversiones/gris.png", gris(m));
    if (m.channels() == 1) return;
    cv::Mat bgr = ocho(m);
    if (m.channels() == 4) {
        cv::Mat alfa;
        cv::extractChannel(bgr, alfa, 3);
        guardar(out / "canales/alfa_intensidad.png", alfa);
        cv::cvtColor(bgr, bgr, cv::COLOR_BGRA2BGR);
    }
    if (bgr.channels() != 3) throw std::runtime_error("Se requieren tres canales BGR");
    std::vector<cv::Mat> canales;
    cv::split(bgr, canales);
    const std::vector<std::string> nombres = {"B", "G", "R"};
    for (std::size_t i = 0; i < canales.size(); ++i) {
        guardar(out / "canales" / (nombres[i] + "_intensidad.png"), canales[i]);
        if (i < 3) {
            std::vector<cv::Mat> aislado(3);
            for (auto& c : aislado) c = cv::Mat::zeros(m.size(), CV_8U);
            aislado[i] = canales[i];
            cv::Mat bgr;
            cv::merge(aislado, bgr);
            guardar(out / "canales" / (nombres[i] + "_color.png"), bgr);
        }
    }
}
}
int main(int argc, char** argv) {
    try { return lote(raiz(argc, argv) / "01_lectura_imagenes", procesar); }
    catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
