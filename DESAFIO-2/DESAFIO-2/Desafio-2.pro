TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        ALBUMES.cpp \
        Cadena.cpp \
        FOTOS.cpp \
        Fecha.cpp \
        MatrizAmistad.cpp \
        Mensajes.cpp \
        MenuAmistades.cpp \
        PersistenciaUsuarios.cpp \
        Recursos.cpp \
        Usuario.cpp \
        main.cpp

HEADERS += \
    ALBUMES.h \
    Cadena.h \
    FOTOS.h \
    Fecha.h \
    Lista.h \
    MatrizAmistad.h \
    Mensajes.h \
    MenuAmistades.h \
    Persistencia.h \
    Recursos.h \
    Usuario.h
