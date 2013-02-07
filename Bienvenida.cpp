#include "Bienvenida.h"
#include "ui_Bienvenida.h"

Bienvenida::Bienvenida(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Bienvenida)
{
    ui->setupUi(this);
    qApp->setStyle(new QCleanlooksStyle());

    cargarConfiguraciones();
}

Bienvenida::~Bienvenida()
{
    delete ui;
}

void Bienvenida::on_btnCrearArchivo_clicked()
{
    (new VentanaPrincipal())->show();
    this->close();
}

void Bienvenida::cargarConfiguraciones()
{
    /*Configuraciones*/
    ui->btnAbrirArchivo->setEnabled(false);
    ui->listaArchivosRecientes->setSelectionMode(QListWidget::SingleSelection);
    ui->listaArchivosRecientes->setMovement(QListWidget::Static);
    ui->listaArchivosRecientes->setIconSize(QSize(32,32));

    QSettings configuraciones("ArchivoConfiguracion.ini",QSettings::IniFormat);
    QStringList archivosRecientes = configuraciones.value("ArchivosRecientes").toStringList();

    for(int i=0; i<archivosRecientes.size(); i++)
    {
        QListWidgetItem *elemento = new QListWidgetItem();
        QString nombreArchivo = QFileInfo(archivosRecientes.at(i)).fileName();
        elemento->setData(0, nombreArchivo);
        elemento->setToolTip(archivosRecientes.at(i));

        QRegExp rx("*.java");
        rx.setPatternSyntax(QRegExp::Wildcard);
        if(!nombreArchivo.contains(rx))
        {
            elemento->setIcon(QIcon(":/abrir_documento.png"));
        }
        else
        {
            elemento->setIcon(QIcon(":/abrir_documento_java.png"));
        }
        ui->listaArchivosRecientes->addItem(elemento);
    }

    bool AbrirBievenida = configuraciones.value("AbrirBienvenida", true).value<bool>();
    ui->chkAbrirVentana->setChecked(AbrirBievenida);
}

void Bienvenida::on_listaArchivosRecientes_doubleClicked(const QModelIndex &index)
{
    abrirArchivoSeleccionado();
}

void Bienvenida::on_btnAbrirArchivo_clicked()
{
    abrirArchivoSeleccionado();
}

void Bienvenida::abrirArchivoSeleccionado()
{
    QList<QListWidgetItem*> elementos = ui->listaArchivosRecientes->selectedItems();
    if( elementos.size() > 0)
    {
        (new VentanaPrincipal(elementos.at(0)->toolTip()))->show();
        this->close();
    }
}

void Bienvenida::on_listaArchivosRecientes_itemClicked(QListWidgetItem *item)
{
    ui->btnAbrirArchivo->setEnabled(true);
}

void Bienvenida::on_chkAbrirVentana_stateChanged(int arg1)
{
    QSettings configuraciones("ArchivoConfiguracion.ini",QSettings::IniFormat);
    configuraciones.setValue("AbrirBienvenida", ui->chkAbrirVentana->isChecked());
    configuraciones.sync();
}

void Bienvenida::on_btnTutorialesLegus_clicked()
{
    QDesktopServices::openUrl(QUrl("www.youtube.com/user/ProyectoLegus"));
}

void Bienvenida::on_btnAbrirOtroArchivo_clicked()
{
    QString archivo = QFileDialog::getOpenFileName(this, "Abrir Archivo",
                                                   QDir::homePath(), tr("Archivos de Legus (*.legus *.java)"));

    if( archivo.isEmpty() )
    {
        QMessageBox::critical(this,"Error", "Error al abrir el archivo");
        return;
    }

    (new VentanaPrincipal( archivo ))->show();
    this->close();
}
