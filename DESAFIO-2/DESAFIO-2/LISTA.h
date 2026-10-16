#ifndef LISTA_H
#define LISTA_H
//Es una plantilla que reemplaza los contenedores  que guarda elementos en MeDi,duplicacapacidad y esta para todas las funciones

template <typename T>
class Lista{

private:
    T* Datos; //arreglo dinamico con los elementos
    int Tamanio; //cantidad de elementos guardados
    int Capacidad; //cantidad de espacios reservados

    //funcion interna
    void Crecer(); //duplica capacidad cuando se llena

public:

    Lista(int Capacidad_Inicial = 4);
    Lista(const Lista<T>& Otra); //de copia
    ~Lista(); //destructor

    //sobrecarga

    Lista<T>& operator = (const Lista<T>& Otra); //asignacion
    T& operator[](int i); // acceso para poder modificar
    const T& operator[](int i) const; //solo de lectura

    //operaciones

    void Agregar (const T& Elemento); //para agregar al final
    void Vaciar(); //deja la lista sin elementos

    //getters

    int getTamanio() const{
        return Tamanio;
    }
    int getCapacidad() const{
        return Capacidad;
    }
    bool Esta_Vacia() const{
        return Tamanio == 0;
    }

    //consultas bytes del arreglo reservado y no incluye memoria dinamica interna

    unsigned int Memoria() const{
        return sizeof(T) * Capacidad;
    }
};

//implementacion ya que no se debe de usar cpp
//constructores y destructor

template <typename T>
Lista<T>::Lista(int Capacidad_Inicial){
    Tamanio = 0;
    Capacidad = (Capacidad_Inicial > 0) ? Capacidad_Inicial : 1;
    Datos = new T[Capacidad];
}

//copia para la lista nueva tiene su propio arreglo

template <typename T>
Lista<T>::Lista(const Lista<T>& Otra){
    Tamanio = Otra.Tamanio;
    Capacidad = Otra.Capacidad;
    Datos = new T[Capacidad];

    for (int i = 0; i < Tamanio; i++){
        Datos[i] = Otra.Datos[i];
    }
}

template<typename T>
Lista<T>::~Lista(){
    delete[] Datos;
}

//operadores

template<typename T>
Lista<T>& Lista<T>::operator =(const Lista<T>& Otra){
    if (this != &Otra){
        T* PUNTERO_BLOQUE_MEMORIA = new T[Otra.Capacidad];
        for (int i = 0; i < Otra.Tamanio; i++){
            PUNTERO_BLOQUE_MEMORIA[i] = Otra.Datos[i];
        }

        delete[] Datos;

        Datos = PUNTERO_BLOQUE_MEMORIA;
        Tamanio = Otra.Tamanio;
        Capacidad = Otra.Capacidad;
    }
    return *this;
}

//devuelve referencia donde se accede al elemento sin copiarlo

template <typename T>
T& Lista<T>::operator [](int i) {
    return Datos[i];
}

template <typename T>
const T& Lista<T>::operator [](int i) const {
    return Datos[i];
}

//operaciones que reserva un arreglo del doble tamaño lo copia y libera el anterior

template<typename T>
void Lista<T>::Crecer() {
    int Nueva_Capacidad = Capacidad * 2;
    T* PUNTERO_BLOQUE_MEMORIA = new T[Nueva_Capacidad];

    for (int i = 0; i < Tamanio; i++){
        PUNTERO_BLOQUE_MEMORIA[i] = Datos[i];
    }

    delete[] Datos;
    Datos = PUNTERO_BLOQUE_MEMORIA;
    Capacidad = Nueva_Capacidad;
}

template<typename T>
void Lista<T>::Agregar(const T &Elemento){
    if (Tamanio == Capacidad){
        Crecer();
    }
    Datos[Tamanio] = Elemento;
    Tamanio++;
}

template<typename T>
void Lista<T>::Vaciar() {
    Tamanio = 0;
}


#endif // LISTA_H
