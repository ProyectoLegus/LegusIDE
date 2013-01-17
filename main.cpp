#include <QtGui/QApplication>
#include "VentanaPrincipal.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VentanaPrincipal w;
    w.show();

    return a.exec();
}
