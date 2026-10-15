#include <iostream>
#include "Lista.h"
using namespace std;

int main() {
    Lista<int> a;
    for (int i = 0; i < 10; i++) a.agregar(i * 10);   // crece 4 -> 8 -> 16

    Lista<int> b(a);         // copia profunda
    b[0] = 99;
    cout << "a[0] = " << a[0] << ", b[0] = " << b[0] << ", tamano = " << b.tamano() << "\n";

    a.eliminarEn(3);         // quita el 30
    cout << "a[3] = " << a[3] << ", buscar 90 -> " << a.buscar(90) << ", buscar 5 -> " << a.buscar(5) << "\n";
    return 0;
}