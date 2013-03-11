#include "Acerca.h"
#include "ui_Acerca.h"

Acerca::Acerca(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Acerca)
{
    ui->setupUi(this);
}

Acerca::~Acerca()
{
    delete ui;
}

void Acerca::on_btnGithub_clicked()
{
    QDesktopServices::openUrl(QUrl("http://github.com/rsiwady29/Legus"));
    QDesktopServices::openUrl(QUrl("http://github.com/rsiwady29/LegusIDE"));
}
