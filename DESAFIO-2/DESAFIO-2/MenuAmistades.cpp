#include "MenuAmistades.h"
#include "Usuario.h"
#include "Cadena.h"
#include "Recursos.h"
#include <iostream>

using std::cout;
using std::cin;

// ===================== Utilidades =====================

// Reporte de recursos al final de cada funcionalidad (funcionalidad VII).
// Cuando la persona B tenga Recursos::mostrarReporte (con la memoria), se cambia SOLO esta línea.
void MenuAmistades::reportar(const char* funcionalidad) {

    cout << "[Recursos] " << funcionalidad << " | iteraciones: " << Recursos::obtenerIteraciones() << "\n";
}

void MenuAmistades::leerLinea(char* destino, int tamano) {

    destino[0] = '\0';

    if (!cin.getline(destino, tamano)) {

        if (cin.eof()) {                     // se acabó la entrada: se responde "0" para salir

            destino[0] = '0';

            destino[1] = '\0';

            return;
        }
        cin.clear();                         // la línea era más larga que el buffer

        cin.ignore(100000, '\n');
    }
    Cadena::quitarFinDeLinea(destino);
}

int MenuAmistades::leerOpcion() {

    char texto[16];

    leerLinea(texto, sizeof(texto));

    if (texto[0] == '\0') return -1;

    int valor = 0;

    for (int i = 0; texto[i] != '\0'; i++) {

        if (texto[i] < '0' || texto[i] > '9') return -1;   // letras: opción inválida

        valor = valor * 10 + (texto[i] - '0');

        if (valor > 100000) return -1;
    }
    return valor;
}

// ===================== II. Inicio de sesión =====================

int MenuAmistades::iniciarSesion() {

    char nombre[128];

    char clave[128];

    for (int intento = 1; intento <= INTENTOS_LOGIN; intento++) {

        cout << "\n=== Iniciar sesion (escribe 0 para salir) ===\n";

        cout << "Usuario: ";

        leerLinea(nombre, sizeof(nombre));

        if (Cadena::iguales(nombre, "0")) return -1;

        cout << "Contrasena: ";

        leerLinea(clave, sizeof(clave));

        Recursos::reiniciar();

        int indice = Usuario::buscar(nombre);                     // búsqueda binaria

        bool correcto = indice != -1 && Usuario::obtener(indice)->verificarContrasena(clave);

        if (correcto) {

            cout << "\nBienvenido(a), " << *Usuario::obtener(indice) << "\n";

            reportar("Inicio de sesion");

            return indice;
        }
        // No se dice si falló el usuario o la contraseña, para no revelar qué cuentas existen
        cout << "Usuario o contrasena incorrectos. Intentos restantes: "
             << (INTENTOS_LOGIN - intento) << "\n";
    }
    cout << "Se agotaron los intentos.\n";
    return -1;
}

void MenuAmistades::mostrarFotosPerfil(int usuario) {

    Usuario* u = Usuario::obtener(usuario);

    if (u == nullptr) return;

    char opcion[16];
    do {
        Recursos::reiniciar();

        int elegidos[FOTOS_A_MOSTRAR];

        int cuantos = u->elegirAmigosPonderados(elegidos, FOTOS_A_MOSTRAR);

        cout << "\n--- Fotos de perfil de tus amigos ---\n";

        if (cuantos == 0) {

            cout << "Aun no tienes amigos. Busca a alguien en Administrar amistades.\n";
            reportar("Fotos de perfil ponderadas");
            return;
        }
        char ruta[512];

        for (int i = 0; i < cuantos; i++) {

            Usuario* amigo = Usuario::obtener(elegidos[i]);

            amigo->rutaFotoPerfil(ruta, sizeof(ruta));

            cout << "  " << (i + 1) << ". " << amigo->getNombreUsuario() << "  ->  " << ruta << "\n";
        }
        reportar("Fotos de perfil ponderadas");

        cout << "Escribe N para ver otras fotos o Enter para continuar: ";

        leerLinea(opcion, sizeof(opcion));

    } while (opcion[0] == 'N' || opcion[0] == 'n');
}

// ===================== III. Administrar amistades =====================

void MenuAmistades::administrarAmistades(int usuario) {
    int opcion;
    do {
        cout << "\n 8>--- Administrar amistades ---<8 \n"
             << "1. Ver mis amigos\n"
             << "2. Enviar solicitud de amistad\n"
             << "3. Ver y aceptar solicitudes recibidas\n"
             << "0. Volver\n"
             << "Opcion: ";
        opcion = leerOpcion();
        switch (opcion) {
        case 1: verAmigos(usuario); break;
        case 2: enviarSolicitud(usuario); break;
        case 3: gestionarSolicitudes(usuario); break;
        case 0: break;
        default: cout << "Opcion no valida.\n";
        }
    } while (opcion != 0);
}

void MenuAmistades::verAmigos(int usuario) {

    Recursos::reiniciar();

    Lista<int> amigos;

    Usuario::obtener(usuario)->obtenerAmigos(amigos);

    cout << "\nTienes " << amigos.tamano() << " amigo(s):\n";

    for (int i = 0; i < amigos.tamano(); i++) {

        Recursos::sumar();

        cout << "  - " << Usuario::obtener(amigos[i])->getNombreUsuario() << "\n";
    }
    reportar("Ver amigos");
}

void MenuAmistades::enviarSolicitud(int usuario) {

    char nombre[128];

    cout << "Nombre de usuario al que quieres enviar la solicitud: ";

    leerLinea(nombre, sizeof(nombre));

    Recursos::reiniciar();

    int receptor = Usuario::buscar(nombre);

    if (receptor == -1) {

        cout << "No existe el usuario \"" << nombre << "\".\n";

        reportar("Enviar solicitud");

        return;
    }
    switch (Usuario::obtener(usuario)->enviarSolicitud(*Usuario::obtener(receptor))) {

    case Usuario::SOLICITUD_ENVIADA:

        cout << "Solicitud enviada a " << nombre << ".\n"; break;

    case Usuario::ES_EL_MISMO_USUARIO:

        cout << "No puedes enviarte una solicitud a ti mismo.\n"; break;

    case Usuario::YA_SON_AMIGOS:

        cout << "Ya eres amigo(a) de " << nombre << ".\n"; break;

    case Usuario::SOLICITUD_YA_ENVIADA:

        cout << "Ya le habias enviado una solicitud a " << nombre << ".\n"; break;

    case Usuario::EL_OTRO_YA_TE_ENVIO:

        cout << nombre << " ya te envio una solicitud. Aceptala en la opcion 3.\n"; break;
    }
    reportar("Enviar solicitud");
}

void MenuAmistades::gestionarSolicitudes(int usuario) {

    Usuario* u = Usuario::obtener(usuario);

    while (true) {

        const Lista<int>& solicitudes = u->getSolicitudesRecibidas();

        if (solicitudes.vacia()) {

            cout << "\nNo tienes solicitudes pendientes.\n";
            return;
        }
        cout << "\nSolicitudes recibidas:\n";

        for (int i = 0; i < solicitudes.tamano(); i++) {

            cout << "  " << (i + 1) << ". " << Usuario::obtener(solicitudes[i])->getNombreUsuario() << "\n";
        }
        cout << "Numero de la solicitud que quieres aceptar (0 para volver): ";

        int opcion = leerOpcion();

        if (opcion == 0) return;

        if (opcion < 1 || opcion > solicitudes.tamano()) {

            cout << "Numero no valido.\n";
            continue;
        }

        Recursos::reiniciar();

        int emisor = solicitudes[opcion - 1];      // se guarda antes de que la lista cambie

        if (u->aceptarSolicitud(opcion - 1)) {

            cout << "Ahora eres amigo(a) de " << Usuario::obtener(emisor)->getNombreUsuario() << ".\n";
        }
        reportar("Aceptar solicitud");
    }
}