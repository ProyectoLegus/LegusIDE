#ifndef CONFIGURACIONES_H
#define CONFIGURACIONES_H

#include <QDialog>
#include <QSettings>
#include <QBrush>
#include <QPalette>
#include <QListWidgetItem>
#include <QColorDialog>
#include <QFileDialog>

#include <QMessageBox>
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
    void aplicarConfiguraciones();
    void on_listaColoreado_itemClicked(QListWidgetItem *item);
    void on_btnForeground_clicked();
    void on_btnBackground_clicked();

    void on_btnCargarArchivo_clicked();

    void on_btnAgregarXml_clicked();

    void on_archivosXML_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::Configuraciones *ui;
    QColor obtenerColor();
    void cargarConfiguraciones();
    void cargarFuncionesXml();
};

#endif // CONFIGURACIONES_H
