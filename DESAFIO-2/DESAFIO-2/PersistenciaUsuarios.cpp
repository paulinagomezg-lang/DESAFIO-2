#include "Persistencia.h"
#include "Usuario.h"
#include "Cadena.h"
#include "Recursos.h"
#include <fstream>
#include <iostream>
#include <cstdio>


bool Persistencia::leerLinea(std::istream& archivo, char* linea, bool& demasiadoLarga) {
    demasiadoLarga = false;
    linea[0] = '\0';
    if (!archivo.getline(linea, MAX_LINEA)) {
        if (archivo.eof()) return linea[0] != '\0';   // fin del archivo (o última línea sin '\n')
        archivo.clear();                              // la línea no cupo en el buffer:
        archivo.ignore(1000000, '\n');                // se descarta el resto
        demasiadoLarga = true;
        return true;
    }
    Cadena::quitarFinDeLinea(linea);                  // quita el '\r' de archivos de Windows
    return true;
}

// usuarios.txt:  usuario|contrasena|correo|fechaNacimiento|ciudad|pais|fechaInscripcion
bool Persistencia::cargarUsuarios(const char* ruta) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir " << ruta << "\n";
        return false;
    }

    char linea[MAX_LINEA];
    char* campos[8];
    int numeroLinea = 0, descartadas = 0;
    bool larga;

    while (leerLinea(archivo, linea, larga)) {
        numeroLinea++;
        Recursos::sumar();
        if (larga) { descartadas++; continue; }
        if (linea[0] == '\0') continue;                       // línea vacía: se ignora

        // Se piden hasta 8 campos: si salen exactamente 7, el formato es correcto
        if (Cadena::dividir(linea, '|', campos, 8) != 7) {
            std::cout << "  linea " << numeroLinea << ": numero de campos incorrecto\n";
            descartadas++;
            continue;
        }
        if (campos[0][0] == '\0') {
            std::cout << "  linea " << numeroLinea << ": nombre de usuario vacio\n";
            descartadas++;
            continue;
        }
        Fecha nacimiento(campos[3]);
        Fecha inscripcion(campos[6]);
        if (!nacimiento.esValida() || !inscripcion.esValida()) {
            std::cout << "  linea " << numeroLinea << ": fecha invalida\n";
            descartadas++;
            continue;
        }
        Usuario::agregarUsuario(new Usuario(campos[0], campos[1], campos[2], nacimiento,
                                            campos[4], campos[5], inscripcion));
    }

    // Orden obligatorio: ordenar -> quitar repetidos -> crear la matriz
    Usuario::ordenar();
    int repetidos = Usuario::eliminarDuplicados();
    Usuario::prepararMatriz();

    std::cout << "Usuarios cargados: " << Usuario::getCantidad();
    if (descartadas > 0) std::cout << " (" << descartadas << " lineas descartadas)";
    if (repetidos > 0) std::cout << " (" << repetidos << " nombres repetidos ignorados)";
    std::cout << "\n";
    return Usuario::getCantidad() > 0;
}

bool Persistencia::reemplazarArchivo(const char* rutaTemporal, const char* rutaFinal) {
    std::remove(rutaFinal);                          // en Windows, rename falla si el destino existe
    return std::rename(rutaTemporal, rutaFinal) == 0;
}

// amistades.txt:  usuarioA|usuarioB   (cada pareja una sola vez)
bool Persistencia::cargarAmistades(const char* ruta) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "No se encontro " << ruta << " (se empieza sin amistades)\n";
        return false;
    }
    char linea[MAX_LINEA];
    char* campos[3];
    int numeroLinea = 0, cargadas = 0, descartadas = 0;
    bool larga;

    while (leerLinea(archivo, linea, larga)) {
        numeroLinea++;
        Recursos::sumar();
        if (larga) { descartadas++; continue; }
        if (linea[0] == '\0') continue;
        if (Cadena::dividir(linea, '|', campos, 3) != 2) {
            std::cout << "  amistades linea " << numeroLinea << ": formato incorrecto\n";
            descartadas++;
            continue;
        }
        int a = Usuario::buscar(campos[0]);                // búsqueda binaria: O(log n)
        int b = Usuario::buscar(campos[1]);
        if (a == -1 || b == -1) {
            std::cout << "  amistades linea " << numeroLinea << ": usuario inexistente\n";
            descartadas++;
            continue;
        }
        if (Usuario::getMatriz().sonAmigos(a, b)) {         // la pareja ya estaba
            std::cout << "  amistades linea " << numeroLinea << ": amistad repetida\n";
            descartadas++;
            continue;
        }
        if (!Usuario::getMatriz().agregarAmistad(a, b)) {  // falla si a == b
            std::cout << "  amistades linea " << numeroLinea << ": amistad consigo mismo\n";
            descartadas++;
            continue;
        }
        cargadas++;
    }
    std::cout << "Amistades cargadas: " << cargadas;
    if (descartadas > 0) std::cout << " (" << descartadas << " lineas descartadas)";
    std::cout << "\n";
    return true;
}

bool Persistencia::guardarAmistades(const char* ruta) {
    char temporal[512] = "";
    Cadena::anexar(temporal, sizeof(temporal), ruta);
    Cadena::anexar(temporal, sizeof(temporal), ".tmp");     // amistades.txt.tmp

    std::ofstream archivo(temporal);
    if (!archivo.is_open()) return false;

    int n = Usuario::getCantidad();
    for (int i = 0; i < n; i++) {
        Lista<int> amigos;
        Usuario::obtener(i)->obtenerAmigos(amigos);
        for (int k = 0; k < amigos.tamano(); k++) {
            Recursos::sumar();
            if (amigos[k] > i) {       // solo la mitad superior: cada pareja se escribe una vez
                archivo << Usuario::obtener(i)->getNombreUsuario() << '|'
                        << Usuario::obtener(amigos[k])->getNombreUsuario() << '\n';
            }
        }
    }
    archivo.close();
    if (archivo.fail()) return false;
    return reemplazarArchivo(temporal, ruta);
}

// solicitudes.txt:  emisor|receptor   (solo solicitudes pendientes)
bool Persistencia::cargarSolicitudes(const char* ruta) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "No se encontro " << ruta << " (se empieza sin solicitudes)\n";
        return false;
    }
    char linea[MAX_LINEA];
    char* campos[3];
    int numeroLinea = 0, cargadas = 0, descartadas = 0;
    bool larga;

    while (leerLinea(archivo, linea, larga)) {
        numeroLinea++;
        Recursos::sumar();
        if (larga) { descartadas++; continue; }
        if (linea[0] == '\0') continue;
        if (Cadena::dividir(linea, '|', campos, 3) != 2) {
            std::cout << "  solicitudes linea " << numeroLinea << ": formato incorrecto\n";
            descartadas++;
            continue;
        }
        int emisor = Usuario::buscar(campos[0]);
        int receptor = Usuario::buscar(campos[1]);
        if (emisor == -1 || receptor == -1) {
            std::cout << "  solicitudes linea " << numeroLinea << ": usuario inexistente\n";
            descartadas++;
            continue;
        }
        // Se reutilizan las reglas de enviarSolicitud: no a sí mismo, no si ya son amigos,
        // no repetida y no si ya existe la solicitud en sentido contrario.
        Usuario* e = Usuario::obtener(emisor);
        Usuario* r = Usuario::obtener(receptor);
        Usuario::ResultadoSolicitud resultado = e->enviarSolicitud(*r);
        if (resultado != Usuario::SOLICITUD_ENVIADA) {
            std::cout << "  solicitudes linea " << numeroLinea << ": ";
            switch (resultado) {
            case Usuario::ES_EL_MISMO_USUARIO:  std::cout << "solicitud a si mismo\n"; break;
            case Usuario::YA_SON_AMIGOS:        std::cout << "ya son amigos\n"; break;
            case Usuario::SOLICITUD_YA_ENVIADA: std::cout << "solicitud repetida\n"; break;
            default:                            std::cout << "ya existe la solicitud contraria\n";
            }
            descartadas++;
            continue;
        }
        cargadas++;
    }
    std::cout << "Solicitudes pendientes cargadas: " << cargadas;
    if (descartadas > 0) std::cout << " (" << descartadas << " lineas descartadas)";
    std::cout << "\n";
    return true;
}

bool Persistencia::guardarSolicitudes(const char* ruta) {
    char temporal[512] = "";
    Cadena::anexar(temporal, sizeof(temporal), ruta);
    Cadena::anexar(temporal, sizeof(temporal), ".tmp");

    std::ofstream archivo(temporal);
    if (!archivo.is_open()) return false;

    int n = Usuario::getCantidad();
    for (int r = 0; r < n; r++) {
        const Usuario* receptor = Usuario::obtener(r);
        const Lista<int>& solicitudes = receptor->getSolicitudesRecibidas();
        for (int k = 0; k < solicitudes.tamano(); k++) {
            Recursos::sumar();
            archivo << Usuario::obtener(solicitudes[k])->getNombreUsuario() << '|'
                    << receptor->getNombreUsuario() << '\n';
        }
    }
    archivo.close();
    if (archivo.fail()) return false;
    return reemplazarArchivo(temporal, ruta);
}