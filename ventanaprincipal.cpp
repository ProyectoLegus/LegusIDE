#include "VentanaPrincipal.h"
#include "ui_VentanaPrincipal.h"

VentanaPrincipal::VentanaPrincipal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VentanaPrincipal)
{
    ui->setupUi(this);

    /*Inicializacion*/
    ui->barraSalida->setVisible(false);
}

VentanaPrincipal::~VentanaPrincipal()
{
    delete ui;
}
