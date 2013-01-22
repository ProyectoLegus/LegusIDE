#include "VentanaPrincipal.h"
#include "ui_VentanaPrincipal.h"

VentanaPrincipal::VentanaPrincipal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VentanaPrincipal)
{
    ui->setupUi(this);

    qApp->setStyle(new QCleanlooksStyle());
    cargarConfiguraciones();
    this->showMaximized();
}

VentanaPrincipal::~VentanaPrincipal()
{
    delete ui;
}

Editor* VentanaPrincipal::ventanaActiva()
{
    if( QMdiSubWindow *activo = ui->areaMDI->currentSubWindow() )
        return qobject_cast<Editor*>(activo->widget());
    return 0;
}

void VentanaPrincipal::cargarConfiguraciones()
{
    /*Decoracion*/
    ui->areaMDI->setBackground(QBrush(Qt::white));
    ui->areaMDI->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->areaMDI->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->areaMDI->setViewMode(QMdiArea::TabbedView);
    ui->areaMDI->setTabShape(QTabWidget::Rounded);

    /*Inicializacion*/
    ui->panelBuscarYReemplazar->setVisible(false);
    ui->barraSalida->setVisible(false);
}

/*********************+ MAIN WINDOW **************************+*/
void VentanaPrincipal::keyPressEvent(QKeyEvent *evento)
{
    QMainWindow::keyPressEvent(evento);
    if (evento->key() == Qt::Key_Escape)
    {
        if (ui->panelBuscarYReemplazar->isVisible())
        {
            ui->panelBuscarYReemplazar->setVisible(false);
        }
    }
}

void VentanaPrincipal::agregarVentana(QString archivo)
{
    Editor *editor = new Editor();
    /*Agregar estilo*/

    editor->setAttribute(Qt::WA_DeleteOnClose);
    new ColoreadoDeCodigo(editor->document());
    ui->areaMDI->addSubWindow(editor);

    if( archivo.isEmpty() )
    {
        editor->nuevoArchivo();
    }
    else
    {
        editor->cargarArchivo(archivo);
    }
    editor->show();
}

void VentanaPrincipal::on_accionNuevo_Archivo_triggered()
{
    agregarVentana();
}

void VentanaPrincipal::on_accionGuardar_Archivo_triggered()
{
    if( ventanaActiva() && ventanaActiva()->guardar() )
    {
        /*Actualizar Recientes*/
        ui->statusBar->showMessage("¡Archivo guardado exitosamente!", 2500);
    }
}

void VentanaPrincipal::on_accionAbrir_Archivo_triggered()
{
    QString archivo = QFileDialog::getOpenFileName(this, "Abrir Archivo",
                                                   QDir::homePath(), tr("Archivos de Legus (*.legus *.java)"));

    if( archivo.isEmpty() )
    {
        QMessageBox::critical(this,"Error", "Error al abrir el archivo");
        return;
    }
    agregarVentana(archivo);
}

void VentanaPrincipal::on_accionCerrar_Archivo_triggered()
{
    if( ventanaActiva() && ventanaActiva()->close() )
    {
        ui->areaMDI->removeSubWindow(ui->areaMDI->currentSubWindow());
    }
}

void VentanaPrincipal::on_accionSalir_triggered()
{
    close();
}

/********************************* OPCION DE MENU -> EDITAR ***********************/
void VentanaPrincipal::on_accionDeshacer_triggered()
{
    Editor *ventana = ventanaActiva();
    ventana->undo();
}

void VentanaPrincipal::on_actionAccionMostrarPanelBuscar_triggered()
{
    ui->panelBuscarYReemplazar->setVisible(true);
    ui->txtTextoABuscar->setFocus();
}

void VentanaPrincipal::on_btnCerrarBuscar_clicked()
{
    ui->panelBuscarYReemplazar->setVisible(false);
}


/************************************* BLOQUES *********************************/
void VentanaPrincipal::on_accionInsertar_Bloque_Si_triggered()
{
    if( ventanaActiva() )
    {
        ventanaActiva()->insertPlainText("Si /*Condicion*/ Entonces \n /*Bloque de Instrucciones*/ \nFin Si");
    }
}

void VentanaPrincipal::on_accionInsertar_Bloque_Para_triggered()
{
    if( ventanaActiva() )
    {
        ventanaActiva()->insertPlainText("Para /*Asignacion*/ hasta /*Literal Entera*/ ejecute \n /*Bloque de Instrucciones*/ \nFin Para");
    }
}

void VentanaPrincipal::on_accionInsertar_Bloque_Caso_triggered()
{
    if( ventanaActiva() )
    {
        ventanaActiva()->insertPlainText("Caso /*Variable*/ Entonces \n /*Bloque de Instrucciones*/ \nFin Si");
    }
}

void VentanaPrincipal::on_accionInsertar_Bloque_Mientras_triggered()
{
    if( ventanaActiva() )
    {
        ventanaActiva()->insertPlainText("Mientras /*Condicion*/ Ejecute \n /*Bloque de Instrucciones*/ \nFin Mientras");
    }
}

void VentanaPrincipal::on_accionInsertar_Bloque_Repetir_triggered()
{
    if( ventanaActiva() )
    {
        ventanaActiva()->insertPlainText("Repetir /*Literal Entera*/ Veces \n /*Bloque de Instrucciones*/ \nFin Repetir");
    }
}

void VentanaPrincipal::on_accionInsertar_Bloque_Repita_triggered()
{
    if( ventanaActiva() )
    {
        ventanaActiva()->insertPlainText("Repita \n /*Bloque de Instrucciones*/ \nMientras /*Condicion*/ \nFin Repita");
    }
}

void VentanaPrincipal::on_accionInsertar_Bloque_Repita_Desde_triggered()
{
    if( ventanaActiva() )
    {
        ventanaActiva()->insertPlainText("Repita Desde /*Literal Entera*/ Hasta /*Literal Entera*/ \n /*Bloque de Instrucciones*/ \nFin Repita Desde");
    }
}

void VentanaPrincipal::on_accionInsertar_Bloque_Para_Cada_triggered()
{
    if( ventanaActiva() )
    {
        ventanaActiva()->insertPlainText("Para Cada /*Variable*/ En /*Variable Arreglo*/ Ejecute \n /*Bloque de Instrucciones*/ \nFin Para Cada");
    }
}

void VentanaPrincipal::on_accionInsertar_Bloque_Funcion_triggered()
{
    /*Que sea al principio y si hay utilizar ponerlo despues*/
}

void VentanaPrincipal::on_accionInsertar_Bloque_Utilizar_triggered()
{
    /*Que sea al principio del documento*/
}
