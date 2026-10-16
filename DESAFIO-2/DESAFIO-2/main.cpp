// Prueba de Lista<T> y de la clase Album. No es parte del programa final.
// En Qt Creator: reemplazar TODO el contenido de main.cpp por este archivo.
#include <iostream>
#include "FECHA.h"
#include "FOTOS.h"
#include "LISTA.h"
#include "ALBUMES.h"
using namespace std;

int fallos = 0;
void verificar(bool Condicion, const char* Descripcion) {
    cout << (Condicion ? "[OK]    " : "[FALLA] ") << Descripcion << "\n";
    if (!Condicion) fallos++;
}

bool iguales(const char* A, const char* B) {
    int i = 0;
    while (A[i] != '\0' && A[i] == B[i]) i++;
    return A[i] == B[i];
}

int main() {
    Fecha F1("26/12/2025");

    // ---------------- Lista<T> ----------------
    Lista<int> Numeros(2);
    for (int i = 0; i < 10; i++) Numeros.Agregar(i * 10);
    verificar(Numeros.getTamanio() == 10 && Numeros[9] == 90, "Lista: agregar 10 elementos");
    verificar(Numeros.getCapacidad() == 16, "Lista: duplica su capacidad (2 -> 4 -> 8 -> 16)");
    Lista<int> Copia_Numeros = Numeros;
    Copia_Numeros[0] = 999;
    verificar(Numeros[0] == 0, "Lista: constructor de copia es independiente");
    Lista<int> Otra_Lista;
    Otra_Lista = Numeros;
    Otra_Lista = Otra_Lista;
    verificar(Otra_Lista.getTamanio() == 10 && Otra_Lista[5] == 50, "Lista: operador = y autoasignacion");

    // ---------------- Album: construcción ----------------
    Album A1("Cartagena2026", Album::AMIGOS);
    verificar(iguales(A1.getNombre(), "Cartagena2026") && A1.getPrivacidad() == Album::AMIGOS,
              "Album: constructor y getters");
    verificar(A1.Cantidad_Fotos() == 0, "Album: empieza con cero fotos");
    verificar(Album("X", 7).getPrivacidad() == Album::PRIVADO, "Album: privacidad invalida queda PRIVADO");
    Album Vacio;
    verificar(iguales(Vacio.getNombre(), "") && Vacio.getPrivacidad() == Album::PRIVADO,
              "Album: constructor por defecto");

    // ---------------- Fotos ----------------
    for (int i = 0; i < 5; i++) {
        char Nombre[] = "Foto0";
        Nombre[4] = '0' + i;
        A1.Agregar_Foto(Foto(Nombre, F1, 1000 + i));
    }
    verificar(A1.Cantidad_Fotos() == 5, "Album: agregar 5 fotos");
    verificar(!A1.Agregar_Foto(Foto("Mala", F1, 0)) && A1.Cantidad_Fotos() == 5,
              "Album: rechaza fotos invalidas");
    verificar(A1.Indice_Valido(4) && !A1.Indice_Valido(5) && !A1.Indice_Valido(-1), "Album: indices validos");
    A1.getFoto(2).Dar_Like();
    verificar(A1.getFoto(2).getLikes() == 1, "Album: dar like a una foto del album");

    // ---------------- Carrusel ----------------
    verificar(A1.Siguiente(0) == 1 && A1.Siguiente(4) == 0, "Carrusel: siguiente es circular");
    verificar(A1.Anterior(3) == 2 && A1.Anterior(0) == 4, "Carrusel: anterior es circular");
    verificar(Vacio.Siguiente(0) == -1 && Vacio.Anterior(0) == -1, "Carrusel: album vacio retorna -1");

    // ---------------- Permisos ----------------
    Album Publico("P", Album::PUBLICO), Privado("Q", Album::PRIVADO);
    verificar(Publico.Puede_Ver(false, false), "Permisos: PUBLICO lo ve cualquiera");
    verificar(A1.Puede_Ver(false, true) && !A1.Puede_Ver(false, false), "Permisos: AMIGOS solo amigos");
    verificar(!Privado.Puede_Ver(false, true) && Privado.Puede_Ver(true, false),
              "Permisos: PRIVADO solo el dueno");

    // ---------------- Setters ----------------
    verificar(!A1.setNombre("con espacio") && !A1.setNombre("") && iguales(A1.getNombre(), "Cartagena2026"),
              "setNombre rechaza nombres invalidos");
    verificar(!A1.setPrivacidad(5) && A1.getPrivacidad() == Album::AMIGOS && A1.setPrivacidad(Album::PUBLICO),
              "setPrivacidad valida el rango 0..2");

    // ---------------- Copia profunda ----------------
    Album Copia = A1;
    Copia.getFoto(2).Dar_Like();
    Copia.setNombre("Copia");
    verificar(A1.getFoto(2).getLikes() == 1 && Copia.getFoto(2).getLikes() == 2 &&
                  iguales(A1.getNombre(), "Cartagena2026"), "Constructor de copia: fotos y nombre independientes");
    Album Asignado("Z", Album::PUBLICO);
    Asignado = A1;
    Asignado = Asignado;
    verificar(Asignado.Cantidad_Fotos() == 5 && Asignado == A1, "Operador = y autoasignacion");

    // ---------------- Rutas e impresión ----------------
    cout << "        Ruta album: ";
    A1.Mostrar_Ruta(cout, "juanito76");
    cout << "\n        Ruta foto 0: ";
    A1.Mostrar_Ruta_Foto(cout, 0, "juanito76");
    cout << "\n        " << A1 << "\n";
    cout << "        Memoria de A1: " << A1.Memoria() << " bytes\n";

    cout << "\n" << (fallos == 0 ? "Todas las pruebas pasaron." : "Hay pruebas fallidas.") << "\n";
    return fallos;
}
