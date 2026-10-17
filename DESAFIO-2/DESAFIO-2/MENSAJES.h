#ifndef MENSAJES_H
#define MENSAJES_H
#include "FECHA.h"
#include <iostream>

//Clase mensaje: Un mensaje de hasta 512 caracteres que un usuario le envia a otro.

class Mensaje{
private:
    char* Emisor; //quien envia
    char* Receptor; //quien recibe
    Fecha Fecha_de_Envio; //Fecha en la que se envio
    bool Leidos; // true si se leyo o false si no ha sido leido
    char* Texto; // contenido de aquel mensaje

    //Vamos a utilizar estatic porque no depende de un mensaje en particular.

    static int Longitud(const char* Cadena);
    static char* Copiar_Texto(const char* Origen);
    static bool Comparacion_Caracter(const char* A, const char* B);
    static bool Usuario_Valido(const char* Usuario);

public:
    static const int TEXTO_MAX = 512; //Maximo de caracteres del texto
    //Constructor y destructor

    Mensaje();
    Mensaje(const char* Nuevo_Emisor, const char* Nuevo_Receptor, const Fecha& Nueva_Fecha, const char* Nuevo_Texto, bool Nuevo_Leidos = false); // con datos
    Mensaje(const Mensaje& Otro); //de copia
    ~Mensaje(); //destructor

    // Sobre carga: para que una misma operación acepte distintos tipos de datos
    Mensaje& operator = (const Mensaje& Otro); //copia profunda
    bool operator < (const Mensaje& Otro) const{ //oden por fecha de envio
        return Fecha_de_Envio < Otro.Fecha_de_Envio;
    }

    //Getters
    const char* getEmisor() const{
        return Emisor;
    }
    const char* getReceptor() const{
        return Receptor;
    }
    const Fecha& getFecha_de_Envio() const{
        return Fecha_de_Envio;
    }
    const char* getTexto() const{
        return Texto;
    }
    bool getLeidos() const{
        return Leidos;
    }

    //Setters: Validan antes de modificar. true si aplico el cambio y false sino

    bool setEmisor(const char* Nuevo_Emisor);
    bool setReceptor(const char* Nuevo_Receptor);
    bool setFecha_de_Envio(const Fecha& Nueva_Fecha);
    bool setTexto(const char* Nuevo_Texto);

    void setLeidos(bool Nuevo_Leidos){
        Leidos = Nuevo_Leidos;
    }
    void Marcar_Leido(){ //para modificar el estado
        Leidos = true;
    }

    static bool Texto_Valido(const char* Revisar_Texto);

    //Consultas

    bool Fue_Enviado_Por(const char* Usuario) const{
        return Comparacion_Caracter(Emisor, Usuario);
    }
    bool Fue_Recibido_Por(const char* Usuario){
        return Comparacion_Caracter(Receptor, Usuario);
    }
    bool Esta_en_Rango(const Fecha& Inicio, const Fecha& Fin)const;
    unsigned int Memoria() const; //byte objeto mas sus tres textos

    friend ostream& operator<<(ostream& Salida, const Mensaje& S);

};
























#endif // MENSAJES_H
