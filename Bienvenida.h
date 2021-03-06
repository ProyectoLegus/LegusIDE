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
    void on_listaArchivosRecientes_doubleClicked(const QModelIndex &index);
    void on_btnAbrirArchivo_clicked();
    void on_listaArchivosRecientes_itemClicked(QListWidgetItem *item);
    void on_chkAbrirVentana_stateChanged(int arg1);
    void on_btnTutorialesLegus_clicked();
    void on_btnAbrirOtroArchivo_clicked();

private:
    Ui::Bienvenida *ui;
    void cargarConfiguraciones();
    void abrirArchivoSeleccionado();
};

#endif // BIENVENIDA_H
