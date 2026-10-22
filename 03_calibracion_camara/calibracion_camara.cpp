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
    fs::create_directories(p.parent_path());
    if (!cv::imwrite(p.u8string(), m)) throw std::runtime_error("Fallo de escritura: " + p.u8string());
    ++salidas;
}
} // namespace
namespace {
void calibrar(const fs::path& actividad, const std::string& dispositivo) {
    const auto lista = archivos(actividad / "raw" / (dispositivo + "_images"));
    const fs::path out = actividad / "resultados" / dispositivo;
    const cv::Size tablero(8, 5);
    std::vector<cv::Point3f> patron;
    for (int y = 0; y < tablero.height; ++y)
        for (int x = 0; x < tablero.width; ++x) patron.emplace_back(x * 26.f, y * 26.f, 0);
    std::vector<std::vector<cv::Point2f>> puntos;
    std::vector<std::vector<cv::Point3f>> objetos;
    cv::Size resolucion;
    for (const auto& p : lista) {
        cv::Mat m = leer(p), g = gris(m), pequena;
        if (resolucion.empty()) resolucion = m.size();
        if (m.size() != resolucion) throw std::runtime_error("Resoluciones incompatibles: " + p.u8string());
        double escala = std::min(1.0, 1600.0 / std::max(m.cols, m.rows));
        cv::resize(g, pequena, cv::Size(), escala, escala, cv::INTER_AREA);
        cv::Size resolucionDeteccion = pequena.size();
        std::vector<cv::Point2f> esquinas;
        bool ok = cv::findChessboardCorners(pequena, tablero, esquinas,
            cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);
        if (!ok && escala < 1) {
            ok = cv::findChessboardCorners(g, tablero, esquinas,
                cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE);
            resolucionDeteccion = g.size();
        }
        if (!ok) {
            guardar(out / "sin_deteccion" / (p.stem().string() + ".png"), m);
            std::cout << dispositivo << "/" << p.filename() << ": sin tablero completo" << std::endl;
            continue;
        }
        for (auto& q : esquinas) {
            q.x *= float(g.cols) / resolucionDeteccion.width;
            q.y *= float(g.rows) / resolucionDeteccion.height;
        }
        cv::cornerSubPix(g, esquinas, cv::Size(11, 11), cv::Size(-1, -1),
            cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 50, .001));
        puntos.push_back(esquinas); objetos.push_back(patron);
        cv::Mat marcado = color(m);
        cv::drawChessboardCorners(marcado, tablero, esquinas, true);
        guardar(out / "esquinas" / (p.stem().string() + ".png"), marcado);
        std::cout << dispositivo << "/" << p.filename() << ": 40 esquinas" << std::endl;
    }
    if (puntos.size() < 3) throw std::runtime_error(dispositivo + ": se requieren al menos 3 vistas validas");
    if (puntos.size() < 10) std::cerr << dispositivo << ": menos de 10 vistas, calibracion poco robusta\n";
    cv::Mat k, dist;
    std::vector<cv::Mat> rotaciones, traslaciones;
    double rms = cv::calibrateCamera(objetos, puntos, resolucion, k, dist, rotaciones, traslaciones);
    if (!std::isfinite(rms) || !cv::checkRange(k) || !cv::checkRange(dist) ||
        k.at<double>(0, 0) <= 0 || k.at<double>(1, 1) <= 0)
        throw std::runtime_error(dispositivo + ": parametros de calibracion no validos");
    for (const auto& p : lista) {
        cv::Mat corregida;
        cv::undistort(leer(p), corregida, k, dist);
        guardar(out / "corregidas" / (p.stem().string() + ".png"), corregida);
    }
    std::cout << dispositivo << ": " << puntos.size() << "/" << lista.size()
              << " vistas validas; RMS=" << rms << " px; resolucion=" << resolucion << std::endl;
}
}
int main(int argc, char** argv) {
    try {
        const auto actividad = raiz(argc, argv) / "03_calibracion_camara";
        int errores = 0;
        for (const std::string d : {"iphone", "tablet"}) {
            try { calibrar(actividad, d); }
            catch (const std::exception& e) { std::cerr << e.what() << std::endl; ++errores; }
        }
        std::cout << "PNG escritos: " << salidas << std::endl;
        return errores ? 1 : 0;
    } catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
