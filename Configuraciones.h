#ifndef CONFIGURACIONES_H
#define CONFIGURACIONES_H

#include <QDialog>
#include <QSettings>
#include <QBrush>
#include <QPalette>
#include <QListWidgetItem>

namespace Ui {
    class Configuraciones;
}

class Configuraciones : public QDialog
{
    Q_OBJECT

public:
    explicit Configuraciones(QWidget *parent = 0);
    ~Configuraciones();

private slots:
    void on_listaOpciones_itemSelectionChanged();
    void agregarElementosAListaColoreado();
    void on_chkVentanaDeBienvenida_stateChanged(int arg1);
    void on_btnCancelar_clicked();
    void on_btnAceptar_clicked();
    void on_btnAplicar_clicked();
    void aplicarConfiguraciones();
    void on_checkBox_clicked();
    void on_listaColoreado_itemClicked(QListWidgetItem *item);

private:
    Ui::Configuraciones *ui;
};

#endif // CONFIGURACIONES_H
