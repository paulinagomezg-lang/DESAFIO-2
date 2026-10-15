#include <iostream>
#include "Usuario.h"
#include "Recursos.h"
using namespace std;

int main() {
    Fecha f(1, 1, 2000);
    const char* nombres[] = {"zoe", "ana", "mario", "beto", "ana", "carla", "luis"};
    for (int i = 0; i < 7; i++) {
        Usuario::agregarUsuario(new Usuario(nombres[i], "123", "x", f, "Med", "Col", f));
    }

    Usuario::ordenar();
    cout << "Ordenados:";
    for (int i = 0; i < Usuario::getCantidad(); i++) cout << " " << Usuario::obtener(i)->getNombreUsuario();
    cout << "\n";

    int repetidos = Usuario::eliminarDuplicados();
    cout << "Repetidos eliminados: " << repetidos << "\nSin repetidos:";
    for (int i = 0; i < Usuario::getCantidad(); i++)
        cout << " [" << Usuario::obtener(i)->getIndice() << "]" << Usuario::obtener(i)->getNombreUsuario();
    cout << "\n";

    cout << "buscar(\"mario\") = " << Usuario::buscar("mario") << "\n";
    cout << "buscar(\"ana\")   = " << Usuario::buscar("ana") << "\n";
    cout << "buscar(\"zoe\")   = " << Usuario::buscar("zoe") << "\n";
    cout << "buscar(\"pedro\") = " << Usuario::buscar("pedro") << "\n";
    Usuario::liberarTodos();
    return 0;
}