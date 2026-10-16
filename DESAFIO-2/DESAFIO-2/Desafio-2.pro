TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Cadena.cpp \
        Fecha.cpp \
        MatrizAmistad.cpp \
        Mensajes.cpp \
        MenuAmistades.cpp \
        PersistenciaUsuarios.cpp \
        Recursos.cpp \
        Usuario.cpp \
        main.cpp

HEADERS += \
    Cadena.h \
    Fecha.h \
    Lista.h \
    MatrizAmistad.h \
    Mensajes.h \
    MenuAmistades.h \
    Persistencia.h \
    Recursos.h \
    Usuario.h
