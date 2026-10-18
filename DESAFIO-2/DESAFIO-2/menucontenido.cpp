#include "MenuContenido.h"
#include "Recursos.h"
#include <iostream>
using namespace std;

// ================== FUNCIONES DE APOYO ==================

void MenuContenido::leerTexto(const char* pregunta, char* destino, int tamano) {
    cout << pregunta;
    cin >> ws;                       // salta el salto de línea que dejó un cin >> anterior
    cin.getline(destino, tamano);
    if (cin.fail()) {                // texto más largo que el espacio: se descarta el resto
        cin.clear();
        cin.ignore(1000000, '\n');
    }
}

Fecha MenuContenido::leerFecha(const char* pregunta) {
    char texto[20];
    leerTexto(pregunta, texto, sizeof(texto));
    return Fecha(texto);
}

void MenuContenido::reportarRecursos() {
    long iteraciones = Recursos::obtenerIteraciones();   // se toma ANTES de medir la memoria
    long memoria = Usuario::memoriaTotal();
    cout << "\n[Recursos] Iteraciones: " << iteraciones
         << " | Memoria en uso: " << memoria << " bytes\n";
}

// ============ IV. VISITAR PERFIL DE UN AMIGO ============

void MenuContenido::visitarPerfil(Usuario& actual) {
    Recursos::reiniciar();

    char nombre[100];
    leerTexto("Nombre de usuario del amigo: ", nombre, sizeof(nombre));
    int posicion = Usuario::buscar(nombre);
    if (posicion == -1) {
        cout << "No existe ese usuario.\n";
        reportarRecursos();
        return;
    }
    Usuario* amigo = Usuario::obtener(posicion);
    if (!actual.esAmigoDe(*amigo)) {
        cout << "Solo puedes visitar el perfil de tus amigos.\n";
        reportarRecursos();
        return;
    }

    // 1. Listar los álbumes visibles para un amigo: públicos y de amigos
    Lista<int> visibles;                 // posiciones reales de esos álbumes
    cout << "\nAlbumes de " << amigo->getNombreUsuario() << ":\n";
    for (int a = 0; a < amigo->cantidadAlbumes(); a++) {
        Recursos::sumar();
        if (amigo->getAlbum(a).Puede_Ver(false, true)) {
            visibles.agregar(a);
            cout << "  " << visibles.tamano() << ". " << amigo->getAlbum(a) << "\n";
        }
    }
    if (visibles.vacia()) {
        cout << "No tiene albumes visibles.\n";
        reportarRecursos();
        return;
    }

    // 2. Elegir un álbum
    int eleccion = 0;
    cout << "Elige un album (1-" << visibles.tamano() << "): ";
    cin >> eleccion;
    if (cin.fail() || eleccion < 1 || eleccion > visibles.tamano()) {
        cin.clear();
        cin.ignore(1000000, '\n');
        cout << "Opcion invalida.\n";
        reportarRecursos();
        return;
    }
    Album& album = amigo->getAlbum(visibles[eleccion - 1]);
    if (album.Cantidad_Fotos() == 0) {
        cout << "El album no tiene fotos.\n";
        reportarRecursos();
        return;
    }

    // 3. Carrusel circular
    int fotoActual = 0;
    char opcion = ' ';
    while (opcion != 'X') {
        Recursos::sumar();
        Foto& foto = album.getFoto(fotoActual);

        cout << "\nFoto " << (fotoActual + 1) << " de " << album.Cantidad_Fotos() << ": ";
        album.Mostrar_Ruta_Foto(cout, fotoActual, amigo->getNombreUsuario());
        cout << "\n  " << foto << "\n";
        cout << "[S] Siguiente  [P] Previa  [L] Dar like  [Q] Quitar like  [X] Salir: ";

        cin >> opcion;
        if (!cin) break;
        if (opcion >= 'a' && opcion <= 'z') opcion = opcion - 'a' + 'A';   // acepta minúsculas

        if      (opcion == 'S') fotoActual = album.Siguiente(fotoActual);
        else if (opcion == 'P') fotoActual = album.Anterior(fotoActual);
        else if (opcion == 'L') foto.Dar_Like();
        else if (opcion == 'Q') foto.Quitar_Like();
        else if (opcion != 'X') cout << "Opcion invalida.\n";
    }
    reportarRecursos();
}

// =============== VI. ADMINISTRAR MENSAJES ===============

void MenuContenido::administrarMensajes(Usuario& actual) {
    char opcion = ' ';
    while (opcion != '4') {
        cout << "\n--- Mensajes ---\n"
             << "1. Enviar mensaje\n"
             << "2. Leer mensajes nuevos\n"
             << "3. Buscar mensajes por rango de fechas\n"
             << "4. Volver\n"
             << "Opcion: ";
        cin >> opcion;
        if (!cin) { cin.clear(); cin.ignore(1000000, '\n'); opcion = ' '; continue; }

        switch (opcion) {
        case '1': enviarMensaje(actual);  break;
        case '2': leerNuevos(actual);     break;
        case '3': buscarPorRango(actual); break;
        case '4': break;
        default:  cout << "Opcion invalida.\n";
        }
    }
}

// a. Enviar mensaje a un amigo
void MenuContenido::enviarMensaje(Usuario& actual) {
    Recursos::reiniciar();

    char nombre[100];
    leerTexto("Nombre del amigo: ", nombre, sizeof(nombre));
    int posicion = Usuario::buscar(nombre);
    if (posicion == -1 || !actual.esAmigoDe(*Usuario::obtener(posicion))) {
        cout << "Solo puedes enviar mensajes a tus amigos.\n";
        reportarRecursos();
        return;
    }

    char texto[Mensaje::TEXTO_MAX + 100];     // espacio extra para detectar textos largos
    leerTexto("Mensaje (maximo 512 caracteres): ", texto, sizeof(texto));
    if (!Mensaje::Texto_Valido(texto)) {
        cout << "El mensaje no puede estar vacio ni superar 512 caracteres.\n";
        reportarRecursos();
        return;
    }

    Fecha hoy = leerFecha("Fecha de hoy (DD/MM/AAAA): ");
    if (!hoy.esValida()) {
        cout << "Fecha invalida.\n";
        reportarRecursos();
        return;
    }

    Usuario* receptor = Usuario::obtener(posicion);
    receptor->agregarMensaje(Mensaje(actual.getNombreUsuario(), receptor->getNombreUsuario(),
                                     hoy, texto, false));
    cout << "Mensaje enviado a " << receptor->getNombreUsuario() << ".\n";
    reportarRecursos();
}

// b. Leer los mensajes nuevos (ya están ordenados por fecha) y marcarlos como leídos
void MenuContenido::leerNuevos(Usuario& actual) {
    Recursos::reiniciar();

    int nuevos = 0;
    for (int m = 0; m < actual.cantidadMensajes(); m++) {
        Recursos::sumar();
        Mensaje& msj = actual.getMensaje(m);
        if (!msj.getLeidos()) {
            cout << msj << "\n";
            msj.Marcar_Leido();
            nuevos++;
        }
    }
    if (nuevos == 0) cout << "No tienes mensajes nuevos.\n";
    reportarRecursos();
}

// c. Buscar mensajes recibidos en un rango de fechas (ambas inclusive)
void MenuContenido::buscarPorRango(Usuario& actual) {
    Recursos::reiniciar();

    Fecha inicio = leerFecha("Fecha de inicio (DD/MM/AAAA): ");
    Fecha fin    = leerFecha("Fecha de fin (DD/MM/AAAA): ");
    if (!inicio.esValida() || !fin.esValida() || fin < inicio) {
        cout << "Rango de fechas invalido.\n";
        reportarRecursos();
        return;
    }

    int encontrados = 0;
    for (int m = 0; m < actual.cantidadMensajes(); m++) {
        Recursos::sumar();
        const Mensaje& msj = actual.getMensaje(m);
        if (fin < msj.getFecha_de_Envio()) break;   // están ordenados: los demás son posteriores
        if (msj.Esta_en_Rango(inicio, fin)) {
            cout << msj << "\n";
            encontrados++;
        }
    }
    if (encontrados == 0) cout << "No hay mensajes en ese rango.\n";
    else cout << encontrados << " mensaje(s) encontrados.\n";
    reportarRecursos();
}