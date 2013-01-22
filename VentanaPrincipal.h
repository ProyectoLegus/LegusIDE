#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QtGui>
#include "Editor.h"
#include "ColoreadoDeCodigo.h"

namespace Ui {
    class VentanaPrincipal;
}

class VentanaPrincipal : public QMainWindow
{
    Q_OBJECT

public:
    explicit VentanaPrincipal(QWidget *parent = 0);
    ~VentanaPrincipal();

private:
    Ui::VentanaPrincipal *ui;
    void cargarConfiguraciones();
    void agregarVentana(QString archivo="");
    Editor* ventanaActiva();

protected:
    void keyPressEvent(QKeyEvent *);

private slots:
    void on_accionNuevo_Archivo_triggered();
    void on_accionGuardar_Archivo_triggered();
    void on_accionAbrir_Archivo_triggered();
    void on_accionCerrar_Archivo_triggered();
    void on_accionSalir_triggered();
    void on_accionDeshacer_triggered();
    void on_actionAccionMostrarPanelBuscar_triggered();
    void on_btnCerrarBuscar_clicked();
    void on_accionInsertar_Bloque_Si_triggered();
    void on_accionInsertar_Bloque_Para_triggered();
    void on_accionInsertar_Bloque_Caso_triggered();
    void on_accionInsertar_Bloque_Mientras_triggered();
    void on_accionInsertar_Bloque_Repetir_triggered();
    void on_accionInsertar_Bloque_Repita_triggered();
    void on_accionInsertar_Bloque_Repita_Desde_triggered();
    void on_accionInsertar_Bloque_Para_Cada_triggered();
    void on_accionInsertar_Bloque_Funcion_triggered();
    void on_accionInsertar_Bloque_Utilizar_triggered();
};

#endif // VENTANAPRINCIPAL_H
