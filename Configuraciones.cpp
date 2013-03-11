#include "Configuraciones.h"
#include "ui_Configuraciones.h"

Configuraciones::Configuraciones(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configuraciones)
{
    ui->setupUi(this);
    ui->listaOpciones->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listaOpciones->setViewMode(QListWidget::IconMode);
    ui->listaOpciones->setIconSize(QSize(64,64));
    ui->listaOpciones->setSpacing(20);
    ui->panelPrincipal->setCurrentIndex(0);

    agregarElementosAListaColoreado();
    cargarConfiguraciones();
    cargarFuncionesXml();
}

Configuraciones::~Configuraciones()
{
    delete ui;
}

void Configuraciones::agregarElementosAListaColoreado()
{
    /*Text, NumeroDelinea, AreaDeNumeroDeLinea, Parentesis, LineaActual, Numeros, Cadenas
      TiposDeDato, Comentarios */
    QPalette pal = ui->listaColoreado->palette();
    pal.setColor(QPalette::Highlight, Qt::transparent);
    ui->listaColoreado->setPalette(pal);

    QSettings configuraciones("ConfiguracionDeColoreado", QSettings::IniFormat);

    QListWidgetItem *elementoTexto = new QListWidgetItem("Texto");
    elementoTexto->setForeground( configuraciones.value("ColorTextoPalabraReservada",Qt::darkBlue).value<QBrush>());

    QListWidgetItem *elementoNumeroDeLinea = new QListWidgetItem("Numero de linea");

    QListWidgetItem *elementoAreaDeNumeroDeLinea = new QListWidgetItem("Area de numero de linea");

    QListWidgetItem *elementoParentesis = new QListWidgetItem("Parentesis");

    QListWidgetItem *elementoLineaActual = new QListWidgetItem("Linea actual");
    elementoLineaActual->setBackground(configuraciones.value("ColorLineaActual",Qt::cyan).value<QBrush>());

    QListWidgetItem *elementoNumeros = new QListWidgetItem("Numeros");

    QListWidgetItem *elementoCadenas = new QListWidgetItem("Cadenas");

    QListWidgetItem *elementoTiposDeDato = new QListWidgetItem("Tipos de dato");

    QListWidgetItem *elementoComentarios = new QListWidgetItem("Comentarios");
    elementoComentarios->setForeground(configuraciones.value("ColorComentario", Qt::darkGreen).value<QBrush>());

    QListWidgetItem *elementoSeleccion = new QListWidgetItem("Seleccion de Texto");
    elementoSeleccion->setBackground(configuraciones.value("ColorSeleccion",QColor(Qt::blue).lighter(160)).value<QColor>());

    ui->listaColoreado->addItem(elementoTexto);
    ui->listaColoreado->addItem(elementoNumeroDeLinea);
    ui->listaColoreado->addItem(elementoAreaDeNumeroDeLinea);
    ui->listaColoreado->addItem(elementoParentesis);
    ui->listaColoreado->addItem(elementoLineaActual);
    ui->listaColoreado->addItem(elementoNumeros);
    ui->listaColoreado->addItem(elementoCadenas);
    ui->listaColoreado->addItem(elementoTiposDeDato);
    ui->listaColoreado->addItem(elementoComentarios);
    ui->listaColoreado->addItem(elementoSeleccion);
}

void Configuraciones::on_listaOpciones_itemSelectionChanged()
{
    ui->panelPrincipal->setCurrentIndex(ui->listaOpciones->row(ui->listaOpciones->selectedItems().at(0)));
}

void Configuraciones::on_chkVentanaDeBienvenida_stateChanged(int arg1)
{
}

void Configuraciones::on_btnCancelar_clicked()
{
    this->close();
}

void Configuraciones::on_btnAceptar_clicked()
{
    aplicarConfiguraciones();
    this->close();
}

void Configuraciones::aplicarConfiguraciones()
{
    QSettings configuraciones("ArchivoConfiguracion.ini",QSettings::IniFormat);

    // Generales
    configuraciones.setValue("AbrirBienvenida", ui->chkVentanaDeBienvenida->isChecked());
    configuraciones.setValue("VistaPestania", ui->chkVistaEnPestania->isChecked());
    configuraciones.setValue("AbrirBienvenidaAlCerrar", ui->chkVentanaBienvenidaAlCerrar->isChecked());
    configuraciones.sync();

    // Editor de Texto
    QSettings configuracionesEditor("ConfiguracionDeColoreado", QSettings::IniFormat);
    configuracionesEditor.setValue("Fuente",ui->cmbFuente->currentFont());
    configuracionesEditor.setValue("FuenteTamanio",ui->cmbTamanio->currentText().toInt());

        // Todos los colores

    configuracionesEditor.sync();

    // Funciones auxiliares

    configuraciones.sync();
}

void Configuraciones::on_listaColoreado_itemClicked(QListWidgetItem *item)
{
    QPalette pBack = ui->btnBackground->palette();

    if( item->backgroundColor().spec()== QColor::Invalid)
    {
        pBack.setColor(ui->btnBackground->backgroundRole(), Qt::white);
    }
    else
    {
        pBack.setColor(ui->btnBackground->backgroundRole(), item->backgroundColor());
    }
    ui->btnBackground->setPalette(pBack);

    QPalette pFore = ui->btnForeground->palette();
    pFore.setColor(ui->btnForeground->backgroundRole(), item->foreground().color() );
    ui->btnForeground->setPalette(pFore);
}

void Configuraciones::on_btnForeground_clicked()
{
    QColor color = obtenerColor();
    if( color != Qt::transparent)
    {
        if( ui->listaColoreado->selectedItems().count() == 0 ){return;}
        ui->listaColoreado->selectedItems().at(0)->setForeground(color);
    }
}

void Configuraciones::on_btnBackground_clicked()
{
    QColor color = obtenerColor();
    if( color != Qt::transparent)
    {
        if( ui->listaColoreado->selectedItems().count() == 0 ){return;}
        ui->listaColoreado->selectedItems().at(0)->setBackground(color);
    }
}

QColor Configuraciones::obtenerColor()
{
    QColorDialog *dialogo = new QColorDialog(this);
    return dialogo->getColor(Qt::transparent,this, "Selecciona un color");
}

void Configuraciones::cargarConfiguraciones()
{
    QSettings configuraciones("ArchivoConfiguracion.ini",QSettings::IniFormat);
    bool AbrirBievenida = configuraciones.value("AbrirBienvenida", true).value<bool>();
    ui->chkVentanaDeBienvenida->setChecked(AbrirBievenida);

    bool AbrirAlCerrar = configuraciones.value("AbrirBienvenidaAlCerrar",false).toBool();
    ui->chkVentanaBienvenidaAlCerrar->setChecked(AbrirAlCerrar);

    bool MostrarPestania = configuraciones.value("VistaPestania",false).toBool();
    ui->chkVistaEnPestania->setChecked(MostrarPestania);

    QSettings configuracionesEditor("ConfiguracionDeColoreado", QSettings::IniFormat);
    QFont fuente = configuracionesEditor.value("Fuente",QFont("courier")).value<QFont>();
    ui->cmbFuente->setCurrentFont(fuente);

    ui->cmbTamanio->addItem(QString::number(fuente.pointSize()));
    ui->cmbTamanio->setCurrentIndex(ui->cmbTamanio->count()-1);
}

void Configuraciones::on_btnCargarArchivo_clicked()
{
    QString archivo = QFileDialog::getOpenFileName(this, "Cargar Archivo XML",
                                                   QDir::homePath(), tr("Archivos XML(*.xml)"));
    ui->lnArchivoXml->setText(archivo);
}



void Configuraciones::on_btnAgregarXml_clicked()
{
    // Copiar archivo a carpeta mi carpeta xml
    QFile file(ui->lnArchivoXml->text());
    if( !file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QString f = QFileInfo(file.fileName()).fileName();
    file.copy("xml/"+f);
    file.close();

    cargarFuncionesXml();
}

void Configuraciones::cargarFuncionesXml()
{
    ui->archivosXML->clear();
    ui->archivosXML->setColumnCount(1);

    QDir *dir = new QDir("xml/");
    QFileInfoList listaArchivos = dir->entryInfoList();

    QTreeWidgetItem* encabezado = new QTreeWidgetItem();
    encabezado->setText(0,QString("Nombre del Archivo"));
    ui->archivosXML->setHeaderItem(encabezado);
    foreach(QFileInfo info, listaArchivos)
    {
        QTreeWidgetItem* elemento = new QTreeWidgetItem();
        elemento->setText(0,info.fileName());
        if( info.isFile() && info.fileName() != "." && info.fileName() != "..")
        {
            elemento->setIcon(0, QIcon(":/xml.png"));
            elemento->setToolTip(0, info.filePath());
            ui->archivosXML->addTopLevelItem(elemento);
        }
    }
}

void Configuraciones::on_archivosXML_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    // Abrirlo, aun no se donde.!
}
