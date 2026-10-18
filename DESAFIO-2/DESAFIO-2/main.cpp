#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Usuario.h"
#include "Persistencia.h"
#include "MenuAmistades.h"
using namespace std;

int main() {
    srand((unsigned)time(nullptr));

    Persistencia::cargarUsuarios("usuarios_prueba.txt");

    Persistencia::cargarAlbumes("albumes.txt");

    Persistencia::cargarMensajes("mensajes.txt");

    Persistencia::cargarAmistades("amistades_prueba.txt");

    Persistencia::cargarSolicitudes("solicitudes_prueba.txt");

    int usuario = MenuAmistades::iniciarSesion();

    if (usuario != -1) {

        MenuAmistades::mostrarFotosPerfil(usuario);

        MenuAmistades::administrarAmistades(usuario);

        Persistencia::guardarAmistades("amistades_prueba.txt");

        Persistencia::guardarSolicitudes("solicitudes_prueba.txt");

        Persistencia::guardarAlbumes("albumes.txt");

        Persistencia::guardarMensajes("mensajes.txt");

        cout << "Cambios guardados.\n";
    }
    Usuario::liberarTodos();
    return 0;
}