#ifndef USUARIO_H
#define USUARIO_H

#include <iostream>
#include "Fecha.h"          // antes decía "FECHA.h"
#include "Lista.h"
#include "MatrizAmistad.h"
#include "ALBUMES.h"        // NUEVO
#include "Mensajes.h"       // NUEVO

class Usuario {
public:
    // Resultado de enviar una solicitud de amistad

    // (va al inicio porque enviarSolicitud lo usa como tipo de retorno)

    enum ResultadoSolicitud {

        SOLICITUD_ENVIADA,

        ES_EL_MISMO_USUARIO,

        YA_SON_AMIGOS,

        SOLICITUD_YA_ENVIADA,

        EL_OTRO_YA_TE_ENVIO
    };

private:
    // ---- Datos de cada usuario ----
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

    // ---- Miembros estáticos: todos los usuarios de la plataforma ----

    static Usuario** usuarios;    // arreglo dinámico de punteros a Usuario

    static int cantidad;          // usuarios guardados

    static int capacidad;         // espacio reservado

    static MatrizAmistad matriz;  // amistades de todos contra todos      <-- NUEVO

    static void mergeSort(Usuario** arreglo, Usuario** auxiliar, int izquierda, int derecha);

    static void mezclar(Usuario** arreglo, Usuario** auxiliar, int izquierda, int medio, int derecha);

public:
    // ---- Constructores y destructor ----

    Usuario();

    Usuario(const char* nombreUsuario, const char* contrasena, const char* correo,
            const Fecha& fechaNacimiento, const char* ciudad, const char* pais,
            const Fecha& fechaInscripcion);
    Usuario(const Usuario& otro);               // constructor de copia

    Usuario& operator=(const Usuario& otro);    // asignación

    ~Usuario();

    // ---- Getters ----
    const char* getNombreUsuario() const { return nombreUsuario; }

    const char* getCorreo() const { return correo; }

    const char* getCiudad() const { return ciudad; }

    const char* getPais() const { return pais; }

    const Fecha& getFechaNacimiento() const { return fechaNacimiento; }

    const Fecha& getFechaInscripcion() const { return fechaInscripcion; }

    int getIndice() const { return indice; }

    const Lista<int>& getSolicitudesRecibidas() const { return solicitudesRecibidas; }

    Lista<Album>   albumes;             // álbumes del usuario

    Lista<Mensaje> mensajesRecibidos;   // ordenados por fecha de envío

    // ---- Setters ----
    void setCiudad(const char* nuevaCiudad);

    void setPais(const char* nuevoPais);

    bool verificarContrasena(const char* intento) const;

    // ---- Operadores sobrecargados ----
    bool operator<(const Usuario& otro) const;

    bool operator==(const Usuario& otro) const;

    bool operator==(const char* nombre) const;

    friend std::ostream& operator<<(std::ostream& salida, const Usuario& usuario);

    // ---- Amistades y solicitudes ----                                     <-- NUEVO
    bool esAmigoDe(const Usuario& otro) const;

    int cantidadAmigos() const;

    void obtenerAmigos(Lista<int>& destino) const;

    bool tieneSolicitudDe(int indiceEmisor) const;

    void agregarSolicitudRecibida(int indiceEmisor);      // la usa la carga del archivo

    ResultadoSolicitud enviarSolicitud(Usuario& receptor);

    bool aceptarSolicitud(int posicion);                  // posición en solicitudesRecibidas

    // ---- Gestión de todos los usuarios (estáticos) ----
    static void agregarUsuario(Usuario* nuevo);   // el arreglo pasa a ser dueño del objeto

    static Usuario* obtener(int indice);

    static int getCantidad() { return cantidad; }

    static void liberarTodos();

    static void ordenar();                        // Merge Sort por nombre + reasigna índices

    static int eliminarDuplicados();              // requiere el arreglo ordenado

    static int buscar(const char* nombre);        // búsqueda binaria; -1 si no existe

    static void prepararMatriz();                 // reserva la matriz para 'cantidad' usuarios

    static MatrizAmistad& getMatriz() { return matriz; }

    // ---- Fotos de perfil ponderadas (funcionalidad II) ----
    void rutaFotoPerfil(char* destino, int tamano) const;

    int elegirAmigosPonderados(int* elegidos, int k) const;   // devuelve cuántos eligió

    static float pesoAmistad(int usuario, int amigo);

    // ---- Álbumes y mensajes ----
    void agregarAlbum(const Album& nuevo);

    int cantidadAlbumes() const { return albumes.tamano(); }

    Album& getAlbum(int i) { return albumes[i]; }

    const Album& getAlbum(int i) const { return albumes[i]; }

    void agregarMensaje(const Mensaje& nuevo);      // lo inserta en orden de fecha

    int cantidadMensajes() const { return mensajesRecibidos.tamano(); }

    Mensaje& getMensaje(int i) { return mensajesRecibidos[i]; }

    const Mensaje& getMensaje(int i) const { return mensajesRecibidos[i]; }

    static long memoriaTotal();   // memoria de usuarios, álbumes, mensajes y matriz
};

#endif // USUARIO_H