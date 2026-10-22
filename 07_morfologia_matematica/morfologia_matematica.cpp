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
void guardar(const fs::path& p, const cv::Mat& m) {
    if (m.empty()) throw std::runtime_error("Resultado vacio: " + p.u8string());
    if (m.type() != CV_8UC1)
        throw std::runtime_error("Morfologia requiere una banda de 8 bits: " + p.u8string());
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
    cv::Mat g = gris(m), bin;
    const double umbral = cv::threshold(g, bin, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    // El fondo se estima por la clase mayoritaria del borde; el objeto queda blanco.
    cv::Mat borde = cv::Mat::zeros(bin.size(), CV_8U);
    borde.row(0).setTo(255); borde.row(bin.rows-1).setTo(255);
    borde.col(0).setTo(255); borde.col(bin.cols-1).setTo(255);
    const bool invertir = cv::mean(bin, borde)[0] > 127;
    if (invertir) cv::bitwise_not(bin, bin);
    std::cout << out.filename() << ": Otsu=" << umbral << "; invertir=" << invertir
              << "; primer plano=" << cv::countNonZero(bin) << "/" << bin.total() << std::endl;
    guardar(out / "binaria/entrada/otsu_primer_plano_blanco.png", bin);
    guardar(out / "gris/entrada/gris.png", g);
    int tam = std::clamp(cvRound(std::min(g.rows, g.cols)*.015), 3, 15);
    if (tam%2 == 0) ++tam;
    for (const auto& forma : std::vector<std::pair<int, std::string>>{
        {cv::MORPH_RECT,"rectangular"}, {cv::MORPH_ELLIPSE,"eliptico"}, {cv::MORPH_CROSS,"cruz"}}) {
        for (const auto& representacion : std::vector<std::pair<std::string, cv::Mat>>{{"binaria",bin},{"gris",g}}) {
            // Las crestas de huella tienen pocos pixeles: 5x5 eliminaba gran parte del objeto.
            const int lado = representacion.first == "binaria" ? 3 : tam;
            const auto e = cv::getStructuringElement(forma.first, cv::Size(lado,lado));
            for (const auto& op : std::vector<std::pair<int, std::string>>{
                {cv::MORPH_ERODE,"erosion"}, {cv::MORPH_DILATE,"dilatacion"},
                {cv::MORPH_OPEN,"apertura"}, {cv::MORPH_CLOSE,"cierre"}}) {
                cv::Mat r;
                cv::morphologyEx(representacion.second, r, op.first, e);
                const auto dir = out / representacion.first / op.second;
                const auto nombre = forma.second+"_"+std::to_string(lado)+"x"+std::to_string(lado)+".png";
                guardar(dir / nombre, r);
                // No deja el PNG del kernel anterior mezclado con el resultado vigente.
                for (const auto& anterior : fs::directory_iterator(dir)) {
                    const auto n = anterior.path().filename().string();
                    if (anterior.is_regular_file() && anterior.path().extension() == ".png" &&
                        n.rfind(forma.second+"_", 0) == 0 && n != nombre)
                        fs::remove(anterior.path());
                }
            }
        }
    }
}
}
int main(int argc, char** argv) {
    try { return lote(raiz(argc, argv) / "07_morfologia_matematica", procesar); }
    catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
