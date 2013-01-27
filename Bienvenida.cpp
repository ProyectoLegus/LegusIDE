#include "Bienvenida.h"
#include "ui_Bienvenida.h"

Bienvenida::Bienvenida(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Bienvenida)
{
    ui->setupUi(this);
    qApp->setStyle(new QCleanlooksStyle());

}

Bienvenida::~Bienvenida()
{
    delete ui;
}

void Bienvenida::on_btnCrearArchivo_clicked()
{
    (new VentanaPrincipal(0))->show();
    this->close();
}
