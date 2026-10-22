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
    if (m.channels() != 1 && m.channels() != 3 && m.channels() != 4)
        throw std::runtime_error("Canales no soportados: " + p.u8string());
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
        throw std::runtime_error("Preparar Fourier como una banda de 8 bits: " + p.u8string());
    fs::create_directories(p.parent_path());
    if (!cv::imwrite(p.u8string(), m)) throw std::runtime_error("Fallo de escritura: " + p.u8string());
    ++salidas;
}
cv::Mat magnitudVisible(const cv::Mat& m) {
    cv::Mat a = cv::abs(m), r;
    cv::normalize(a, r, 0, 255, cv::NORM_MINMAX, CV_8U);
    return r;
}
} // namespace
namespace {
cv::Mat desplazar(const cv::Mat& m, int dx, int dy) {
    cv::Mat r(m.size(), m.type());
    for (int y = 0; y < m.rows; ++y) {
        const int yy = (y + dy) % m.rows;
        m(cv::Rect(0, y, m.cols - dx, 1)).copyTo(r(cv::Rect(dx, yy, m.cols - dx, 1)));
        if (dx) m(cv::Rect(m.cols - dx, y, dx, 1)).copyTo(r(cv::Rect(0, yy, dx, 1)));
    }
    return r;
}
cv::Mat espectro(const cv::Mat& compleja, double& maximoLog) {
    std::vector<cv::Mat> p;
    cv::split(compleja, p);
    cv::Mat mag, r;
    cv::magnitude(p[0], p[1], mag);
    cv::log(mag + 1, mag);
    // Una escala comun permite comparar el espectro original y los filtrados.
    // El cero sigue siendo negro y no se amplifica un residuo numerico vacio.
    if (maximoLog <= 0) cv::minMaxLoc(mag, nullptr, &maximoLog);
    mag.convertTo(r, CV_8U, maximoLog > 0 ? 255. / maximoLog : 0.);
    return r;
}
void procesar(const fs::path& p, const fs::path& actividad, const std::vector<fs::path>& mascaras) {
    cv::Mat g = gris(leer(p)), f, fourier;
    // Intensidades [0,255]: log(1+|F|) no oculta las componentes debiles.
    g.convertTo(f, CV_32F);
    cv::dft(f, fourier, cv::DFT_COMPLEX_OUTPUT);
    cv::Mat centrada = desplazar(fourier, g.cols / 2, g.rows / 2);
    const auto out = actividad / "resultados" / p.stem();
    guardar(out / "fourier/entrada_gris.png", g);
    double maximoLog = 0;
    guardar(out / "fourier/espectro_log_centrado.png", espectro(centrada, maximoLog));
    // No se recortan filas/columnas impares: el shift circular conserva toda la DFT.
    for (const auto& ruta : mascaras) {
        cv::Mat original = gris(leer(ruta)), mask;
        cv::resize(original, mask, g.size(), 0, 0, cv::INTER_NEAREST);
        double maximoMask;
        cv::minMaxLoc(mask, nullptr, &maximoMask);
        // Admite tanto mascaras 0/1 como las habituales 0/255.
        mask.convertTo(mask, CV_32F, maximoMask > 1 ? 1.0 / 255 : 1.0);
        // Proyecta la mascara real sobre la simetria conjugada del DFT.
        // Con mascaras binarias simetricas este paso no cambia sus valores.
        cv::Mat origen = desplazar(mask, (g.cols + 1) / 2, (g.rows + 1) / 2);
        cv::Mat pareja(origen.size(), CV_32F);
        for (int y = 0; y < origen.rows; ++y)
            for (int x = 0; x < origen.cols; ++x)
                pareja.at<float>(y, x) = origen.at<float>((origen.rows-y)%origen.rows, (origen.cols-x)%origen.cols);
        origen = .5 * (origen + pareja);
        cv::Mat efectiva = desplazar(origen, g.cols / 2, g.rows / 2);
        std::vector<cv::Mat> planos;
        cv::split(centrada, planos);
        cv::multiply(planos[0], efectiva, planos[0]);
        cv::multiply(planos[1], efectiva, planos[1]);
        cv::Mat filtrada, inversa, reconstruida;
        cv::merge(planos, filtrada);
        cv::idft(desplazar(filtrada, (g.cols + 1) / 2, (g.rows + 1) / 2),
            inversa, cv::DFT_SCALE | cv::DFT_COMPLEX_OUTPUT);
        std::vector<cv::Mat> espacial;
        cv::split(inversa, espacial);
        double residuo = cv::norm(espacial[1], cv::NORM_INF);
        if (residuo > 1e-5 * std::max(1., cv::norm(espacial[0], cv::NORM_INF)))
            throw std::runtime_error("Residuo imaginario excesivo");
        // Escala fija: evita que la normalizacion aparente recuperar el brillo DC.
        espacial[0].convertTo(reconstruida, CV_8U);
        const auto dir = out / "mascaras" / ruta.stem();
        cv::Mat visual;
        efectiva.convertTo(visual, CV_8U, 255);
        guardar(dir / "mascara_aplicada.png", visual);
        guardar(dir / "espectro_filtrado.png", espectro(filtrada, maximoLog));
        guardar(dir / "reconstruida.png", reconstruida);
        guardar(dir / "respuesta_absoluta_normalizada.png", magnitudVisible(espacial[0]));
        std::cout << p.filename() << " + " << ruta.filename() << ": " << original.size()
                  << " -> " << mask.size() << "; residuo imaginario=" << residuo << std::endl;
    }
}
}
int main(int argc, char** argv) {
    try {
        const auto a = raiz(argc, argv) / "05_filtros_frecuencia";
        const auto masks = archivos(a / "raw/mascaras");
        const auto entradas = archivos(a / "raw/imagenes");
        int fallos = 0;
        for (const auto& p : entradas) {
            try { procesar(p, a, masks); }
            catch (const std::exception& e) { ++fallos; std::cerr << p << ": " << e.what() << '\n'; }
        }
        std::cout << "Entradas: " << entradas.size() << "; mascaras: " << masks.size()
                  << "; fallos: " << fallos << "; PNG escritos: " << salidas << std::endl;
        return fallos ? 1 : 0;
    } catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
