#include <QtGui/QApplication>
#include "VentanaPrincipal.h"
#include "Bienvenida.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSettings configuraciones("ArchivoConfiguracion.ini",QSettings::IniFormat);
    bool AbrirBievenida = configuraciones.value("AbrirBienvenida", true).value<bool>();

    if( AbrirBievenida )
    {
        Bienvenida v;
        v.show();
        return a.exec();
    }
    else
    {
        VentanaPrincipal w;
        w.show();
        return a.exec();
    }
    return a.exec();
}
