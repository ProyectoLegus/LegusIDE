#include <QtGui/QApplication>
#include "VentanaPrincipal.h"
#include "Bienvenida.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    VentanaPrincipal w;
//    w.show();

//    return a.exec();


    Bienvenida v;
    v.show();

    return a.exec();
}
