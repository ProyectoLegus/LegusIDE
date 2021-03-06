#-------------------------------------------------
#
# Project created by QtCreator 2013-01-17T10:56:44
#
#-------------------------------------------------

QT       += core gui

TARGET = LegusIDE
TEMPLATE = app


SOURCES += main.cpp\
        ventanaprincipal.cpp \
        ColoreadoDeCodigo.cpp \
    Editor.cpp \
    Bienvenida.cpp \
    ColoreadoDeCodigoJava.cpp \
    Configuraciones.cpp \
    Acerca.cpp

HEADERS  += VentanaPrincipal.h \
        ColoreadoDeCodigo.h \
    Editor.h \
    Bienvenida.h \
    ColoreadoDecodigoJava.h \
    Configuraciones.h \
    Acerca.h

FORMS    += VentanaPrincipal.ui \
    Bienvenida.ui \
    Configuraciones.ui \
    Acerca.ui

RESOURCES += \
    imagenes/iconos.qrc \
    autocompletacion/autocompletacion.qrc

OTHER_FILES += \
    scripts/compilacion_windows.bat \
    autocompletacion/palabras_incorporadas.txt
