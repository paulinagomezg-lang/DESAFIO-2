#include "Persistencia.h"
#include "Usuario.h"
#include "Cadena.h"
#include "Recursos.h"
#include <fstream>
#include <iostream>

// Convierte texto a entero sin atoi (atoi es de C). Solo acepta dígitos.
bool Persistencia::convertirEntero(const char* texto, int& resultado) {
    resultado = 0;
    if (texto == nullptr || texto[0] == '\0') return false;
    for (int i = 0; texto[i] != '\0'; i++) {
        Recursos::sumar();
        if (texto[i] < '0' || texto[i] > '9') return false;
        if (resultado > 200000000) return false;          // evita desbordar el int
        resultado = resultado * 10 + (texto[i] - '0');
    }
    return true;
}

// albumes.txt:
//   A|usuario|nombreAlbum|privacidad          (0 publico, 1 privado, 2 amigos)
//   F|nombreFoto|fechaCaptura|tamano|likes    (fotos del ultimo album leido)
bool Persistencia::cargarAlbumes(const char* ruta) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "No se encontro " << ruta << " (se empieza sin albumes)\n";
        return false;
    }
    char linea[MAX_LINEA];
    char* campos[6];
    int numeroLinea = 0, albumesCargados = 0, fotosCargadas = 0, descartadas = 0;
    bool larga;
    Usuario* dueno = nullptr;     // dueño del álbum que se está llenando
    int albumActual = -1;         // posición de ese álbum en la lista del dueño

    while (leerLinea(archivo, linea, larga)) {
        numeroLinea++;
        Recursos::sumar();
        if (larga) { descartadas++; continue; }
        if (linea[0] == '\0') continue;

        int n = Cadena::dividir(linea, '|', campos, 6);

        if (Cadena::iguales(campos[0], "A")) {
            dueno = nullptr;                 // si el álbum es inválido, sus fotos se ignoran
            albumActual = -1;
            int privacidad;
            if (n != 4 || !convertirEntero(campos[3], privacidad)) {
                std::cout << "  albumes linea " << numeroLinea << ": formato de album incorrecto\n";
                descartadas++;
                continue;
            }
            int u = Usuario::buscar(campos[1]);
            if (u == -1) {
                std::cout << "  albumes linea " << numeroLinea << ": usuario inexistente\n";
                descartadas++;
                continue;
            }
            if (!Album::Nombre_Valido(campos[2]) || !Album::Privacidad_Valida(privacidad)) {
                std::cout << "  albumes linea " << numeroLinea << ": nombre o privacidad invalidos\n";
                descartadas++;
                continue;
            }
            dueno = Usuario::obtener(u);
            dueno->agregarAlbum(Album(campos[2], privacidad));
            albumActual = dueno->cantidadAlbumes() - 1;
            albumesCargados++;
        }
        else if (Cadena::iguales(campos[0], "F")) {
            if (dueno == nullptr) {
                std::cout << "  albumes linea " << numeroLinea << ": foto sin album valido\n";
                descartadas++;
                continue;
            }
            int tamano, likes;
            if (n != 5 || !convertirEntero(campos[3], tamano) || !convertirEntero(campos[4], likes)) {
                std::cout << "  albumes linea " << numeroLinea << ": formato de foto incorrecto\n";
                descartadas++;
                continue;
            }
            Foto nueva(campos[1], Fecha(campos[2]), tamano, likes);
            if (!dueno->getAlbum(albumActual).Agregar_Foto(nueva)) {   // valida nombre, fecha y tamaño
                std::cout << "  albumes linea " << numeroLinea << ": foto invalida\n";
                descartadas++;
                continue;
            }
            fotosCargadas++;
        }
        else {
            std::cout << "  albumes linea " << numeroLinea << ": tipo de linea desconocido\n";
            descartadas++;
        }
    }
    std::cout << "Albumes cargados: " << albumesCargados << " (" << fotosCargadas << " fotos)";
    if (descartadas > 0) std::cout << " (" << descartadas << " lineas descartadas)";
    std::cout << "\n";
    return true;
}

bool Persistencia::guardarAlbumes(const char* ruta) {
    std::ofstream archivo(ruta);
    if (!archivo.is_open()) return false;

    char fecha[Fecha::TAM_TEXTO];
    for (int u = 0; u < Usuario::getCantidad(); u++) {
        Usuario* x = Usuario::obtener(u);
        for (int a = 0; a < x->cantidadAlbumes(); a++) {
            const Album& album = x->getAlbum(a);
            archivo << "A|" << x->getNombreUsuario() << '|' << album.getNombre()
                    << '|' << album.getPrivacidad() << '\n';

            for (int f = 0; f < album.Cantidad_Fotos(); f++) {
                Recursos::sumar();
                const Foto& foto = album.getFoto(f);
                foto.getFecha_de_Captura().A_Texto(fecha);
                archivo << "F|" << foto.getNombre() << '|' << fecha << '|'
                        << foto.getTamanio() << '|' << foto.getLikes() << '\n';
            }
        }
    }
    archivo.close();
    return !archivo.fail();
}

// mensajes.txt:  emisor|receptor|fecha|leido|texto
// (leido: 0 o 1; el texto va al final para que un '|' escrito por el usuario no rompa la linea)
bool Persistencia::cargarMensajes(const char* ruta) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        std::cout << "No se encontro " << ruta << " (se empieza sin mensajes)\n";
        return false;
    }
    char linea[MAX_LINEA];
    char* campos[5];
    int numeroLinea = 0, cargados = 0, descartadas = 0;
    bool larga;

    while (leerLinea(archivo, linea, larga)) {
        numeroLinea++;
        Recursos::sumar();
        if (larga) { descartadas++; continue; }
        if (linea[0] == '\0') continue;

        // Con máximo 5 campos, el texto conserva cualquier '|' que tenga
        if (Cadena::dividir(linea, '|', campos, 5) != 5) {
            std::cout << "  mensajes linea " << numeroLinea << ": formato incorrecto\n";
            descartadas++;
            continue;
        }
        int e = Usuario::buscar(campos[0]);
        int r = Usuario::buscar(campos[1]);
        if (e == -1 || r == -1) {
            std::cout << "  mensajes linea " << numeroLinea << ": usuario inexistente\n";
            descartadas++;
            continue;
        }
        Fecha envio(campos[2]);
        bool leido = Cadena::iguales(campos[3], "1");
        bool estadoValido = leido || Cadena::iguales(campos[3], "0");

        if (!envio.esValida() || !estadoValido || !Mensaje::Texto_Valido(campos[4])) {
            std::cout << "  mensajes linea " << numeroLinea << ": datos invalidos\n";
            descartadas++;
            continue;
        }
        Usuario::obtener(r)->agregarMensaje(Mensaje(campos[0], campos[1], envio, campos[4], leido));
        cargados++;
    }
    std::cout << "Mensajes cargados: " << cargados;
    if (descartadas > 0) std::cout << " (" << descartadas << " lineas descartadas)";
    std::cout << "\n";
    return true;
}

bool Persistencia::guardarMensajes(const char* ruta) {
    std::ofstream archivo(ruta);
    if (!archivo.is_open()) return false;

    char fecha[Fecha::TAM_TEXTO];
    for (int u = 0; u < Usuario::getCantidad(); u++) {
        Usuario* x = Usuario::obtener(u);
        for (int m = 0; m < x->cantidadMensajes(); m++) {
            Recursos::sumar();
            const Mensaje& msj = x->getMensaje(m);
            msj.getFecha_de_Envio().A_Texto(fecha);
            archivo << msj.getEmisor() << '|' << msj.getReceptor() << '|' << fecha << '|'
                    << (msj.getLeidos() ? 1 : 0) << '|' << msj.getTexto() << '\n';
        }
    }
    archivo.close();
    return !archivo.fail();
}