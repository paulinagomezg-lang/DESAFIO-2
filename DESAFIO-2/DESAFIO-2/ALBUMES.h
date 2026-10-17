#ifndef ALBUMES_H
#define ALBUMES_H
#include <iostream>
#include "Lista.h"
#include "FOTOS.h"
using namespace std;

//clase album de 0 o mas fotos con configuracion de privacidad
//0 = Publico, 1= Privado, 2= Amigos

class Album{
private:

    char* Nombre; //nombre del album
    int Privacidad; //pu,pri o ami
    Lista<Foto> Fotos; //fotos

    static int Longitud (const char* Cadena);
    static char* Copiar_Texto (const char* Origen);
    static bool Comparacion_Caracter (const char* A, const char* B);

public:
    static const int PUBLICO = 0;
    static const int PRIVADO = 1;
    static const int AMIGOS = 2;

    //constructores y destructor

    Album();
    Album (const char* Nuevo_Nombre, int Nueva_Privacidad);
    Album (const Album& Otro); //de copia
    ~Album(); //destructor

    //sobrecarga
    Album& operator = (const Album& Otro); //asignacion copia profunda
    bool operator == (const Album& Otro) const; //iguales si tienen el mismo nombre

    //getters

    const char* getNombre() const{
        return Nombre;
    }
    int getPrivacidad() const{
        return Privacidad;
    }
    int Cantidad_Fotos() const{
        return Fotos.tamano();
    }
    Foto& getFoto(int i){
        return Fotos[i]; //para dar o quitar like
    }
    const Foto& getFoto(int i) const{
        return Fotos[i]; //solo de lectura
    }

    //setters valida antes de modificar

    bool setNombre(const char* Nuevo_Nombre);
    bool setPrivacidad(int Nueva_Privacidad);

    //fotos

    bool Agregar_Foto(const Foto& Nueva_Foto); //agrega fotos validas
    bool Indice_Valido(int i) const; //existe la foto i?

    //permisos que se le da a albumes

    bool Puede_Ver(bool Es_Dueno, bool Es_Amigo) const;

    //carrusel que realiza un recorrido circular donde va de la ultima a la primera y de la primera a la ultima

    int Siguiente(int Actual) const;
    int Anterior(int Actual) const;

    //Validacion de las configuraciones

    static bool Nombre_Valido(const char* Revisar_Nombre);
    static bool Privacidad_Valida(int Revisar_Privacidad);

    //Rutas del album estipuladas por el cliente

    void Mostrar_Ruta(ostream& Salida, const char* Usuario) const; //ruta del album: /users/(sub-ruta)/usuario/albums/album
    void Mostrar_Ruta_Foto(ostream& Salida, int i, const char* Usuario) const;

    //consultas

    static const char* Texto_Privacidad(int Tipo); //publico, privado o amigos
    unsigned int Memoria() const; //byte del album y todas las fotos

    //funcion amiga

    friend ostream& operator<<(ostream& Salida, const Album& A);


};
#endif
