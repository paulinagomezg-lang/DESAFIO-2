#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Usuario.h"
using namespace std;

int main() {
    srand((unsigned)time(nullptr));          // semilla: resultados distintos en cada ejecución

    // 12 usuarios: ua, ub, uc, ... ul
    Fecha f(1, 1, 2000);
    char nombre[3] = "u?";
    for (int i = 0; i < 12; i++) {
        nombre[1] = (char)('a' + i);
        Usuario::agregarUsuario(new Usuario(nombre, "1", "x", f, "x", "x", f));
    }
    Usuario::ordenar();
    Usuario::prepararMatriz();
    MatrizAmistad& m = Usuario::getMatriz();

    // ua (0) tiene 9 amigos. Con ub (1) comparte 6 amigos, con uc (2) comparte 1, con ud (3) ninguno.
    m.agregarAmistad(0, 1); m.agregarAmistad(0, 2); m.agregarAmistad(0, 3);
    for (int k = 4; k <= 9; k++) { m.agregarAmistad(0, k); m.agregarAmistad(1, k); }
    m.agregarAmistad(2, 4);

    cout << "peso ub = " << Usuario::pesoAmistad(0, 1)
         << " | peso uc = " << Usuario::pesoAmistad(0, 2)
         << " | peso ud = " << Usuario::pesoAmistad(0, 3) << "\n";

    Usuario* ua = Usuario::obtener(0);
    int elegidos[3];
    int cuantos = ua->elegirAmigosPonderados(elegidos, 3);
    char ruta[256];
    cout << "Fotos elegidas para ua:\n";
    for (int i = 0; i < cuantos; i++) {
        Usuario::obtener(elegidos[i])->rutaFotoPerfil(ruta, sizeof(ruta));
        cout << "  " << ruta << "\n";
    }

    // 3000 sorteos de 1 foto: ub (peso 3.5) debe salir mucho más que ud (peso 1)
    int vecesUb = 0, vecesUd = 0;
    for (int r = 0; r < 3000; r++) {
        ua->elegirAmigosPonderados(elegidos, 1);
        if (elegidos[0] == 1) vecesUb++;
        if (elegidos[0] == 3) vecesUd++;
    }
    cout << "En 3000 sorteos: ub salio " << vecesUb << " veces, ud salio " << vecesUd << " veces\n";

    cout << "ul (sin amigos) eligio: " << Usuario::obtener(11)->elegirAmigosPonderados(elegidos, 3) << "\n";
    Usuario::liberarTodos();
    return 0;
}