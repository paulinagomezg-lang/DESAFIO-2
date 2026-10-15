#ifndef USUARIO_H
#define USUARIO_H

#include <iostream>
#include "Fecha.h"
#include "Lista.h"

class Usuario {
private:
    char* nombreUsuario;

    char* contrasena;

    char* correo;

    char* ciudad;

    char* pais;

    Fecha fechaNacimiento;

    Fecha fechaInscripcion;

    int indice;                        // posición en el arreglo = fila/columna en la matriz

    Lista<int> solicitudesRecibidas;   // índices de quienes le enviaron solicitud

    void copiarDatosDe(const Usuario& otro);

    void liberar();

    static Usuario** usuarios;   // arreglo dinámico de punteros a Usuario

    static int cantidad;         // usuarios guardados

    static int capacidad;        // espacio reservado

    static void mergeSort(Usuario** arreglo, Usuario** auxiliar, int izquierda, int derecha);

    static void mezclar(Usuario** arreglo, Usuario** auxiliar, int izquierda, int medio, int derecha);

public:
    Usuario();

    Usuario(const char* nombreUsuario, const char* contrasena, const char* correo,
            const Fecha& fechaNacimiento, const char* ciudad, const char* pais,
            const Fecha& fechaInscripcion);
    Usuario(const Usuario& otro);               // constructor de copia

    Usuario& operator=(const Usuario& otro);    // asignación

    ~Usuario();

    // Getters
    const char* getNombreUsuario() const { return nombreUsuario; }

    const char* getCorreo() const { return correo; }

    const char* getCiudad() const { return ciudad; }

    const char* getPais() const { return pais; }

    const Fecha& getFechaNacimiento() const { return fechaNacimiento; }

    const Fecha& getFechaInscripcion() const { return fechaInscripcion; }

    int getIndice() const { return indice; }

    const Lista<int>& getSolicitudesRecibidas() const { return solicitudesRecibidas; }

    // Setters
    void setCiudad(const char* nuevaCiudad);

    void setPais(const char* nuevoPais);

    bool verificarContrasena(const char* intento) const;

    // Operadores sobrecargados
    bool operator<(const Usuario& otro) const;

    bool operator==(const Usuario& otro) const;

    bool operator==(const char* nombre) const;

    friend std::ostream& operator<<(std::ostream& salida, const Usuario& usuario);

    static void agregarUsuario(Usuario* nuevo);   // el arreglo pasa a ser dueño del objeto

    static Usuario* obtener(int indice);

    static int getCantidad() { return cantidad; }

    static void liberarTodos();

    static void ordenar();                        // Merge Sort por nombre + reasigna índices

    static int eliminarDuplicados();              // requiere el arreglo ordenado

    static int buscar(const char* nombre);        // búsqueda binaria; -1 si no existe
};

#endif // USUARIO_H
