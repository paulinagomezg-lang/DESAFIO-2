#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <istream>

// Carga y guarda los 5 archivos de texto (campos separados por '|').
class Persistencia {
public:
    // ---- Parte de Pablo ----
    static bool cargarUsuarios(const char* ruta);

    static bool cargarAmistades(const char* ruta);

    static bool guardarAmistades(const char* ruta);

    static bool cargarSolicitudes(const char* ruta);

    static bool guardarSolicitudes(const char* ruta);

    // ---- Utilidades compartidas ----
    static const int MAX_LINEA = 1024;   // el mensaje (512) más los demás campos, con margen

    // Lee una línea. Devuelve false al llegar al final del archivo.
    // Si la línea no cabe en el buffer, la descarta y deja 'demasiadoLarga' en true.
    static bool leerLinea(std::istream& archivo, char* linea, bool& demasiadoLarga);

    // Reemplaza el archivo final por el temporal ya escrito por completo.
    static bool reemplazarArchivo(const char* rutaTemporal, const char* rutaFinal);
};

#endif