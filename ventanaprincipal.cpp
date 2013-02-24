#include "VentanaPrincipal.h"
#include "ui_VentanaPrincipal.h"

VentanaPrincipal::VentanaPrincipal(QString nombreArchivo, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VentanaPrincipal)
{
    ui->setupUi(this);

    qApp->setStyle(new QCleanlooksStyle());
    cargarConfiguraciones();
    this->showMaximized();

    agregarVentana(nombreArchivo);
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
    //ui->areaMDI->setViewMode(QMdiArea::TabbedView);
    ui->areaMDI->setTabShape(QTabWidget::Rounded);
    /*SetTabsClosable*/

    /*Inicializacion*/
    ui->panelBuscarYReemplazar->setVisible(false);
    ui->barraSalida->setVisible(false);

    /*Agregar archivos Recientes*/
    QSettings configuraciones("ArchivoConfiguracion.ini",QSettings::IniFormat);
    QStringList archivosRecientes = configuraciones.value("ArchivosRecientes").toStringList();

    for(int i=0; i<archivosRecientes.size(); i++)
    {
        QAction *accion = new QAction(this);
        accion->setIconText(archivosRecientes.at(i));
        ui->menuArchivos_Recientes->addAction(accion);
        connect(accion, SIGNAL(triggered()), this, SLOT(accion_Abrir_Archivo_Reciente()));
    }
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
    else if( evento->key() == Qt::Key_F1)
    {
        ui->barraSalida->setVisible( !ui->barraSalida->isVisible() );
    }
}

void VentanaPrincipal::closeEvent(QCloseEvent *evento)
{
    if( !cerrarVentanas() )
    {
        evento->accept();
    }
    else
    {
        evento->ignore();
    }
}

void VentanaPrincipal::agregarVentana(QString archivo)
{
    Editor *editor = new Editor();
    /*Agregar estilo*/

    editor->setAttribute(Qt::WA_DeleteOnClose);

    if( archivo.isEmpty() )
    {
        new ColoreadoDeCodigo(editor->document());
        editor->nuevoArchivo();
    }
    else
    {
        QRegExp rx("*.java");
        rx.setPatternSyntax(QRegExp::Wildcard);
        if( !archivo.contains(rx) )
        {
            new ColoreadoDeCodigo(editor->document());
        }
        else
        {
            new ColoreadoDeCodigoJava(editor->document());
        }
        editor->cargarArchivo(archivo);
    }
    // CONENNE
    connect(ui->accionZoomAdentro,SIGNAL(triggered()), editor, SLOT(zoomAdentro()));
    connect(ui->accionZoomAfuera, SIGNAL(triggered()), editor, SLOT(zoomAfuera()));
    connect(ui->accionDeshacer, SIGNAL(triggered()), editor, SLOT(undo()));
    connect(ui->accionRehacer, SIGNAL(triggered()),editor, SLOT(redo()));
    connect(ui->accionCopiar, SIGNAL(triggered()), editor, SLOT(copy()));
    connect(ui->accionCortar, SIGNAL(triggered()), editor, SLOT(cut()));
    connect(ui->accionPegar, SIGNAL(triggered()), editor, SLOT(paste()));

    // Intellisense 0__0
    QCompleter *completador = new QCompleter(this);
    completador->setModel(obtenerModeloDeArchivo(":/autocompletacion/palabras_incorporadas.txt", completador));
    completador->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completador->setCaseSensitivity(Qt::CaseInsensitive);
    completador->setWrapAround(false);
    editor->establecerCompletador(completador);

    ui->areaMDI->addSubWindow(editor, Qt::SubWindow)->showMaximized();
    editor->show();
}

bool VentanaPrincipal::cerrarVentanas()
{
    bool resultado = false;
    foreach( QMdiSubWindow *ventana, ui->areaMDI->subWindowList())
    {
        if(!ventana->close())
        {
            resultado = true;
        }
    }
    return resultado;
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

    actualizarArchivosRecientes(archivo);
    agregarVentana(archivo);
}

void VentanaPrincipal::on_accionCerrar_Archivo_triggered()
{
    if( ventanaActiva())
    {
        QString nombreArchivo = ventanaActiva()->obtenerNombreArchivo();
        if(  ventanaActiva()->close()  )
        {
            ui->areaMDI->removeSubWindow(ui->areaMDI->currentSubWindow());
        }

        if( !nombreArchivo.isEmpty() )
        {
            ui->statusBar->showMessage(nombreArchivo,2500);
        }
    }
}

void VentanaPrincipal::on_accionCerrar_Todos_triggered()
{
    cerrarVentanas();
}

void VentanaPrincipal::on_accionGuardar_Todos_triggered()
{
    foreach( QMdiSubWindow *ventana, ui->areaMDI->subWindowList())
    {
        Editor *editor = qobject_cast<Editor*>(ventana->widget());
        editor->guardar();
    }
}

void VentanaPrincipal::on_accionSalir_triggered()
{
    close();
}

void VentanaPrincipal::actualizarArchivosRecientes(QString archivo)
{
    QSettings configuraciones("ArchivoConfiguracion.ini",QSettings::IniFormat);

    QStringList archivosRecientes = configuraciones.value("ArchivosRecientes").toStringList();
    archivosRecientes.removeAll(archivo);
    archivosRecientes.prepend(archivo);

    while( archivosRecientes.size() > MAXIMO_ARCHIVOS_RECIENTES)
    {
        archivosRecientes.removeLast();
    }

    for(int i=0; i<archivosRecientes.size(); i++)
    {
        QAction *accion = new QAction(this);
        accion->setIconText(archivosRecientes.at(i));
        ui->menuArchivos_Recientes->addAction(accion);
        connect(accion, SIGNAL(triggered()), this, SLOT(accion_Abrir_Archivo_Reciente()));
    }

    configuraciones.setValue("ArchivosRecientes", archivosRecientes);
    configuraciones.sync();
}

void  VentanaPrincipal::accion_Abrir_Archivo_Reciente()
{
    QAction *accion = (QAction*)sender();

    /*Revisar si ya esta abierto, si ya lo esta solo enfocar la ventana*/
    QMdiSubWindow *ultimoEditor = 0;
    foreach( QMdiSubWindow *ventana, ui->areaMDI->subWindowList())
    {
        Editor *editor = qobject_cast<Editor*>(ventana->widget());
        if( editor->obtenerNombreArchivo().compare(accion->text())==0 )
        {
            ultimoEditor = ventana;
        }
    }

    if( ultimoEditor == 0)
    {
        agregarVentana(accion->text());
    }
    else
    {
        ui->areaMDI->setActiveSubWindow(ultimoEditor);
    }
}

/********************************* OPCION DE MENU -> EDITAR ***********************/

void VentanaPrincipal::on_actionAccionMostrarPanelBuscar_triggered()
{
    if(ui->panelBuscarYReemplazar->isVisible())
    {
        ui->panelBuscarYReemplazar->setVisible(false);
    }
    else
    {
        ui->panelBuscarYReemplazar->setVisible(true);
        ui->txtTextoABuscar->setFocus();
    }
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
        ventanaActiva()->insertPlainText("Si /*Condicion*/ Entonces \n /*Bloque de Instrucciones*/ \nFin Si\n");
    }
}

void VentanaPrincipal::on_accionInsertar_Bloque_Para_triggered()
{
    if( ventanaActiva() )
    {
        ventanaActiva()->insertPlainText("Para /*Asignacion*/ hasta /*Literal Entera*/ ejecute \n /*Bloque de Instrucciones*/ \nFin Para\n");
    }
}

void VentanaPrincipal::on_accionInsertar_Bloque_Caso_triggered()
{
    if( ventanaActiva() )
    {
        ventanaActiva()->insertPlainText("Caso /*Variable*/ Entonces \n /*Bloque de Instrucciones*/ \nFin Si\n");
    }
}

void VentanaPrincipal::on_accionInsertar_Bloque_Mientras_triggered()
{
    if( ventanaActiva() )
    {
        ventanaActiva()->insertPlainText("Mientras /*Condicion*/ Ejecute \n /*Bloque de Instrucciones*/ \nFin Mientras\n");
    }
}

void VentanaPrincipal::on_accionInsertar_Bloque_Repetir_triggered()
{
    if( ventanaActiva() )
    {
        ventanaActiva()->insertPlainText("Repetir /*Literal Entera*/ Veces \n /*Bloque de Instrucciones*/ \nFin Repetir\n");
    }
}

void VentanaPrincipal::on_accionInsertar_Bloque_Repita_triggered()
{
    if( ventanaActiva() )
    {
        ventanaActiva()->insertPlainText("Repita \n /*Bloque de Instrucciones*/ \nMientras /*Condicion*/ \nFin Repita\n");
    }
}

void VentanaPrincipal::on_accionInsertar_Bloque_Repita_Desde_triggered()
{
    if( ventanaActiva() )
    {
        ventanaActiva()->insertPlainText("Repita Desde /*Literal Entera*/ Hasta /*Literal Entera*/ \n /*Bloque de Instrucciones*/ \nFin Repita Desde\n");
    }
}

void VentanaPrincipal::on_accionInsertar_Bloque_Para_Cada_triggered()
{
    if( ventanaActiva() )
    {
        ventanaActiva()->insertPlainText("Para Cada /*Variable*/ En /*Variable Arreglo*/ Ejecute \n /*Bloque de Instrucciones*/ \nFin Para Cada\n");
    }
}

void VentanaPrincipal::on_accionInsertar_Bloque_Funcion_triggered()
{
    /*Que sea al principio y si hay utilizar ponerlo despues*/
}

void VentanaPrincipal::on_accionInsertar_Bloque_Utilizar_triggered()
{
    /*Que sea al principio del documento*/
    if( ventanaActiva() )
    {
        ventanaActiva()->moveCursor(QTextCursor::Start);
        ventanaActiva()->insertPlainText("Utilizar /*#Puerto*/ Como /*tTipo De Sensor*/ En /*Variable*/\n\n\n");
    }
}

void VentanaPrincipal::on_accionCompilar_triggered()
{
    ui->barraSalida->setVisible(true);

}

void VentanaPrincipal::on_accionBarra_de_Salida_triggered()
{
    ui->barraSalida->setVisible(true);
}

void VentanaPrincipal::on_accionVista_a_la_Par_triggered()
{
    if( ui->areaMDI->subWindowList().size() < 2)
    {
        return;
    }

/*
    ui->areaMDI->setViewMode(QMdiArea::SubWindowView);
    QMdiSubWindow *ventana1 = ui->areaMDI->subWindowList().at(0);
    QMdiSubWindow *ventana2 = ui->areaMDI->subWindowList().at(1);

    ventana1->setGeometry( 0, 0, ui->areaMDI->x()/2,ui->areaMDI->y());
    ventana2->setGeometry(ui->areaMDI->x()/2, 0, ui->areaMDI->x()/2, ui->areaMDI->y() );
*/
}

void VentanaPrincipal::on_accionOpciones_triggered()
{
    (new Configuraciones(0))->show();
    //ui->areaMDI->setViewMode(QMdiArea::TabbedView);
}

void VentanaPrincipal::on_txtTextoABuscar_returnPressed()
{
    if( ventanaActiva() == 0)
    {   /*Nada que hacer ninguna ventana esta activa*/
        return;
    }

    bool usaExpresionRegular = ui->chkUsaExpresionRegular->isChecked();
    QString texto = ui->txtTextoABuscar->text();

    QTextCursor cursor;
    if( !usaExpresionRegular)
    {
        cursor = ventanaActiva()->document()->find(texto, ventanaActiva()->textCursor());
    }
    else
    {
        QRegExp expr(texto);
        if(ui->chkMayusMinusIgual->isChecked())
        {
            expr.setCaseSensitivity(Qt::CaseSensitive);
        }
        cursor = ventanaActiva()->document()->find(expr, ventanaActiva()->textCursor());
    }

    if( !cursor.isNull())
    {
        ventanaActiva()->setTextCursor(cursor);
    }
    else
    {
        if( !usaExpresionRegular )
        {
            cursor = ventanaActiva()->document()->find(texto, QTextCursor());
        }
        else
        {
            QRegExp expr(texto);
            if(ui->chkMayusMinusIgual->isChecked())
            {
                expr.setCaseSensitivity(Qt::CaseSensitive);
            }
            cursor = ventanaActiva()->document()->find(expr, QTextCursor() );
        }

        if(!cursor.isNull())
        {
            ventanaActiva()->setTextCursor(cursor);
        }
        else
        {
            ventanaActiva()->textCursor().clearSelection();
        }
    }

}

/*Proceso de Compilacion*/
void VentanaPrincipal::on_accionEjecutar_en_NXT_triggered()
{
    ui->barraSalida->setVisible(true);
    if(ventanaActiva()==0){return;}

    /*Agregar llamada al proceso de compilacion en NXT*/
    QString path = QDir::currentPath();
    //path += "/lejos/bin/nxjc.bat";
    path += "/compilacion_windows.bat";

    QString archivo = ventanaActiva()->obtenerNombreArchivo();
    QStringList argumentos = QStringList() << ventanaActiva()->obtenerFolder()
                                           << ventanaActiva()->obtenerNombreAmigable()
                                           << ventanaActiva()->obtenerNombreSinExtension();

    procesoCompilacion.start(path, argumentos);
    connect(&procesoCompilacion, SIGNAL(readyReadStandardOutput()), this, SLOT(salidaStandard()));
    connect(&procesoCompilacion, SIGNAL(readyReadStandardError()), this, SLOT(salidaStandardError()));
    connect(&procesoCompilacion, SIGNAL(finished(int)), this, SLOT(compilacionTerminada()));

    archivoAEjecutar = ventanaActiva()->obtenerNombreSinExtension();
    ui->statusBar->showMessage("Compilando "+archivo, 2000);
}

void VentanaPrincipal::compilacionTerminada()
{
    //ui->txtSalida->append("Instalacion Exitosa!");
}

void VentanaPrincipal::on_accionEjecutar_en_PC_triggered()
{
    ui->barraSalida->setVisible(true);
    /*Agregar llamada al proceso de compilacion en PC*/
}

void VentanaPrincipal::salidaStandard()
{
    ui->txtSalida->append(procesoCompilacion.readAllStandardOutput());
}

void VentanaPrincipal::salidaStandardError()
{
    ui->txtSalida->append(procesoCompilacion.readAllStandardError());
}

void VentanaPrincipal::on_accionPestania_triggered()
{
    ui->areaMDI->setViewMode(QMdiArea::TabbedView);
}

void VentanaPrincipal::on_accionCascada_triggered()
{
    ui->areaMDI->setViewMode(QMdiArea::SubWindowView);
}

void VentanaPrincipal::on_accionMaximizado_triggered()
{
    ui->areaMDI->setViewMode(QMdiArea::SubWindowView);
    if(ventanaActiva()!=0)
    {
        ventanaActiva()->setWindowState(Qt::WindowMaximized);
    }

}

QAbstractItemModel* VentanaPrincipal::obtenerModeloDeArchivo(QString nomArchivo, QCompleter *completador)
{
    QFile archivo(nomArchivo);
    if( !archivo.open(QFile::ReadOnly))
    {
        return new QStringListModel(completador);
    }

    QStringList palabras;
    while( !archivo.atEnd())
    {
        QByteArray linea = archivo.readLine();
        if( !linea.isEmpty())
        {
            // por si se me va un espacio :D
            palabras << linea.trimmed();
        }
    }
    return new QStringListModel(palabras, completador);
}
