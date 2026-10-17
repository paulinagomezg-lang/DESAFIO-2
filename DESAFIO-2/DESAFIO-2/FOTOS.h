#ifndef FOTOS_H
#define FOTOS_H
#include"FECHA.h"
#include <iostream>
using namespace std;

class Foto{

private:
    char* Nombre; //nombre sin extension
    Fecha Fecha_de_Captura; //fecha de toma de la foto
    unsigned int Tamanio; // tamaño de bytes
    unsigned int Likes; //Cantidad de likes

    static int Longitud (const char* Cadena); //recorre caracter por caracter
    static char* Copiar_Texto (const char* Origen); //guarda texto e la memoria dinamica
    static bool Comparacion_Caracter (const char* A, const char* B); //Evalua caracter por caracter y valida que ambas cadenas tengan la misma longitud.

public:
    static const char* const SUB_RUTA; //parte fija de la ruta

    Foto();
    Foto(const char* Nuevo_Nombre, const Fecha& Nueva_Fecha, unsigned int Nuevo_Tamanio, unsigned int Nuevos_Likes = 0); //datos
    Foto(const Foto& Otra); //copia
    ~Foto(); //destructor

    //sobrecarga
    Foto& operator = (const Foto& Otra);
    bool operator == (const Foto& Otra) const;

    //getters
    const char* getNombre() const{
        return Nombre;
    }
    const Fecha& getFecha_de_Captura() const{
        return Fecha_de_Captura;
    }
    unsigned int getTamanio() const{
        return Tamanio;
    }
    unsigned int getLikes() const{
        return Likes;
    }

    //setters :validar antes de modificar
    bool setNombre (const char* Nuevo_Nombre);
    bool setFecha_de_Captura (const Fecha& Nueva_Fecha);
    bool setTamanio (unsigned int Nuevo_Tamanio);
    void setLikes (unsigned int Nuevos_Likes);

    //parte de los likes
    void Dar_Like();
    void Quitar_Like();

    //validacion
    static bool Nombre_Valido(const char* Revisar_Nombre);
    bool Es_Valida() const;

    //Ruta estipulada por el cliente
    void Mostrar_Ruta(ostream& Salida, const char* Usuario, const char* Album) const; //ruta de la foto sin reserva de memoria
    static void Mostrar_Ruta_Perfil(ostream& Salida, const char* Usuario);

    unsigned int Memoria() const; //consulta bytes utilizados

    friend ostream& operator << (ostream& Salida, const Foto& F);

};

#endif // FOTOS_H