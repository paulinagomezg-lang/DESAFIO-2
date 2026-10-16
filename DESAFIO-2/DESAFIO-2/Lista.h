#ifndef LISTA_H
#define LISTA_H

#include "Recursos.h"

// Arreglo dinámico genérico propio (reemplaza a los contenedores de la STL).
// Va completo en el .h porque es una plantilla.
template <typename T>
class Lista {
private:
    T* datos;
    int cantidad;     // elementos guardados
    int capacidad;    // espacio reservado

    void crecer() {
        int nuevaCapacidad = (capacidad == 0) ? 4 : capacidad * 2;
        T* nuevos = new T[nuevaCapacidad];
        for (int i = 0; i < cantidad; i++) {
            nuevos[i] = datos[i];
            Recursos::sumar();
        }
        delete[] datos;
        datos = nuevos;
        capacidad = nuevaCapacidad;
    }

    void copiarDesde(const Lista& otra) {
        cantidad = otra.cantidad;
        capacidad = otra.cantidad;          // la copia queda ajustada, sin espacio sobrante
        datos = (capacidad > 0) ? new T[capacidad] : nullptr;
        for (int i = 0; i < cantidad; i++) {
            datos[i] = otra.datos[i];
            Recursos::sumar();
        }
    }

public:
    Lista() : datos(nullptr), cantidad(0), capacidad(0) {}

    Lista(const Lista& otra) { copiarDesde(otra); }   // constructor de copia

    Lista& operator=(const Lista& otra) {
        if (this != &otra) {
            delete[] datos;
            copiarDesde(otra);
        }
        return *this;
    }

    ~Lista() { delete[] datos; }

    void agregar(const T& valor) {
        if (cantidad == capacidad) crecer();
        datos[cantidad++] = valor;
    }

    bool eliminarEn(int posicion) {
        if (posicion < 0 || posicion >= cantidad) return false;
        for (int i = posicion; i < cantidad - 1; i++) {
            datos[i] = datos[i + 1];
            Recursos::sumar();
        }
        cantidad--;
        return true;
    }

    // Búsqueda lineal; devuelve la posición o -1. T debe tener operator==.
    int buscar(const T& valor) const {
        for (int i = 0; i < cantidad; i++) {
            Recursos::sumar();
            if (datos[i] == valor) return i;
        }
        return -1;
    }

    T& operator[](int i) { return datos[i]; }
    const T& operator[](int i) const { return datos[i]; }

    int tamano() const { return cantidad; }
    bool vacia() const { return cantidad == 0; }

    void vaciar() {
        delete[] datos;
        datos = nullptr;
        cantidad = 0;
        capacidad = 0;
    }

    // Bytes que ocupa: el objeto más el arreglo reservado
    long memoria() const { return (long)sizeof(*this) + (long)capacidad * (long)sizeof(T); }
};

#endif