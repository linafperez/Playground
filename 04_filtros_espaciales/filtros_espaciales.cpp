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
cv::Mat magnitudVisible(const cv::Mat& m) {
    cv::Mat a = cv::abs(m), r;
    cv::normalize(a, r, 0, 255, cv::NORM_MINMAX, CV_8U);
    return r;
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
    // Suavizado y enfoque conservan el numero de canales de la entrada.
    cv::Mat b = m.channels() == 1 ? ocho(m) : color(m), g = gris(m), r, x, y, mag;
    cv::blur(b, r, cv::Size(3, 3)); guardar(out / "suavizado/caja_3x3.png", r);
    cv::GaussianBlur(b, r, cv::Size(5, 5), 1.2); guardar(out / "suavizado/gaussiano_5x5.png", r);
    cv::medianBlur(b, r, 5); guardar(out / "suavizado/mediana_5x5.png", r);
    cv::Mat e = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::dilate(b, r, e); guardar(out / "suavizado/maximo_5x5.png", r);
    cv::erode(b, r, e); guardar(out / "suavizado/minimo_5x5.png", r);
    cv::Mat kernel = (cv::Mat_<float>(3, 3) << 0,-1,0,-1,5,-1,0,-1,0);
    cv::filter2D(b, r, -1, kernel); guardar(out / "enfoque/sharpening.png", r);
    cv::Mat suave;
    cv::GaussianBlur(b, suave, cv::Size(5, 5), 1.2);
    cv::addWeighted(b, 1.6, suave, -.6, 0, r); guardar(out / "enfoque/unsharp_mask.png", r);
    cv::Laplacian(g, r, CV_32F, 3); guardar(out / "enfoque/laplaciano.png", magnitudVisible(r));
    cv::Sobel(g, x, CV_32F, 1, 0, 3); guardar(out / "bordes/sobel_x.png", magnitudVisible(x));
    cv::Sobel(g, y, CV_32F, 0, 1, 3); guardar(out / "bordes/sobel_y.png", magnitudVisible(y));
    cv::magnitude(x, y, mag); guardar(out / "bordes/sobel_magnitud.png", magnitudVisible(mag));
    cv::Mat px = (cv::Mat_<float>(3, 3) << -1,0,1,-1,0,1,-1,0,1);
    cv::Mat py = px.t();
    cv::filter2D(g, x, CV_32F, px); cv::filter2D(g, y, CV_32F, py);
    cv::magnitude(x, y, mag); guardar(out / "bordes/prewitt.png", magnitudVisible(mag));
    cv::Mat rx = (cv::Mat_<float>(2, 2) << 1,0,0,-1);
    cv::Mat ry = (cv::Mat_<float>(2, 2) << 0,1,-1,0);
    cv::filter2D(g, x, CV_32F, rx, cv::Point(0, 0));
    cv::filter2D(g, y, CV_32F, ry, cv::Point(0, 0));
    cv::magnitude(x, y, mag); guardar(out / "bordes/roberts.png", magnitudVisible(mag));
    cv::GaussianBlur(g, suave, cv::Size(5, 5), 1.2);
    // Los umbrales fijos 60/150 dejaban solo un 0.44% de bordes en gato.
    // Ajusta la histeresis a la distribucion real del gradiente suavizado.
    cv::Sobel(suave, x, CV_32F, 1, 0, 3);
    cv::Sobel(suave, y, CV_32F, 0, 1, 3);
    cv::magnitude(x, y, mag);
    std::vector<float> gradientes;
    for (int fila = 0; fila < mag.rows; ++fila) {
        const auto* valores = mag.ptr<float>(fila);
        for (int col = 0; col < mag.cols; ++col)
            if (valores[col] > 0) gradientes.push_back(valores[col]);
    }
    double alto = 1.;
    if (!gradientes.empty()) {
        auto percentil = gradientes.begin() + gradientes.size()*9/10;
        std::nth_element(gradientes.begin(), percentil, gradientes.end());
        alto = std::max(1., double(*percentil));
    }
    cv::Canny(suave, r, .4*alto, alto, 3, true);
    guardar(out / "bordes/canny.png", r);
    std::cout << out.filename() << ": Canny bajo=" << .4*alto << "; alto=" << alto << std::endl;
}
}
int main(int argc, char** argv) {
    try { return lote(raiz(argc, argv) / "04_filtros_espaciales", procesar); }
    catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
