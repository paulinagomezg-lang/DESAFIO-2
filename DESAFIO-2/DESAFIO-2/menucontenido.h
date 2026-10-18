
#ifndef MENUCONTENIDO_H
#define MENUCONTENIDO_H

#include "Usuario.h"

    class MenuContenido {
public:
    static void visitarPerfil(Usuario& actual);         // funcionalidad IV

    static void administrarMensajes(Usuario& actual);   // funcionalidad VI

private:
    static void enviarMensaje(Usuario& actual);

    static void leerNuevos(Usuario& actual);

    static void buscarPorRango(Usuario& actual);

    static void leerTexto(const char* pregunta, char* destino, int tamano);

    static Fecha leerFecha(const char* pregunta);

    static void reportarRecursos();                     // funcionalidad VII
};

#endif