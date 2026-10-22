#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
namespace fs = std::filesystem;
namespace {
void exigir(bool condicion, const std::string& mensaje) {
    if (!condicion) throw std::runtime_error(mensaje);
}
cv::Mat leer(const fs::path& p) {
    auto m = cv::imread(p.string(), cv::IMREAD_UNCHANGED);
    exigir(!m.empty(), "Ilegible: " + p.string());
    return m;
}
cv::Mat gris(const cv::Mat& m) {
    if (m.channels() == 1) return m;
    cv::Mat g;
    cv::cvtColor(m, g, m.channels() == 4 ? cv::COLOR_BGRA2GRAY : cv::COLOR_BGR2GRAY);
    return g;
}
void coincide(const cv::Mat& esperado, const fs::path& p, double tolerancia = 0) {
    auto actual = leer(p);
    exigir(actual.size() == esperado.size() && actual.type() == esperado.type(), "Tipo/tamano: " + p.string());
    exigir(cv::norm(esperado, actual, cv::NORM_INF) <= tolerancia, "Diferencia: " + p.string());
}
std::vector<fs::path> imagenes(const fs::path& dir) {
    std::vector<fs::path> paths;
    for (const auto& e : fs::directory_iterator(dir))
        if (e.is_regular_file() && e.path().filename().string()[0] != '.') paths.push_back(e.path());
    return paths;
}
void verificarLectura(const fs::path& actividad) {
    for (const auto& p : imagenes(actividad / "raw")) {
        const auto m = leer(p);
        const auto out = actividad / "resultados" / p.stem();
        exigir(m.type() == CV_8UC3, "Las cuatro fotos deben contener color");
        coincide(m, out / "lectura/original.png");
        coincide(gris(m), out / "conversiones/gris.png");
        std::vector<cv::Mat> c; cv::split(m, c);
        const std::vector<std::string> nombres = {"B", "G", "R"};
        for (int i = 0; i < 3; ++i) {
            coincide(c[i], out / "canales" / (nombres[i]+"_intensidad.png"));
            auto aislado = leer(out / "canales" / (nombres[i]+"_color.png"));
            std::vector<cv::Mat> componentes; cv::split(aislado, componentes);
            exigir(componentes.size() == 3, "Canal visual sin tres componentes");
            for (int j = 0; j < 3; ++j)
                exigir(j == i ? cv::norm(componentes[j], c[j], cv::NORM_INF) == 0 : cv::countNonZero(componentes[j]) == 0,
                    "Orden BGR incorrecto");
        }
    }
    std::cout << "Lectura: copias exactas, gris y canales BGR verificados\n";
}
void verificarFourier(const fs::path& actividad) {
    double peor = 0;
    for (const auto& p : imagenes(actividad / "raw/imagenes")) {
        const auto g = gris(leer(p));
        const auto out = actividad / "resultados" / p.stem();
        coincide(g, out / "fourier/entrada_gris.png");
        cv::Mat f, dft; g.convertTo(f, CV_32F); cv::dft(f, dft, cv::DFT_COMPLEX_OUTPUT);
        cv::Mat identidad; cv::idft(dft, identidad, cv::DFT_REAL_OUTPUT | cv::DFT_SCALE);
        exigir(cv::norm(f, identidad, cv::NORM_INF) < .001, "DFT/inversa no recupera la entrada");
        std::vector<cv::Mat> planos; cv::split(dft, planos);
        cv::Mat magnitud; cv::magnitude(planos[0], planos[1], magnitud); cv::log(magnitud+1, magnitud);
        double maxLog; cv::minMaxLoc(magnitud, nullptr, &maxLog);
        for (const auto& maskPath : imagenes(actividad / "raw/mascaras")) {
            cv::Mat mask; cv::resize(gris(leer(maskPath)), mask, g.size(), 0, 0, cv::INTER_NEAREST);
            double maxMask; cv::minMaxLoc(mask, nullptr, &maxMask);
            const double divisor = maxMask > 1 ? 255. : 1.;
            auto ref = dft.clone();
            cv::Mat efectiva(g.size(), CV_8U);
            // Referencia directa en coordenadas SIN centrar. No reutiliza el shift del programa.
            for (int y = 0; y < g.rows; ++y) for (int x = 0; x < g.cols; ++x) {
                const int yp = (y+g.rows/2)%g.rows, xp = (x+g.cols/2)%g.cols;
                const int yn = ((g.rows-y)%g.rows+g.rows/2)%g.rows;
                const int xn = ((g.cols-x)%g.cols+g.cols/2)%g.cols;
                const float h = float((mask.at<uchar>(yp,xp)+mask.at<uchar>(yn,xn))/(2*divisor));
                ref.at<cv::Vec2f>(y,x) *= h;
                efectiva.at<uchar>(yp,xp) = cv::saturate_cast<uchar>(255*h);
            }
            const auto dir = out / "mascaras" / maskPath.stem();
            coincide(efectiva, dir / "mascara_aplicada.png", 1);
            cv::Mat inversa, esperado; cv::idft(ref, inversa, cv::DFT_REAL_OUTPUT | cv::DFT_SCALE);
            inversa.convertTo(esperado, CV_8U);
            const auto actual = leer(dir / "reconstruida.png");
            coincide(esperado, dir / "reconstruida.png", 1);
            peor = std::max(peor, cv::norm(esperado, actual, cv::NORM_INF));
            exigir(cv::norm(actual, cv::NORM_INF) > 0, "Reconstruccion negra");
            // Verifica tambien que ambos espectros comparten la misma escala logaritmica.
            cv::split(ref, planos); cv::magnitude(planos[0], planos[1], magnitud); cv::log(magnitud+1, magnitud);
            cv::Mat espectro(g.size(), CV_8U);
            for (int y = 0; y < g.rows; ++y) for (int x = 0; x < g.cols; ++x)
                espectro.at<uchar>((y+g.rows/2)%g.rows,(x+g.cols/2)%g.cols) =
                    cv::saturate_cast<uchar>(magnitud.at<float>(y,x)*255/maxLog);
            coincide(espectro, dir / "espectro_filtrado.png", 1);
        }
    }
    std::cout << "Fourier: 16 mascaras y reconstrucciones, error maximo=" << peor << "/255\n";
}
void verificarUmbrales(const fs::path& actividad) {
    for (const auto& p : imagenes(actividad / "raw")) {
        auto m = leer(p), g = gris(m);
        const auto out = actividad / "resultados" / p.stem();
        coincide(g, out / "entrada/gris.png");
        if (p.stem() != "Mango") {
            exigir(leer(out / "entrada/original.png").channels() == 1, "Entrada gris tratada como BGR");
            exigir(!fs::exists(out / "histogramas/BGR_normalizado.png"), "Histograma BGR obsoleto para gris");
            exigir(!fs::exists(out / "ecualizacion/color_luminancia.png"), "Ecualizacion color obsoleta");
        } else {
            coincide(m, out / "entrada/original.png");
            for (const auto& nombre : {"B", "G", "R", "BGR"})
                exigir(leer(out / "histogramas" / (std::string(nombre)+"_normalizado.png")).type() == CV_8UC3,
                    "Falta histograma color de Mango");
            cv::Mat ycrcb, ec; cv::cvtColor(m, ycrcb, cv::COLOR_BGR2YCrCb);
            std::vector<cv::Mat> c; cv::split(ycrcb,c); cv::equalizeHist(c[0],c[0]);
            cv::merge(c,ycrcb); cv::cvtColor(ycrcb,ec,cv::COLOR_YCrCb2BGR);
            coincide(ec, out / "ecualizacion/color_luminancia.png");
        }
        cv::Mat r; cv::equalizeHist(g,r); coincide(r,out / "ecualizacion/gris.png");
        cv::threshold(g,r,0,255,cv::THRESH_BINARY | cv::THRESH_OTSU); coincide(r,out / "umbrales/otsu.png");
        for (const auto& par : std::vector<std::pair<int,std::string>>{
            {cv::THRESH_BINARY,"binaria_127"},{cv::THRESH_BINARY_INV,"binaria_inversa_127"},
            {cv::THRESH_TRUNC,"truncamiento_127"},{cv::THRESH_TOZERO,"a_cero_127"},
            {cv::THRESH_TOZERO_INV,"a_cero_inversa_127"}}) {
            cv::threshold(g,r,127,255,par.first); coincide(r,out / "umbrales" / (par.second+".png"));
        }
        for (const auto& par : std::vector<std::pair<int,std::string>>{
            {cv::ADAPTIVE_THRESH_MEAN_C,"adaptativa_media"},{cv::ADAPTIVE_THRESH_GAUSSIAN_C,"adaptativa_gaussiana"}}) {
            cv::adaptiveThreshold(g,r,255,par.first,cv::THRESH_BINARY,31,5);
            coincide(r,out / "umbrales" / (par.second+".png"));
        }
    }
    std::cout << "Histogramas: clasificacion, ecualizacion y ocho umbralizaciones por imagen verificadas\n";
}
void verificarMorfologia(const fs::path& actividad) {
    for (const std::string nombre : {"cat","huella","letra"}) {
        const auto dir = actividad / "resultados" / nombre / "binaria";
        const auto bin = leer(dir / "entrada/otsu_primer_plano_blanco.png");
        exigir(bin.type() == CV_8UC1 && cv::countNonZero((bin != 0) & (bin != 255)) == 0, "Entrada no binaria");
        exigir(cv::countNonZero(bin) > 0 && cv::countNonZero(bin) < int(bin.total()), "Entrada uniforme");
        for (const auto& forma : std::vector<std::pair<int,std::string>>{
            {cv::MORPH_RECT,"rectangular"},{cv::MORPH_ELLIPSE,"eliptico"},{cv::MORPH_CROSS,"cruz"}}) {
            const auto e = cv::getStructuringElement(forma.first,{3,3});
            const auto archivo = forma.second+"_3x3.png";
            const auto erosion = leer(dir / "erosion" / archivo), dilatacion = leer(dir / "dilatacion" / archivo);
            const auto apertura = leer(dir / "apertura" / archivo), cierre = leer(dir / "cierre" / archivo);
            exigir(cv::countNonZero(erosion > bin) == 0 && cv::countNonZero(dilatacion < bin) == 0, "Inclusion erosion/dilatacion");
            exigir(cv::countNonZero(apertura > bin) == 0 && cv::countNonZero(cierre < bin) == 0, "Inclusion apertura/cierre");
            cv::Mat segunda; cv::morphologyEx(apertura,segunda,cv::MORPH_OPEN,e);
            exigir(cv::norm(apertura,segunda,cv::NORM_INF) == 0, "Apertura no idempotente");
            cv::morphologyEx(cierre,segunda,cv::MORPH_CLOSE,e);
            exigir(cv::norm(cierre,segunda,cv::NORM_INF) == 0, "Cierre no idempotente");
            for (const auto& op : {"erosion","dilatacion","apertura","cierre"}) {
                const auto r = leer(dir / op / archivo);
                exigir(r.type() == CV_8UC1 && cv::countNonZero((r != 0) & (r != 255)) == 0, "Salida no binaria");
            }
        }
    }
    std::cout << "Morfologia: binariedad, inclusion e idempotencia para las tres formas verificadas\n";
}
} // namespace
int main(int argc, char** argv) {
    try {
        exigir(argc == 2, "Uso: verificar_resultados <raiz_Playground>");
        const fs::path root = fs::absolute(argv[1]);
        int total = 0;
        for (const auto& par : std::vector<std::pair<std::string,int>>{
            {"01_lectura_imagenes",32},{"04_filtros_espaciales",56},{"05_filtros_frecuencia",72},
            {"06_histogramas_umbralizacion",44},{"07_morfologia_matematica",78}}) {
            int n = 0;
            for (const auto& e : fs::recursive_directory_iterator(root / par.first / "resultados"))
                if (e.is_regular_file() && e.path().extension() == ".png") {
                    auto m = leer(e.path());
                    exigir(m.depth() == CV_8U && (m.channels() == 1 || m.channels() == 3), "PNG no visualizable");
                    ++n;
                }
            exigir(n == par.second, "Cantidad inesperada: " + par.first + " " + std::to_string(n));
            total += n;
        }
        verificarLectura(root / "01_lectura_imagenes");
        verificarFourier(root / "05_filtros_frecuencia");
        verificarUmbrales(root / "06_histogramas_umbralizacion");
        verificarMorfologia(root / "07_morfologia_matematica");
        std::cout << "OK: " << total << " PNG regenerados y decodificables\n";
        return 0;
    } catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; }
}
