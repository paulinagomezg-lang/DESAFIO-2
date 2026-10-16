#ifndef MENUAMISTADES_H
#define MENUAMISTADES_H

// Interacción por consola de la persona A:
//   II.  Inicio de sesión y 3 fotos de perfil ponderadas
//   III. Administrar amistades (ver amigos, enviar y aceptar solicitudes)
class MenuAmistades {
public:
    static int iniciarSesion();                 // índice del usuario, o -1 si se canceló
    static void mostrarFotosPerfil(int usuario);
    static void administrarAmistades(int usuario);

    // Lectura segura de consola (la puede usar todo el menú)
    static void leerLinea(char* destino, int tamano);
    static int leerOpcion();                    // número escrito, o -1 si no es un número

private:
    static void verAmigos(int usuario);
    static void enviarSolicitud(int usuario);
    static void gestionarSolicitudes(int usuario);
    static void reportar(const char* funcionalidad);

    static const int INTENTOS_LOGIN = 3;
    static const int FOTOS_A_MOSTRAR = 3;
};

#endif