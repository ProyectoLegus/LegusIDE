#include "Configuraciones.h"
#include "ui_Configuraciones.h"

Configuraciones::Configuraciones(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configuraciones)
{
    ui->setupUi(this);
    ui->listaOpciones->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->panelPrincipal->setCurrentIndex(0);

    agregarElementosAListaColoreado();

    QSettings configuraciones("ArchivoConfiguracion.ini",QSettings::IniFormat);

    bool AbrirBievenida = configuraciones.value("AbrirBienvenida", true).value<bool>();
    ui->chkVentanaDeBienvenida->setChecked(AbrirBievenida);
}

Configuraciones::~Configuraciones()
{
    delete ui;
}

void Configuraciones::agregarElementosAListaColoreado()
{
    /*Text, NumeroDelinea, AreaDeNumeroDeLinea, Parentesis, LineaActual, Numeros, Cadenas
      TiposDeDato, Comentarios */
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

void Configuraciones::on_btnAplicar_clicked()
{
    aplicarConfiguraciones();
    /*Hacer fresa! Que se deshabilite el boton*/
}

void Configuraciones::aplicarConfiguraciones()
{
    QSettings configuraciones("ArchivoConfiguracion.ini",QSettings::IniFormat);
    configuraciones.setValue("AbrirBienvenida", ui->chkVentanaDeBienvenida->isChecked());
    configuraciones.sync();

}

void Configuraciones::on_checkBox_clicked()
{

}

void Configuraciones::on_listaColoreado_itemClicked(QListWidgetItem *item)
{
    QPalette pBack = ui->btnBackground->palette();
    pBack.setColor(ui->btnBackground->backgroundRole(), item->backgroundColor());
    ui->btnBackground->setPalette(pBack);

    QPalette pFore = ui->btnForeground->palette();
    pFore.setColor(ui->btnForeground->foregroundRole(), item->foreground().color() );
    ui->btnForeground->setPalette(pFore);
}
