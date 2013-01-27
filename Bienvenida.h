#ifndef BIENVENIDA_H
#define BIENVENIDA_H

#include <QtGui>
#include "VentanaPrincipal.h"

namespace Ui {
    class Bienvenida;
}

class Bienvenida : public QMainWindow
{
    Q_OBJECT

public:
    explicit Bienvenida(QWidget *parent = 0);
    ~Bienvenida();

private slots:
    void on_btnCrearArchivo_clicked();

private:
    Ui::Bienvenida *ui;
};

#endif // BIENVENIDA_H
