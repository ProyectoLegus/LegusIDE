#include "Editor.h"

Editor::Editor(QWidget *padre)
    :QPlainTextEdit(padre)
{
    this->areaDeNumeroDeLinea = new AreaNumeroDeLinea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(actualizarAnchoDelAreaDeNumeroDeLinea(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(actualizarAreaDeNumeroDeLinea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(colorearLineaActual()));

    actualizarAnchoDelAreaDeNumeroDeLinea(0);
    colorearLineaActual();

    /*Tamaño del Tabulador*/
    setTabStopWidth(20);

    /*Letra, deberia venir de configuracion*/
    setFont(QFont("courier",13));

    QSettings configuraciones("ConfiguracionDeColoreado", QSettings::IniFormat);
    QPalette p = palette();
    QColor colorSeleccion = configuraciones.value("ColorSeleccion",QColor(Qt::blue).lighter(160)).value<QColor>();
    p.setColor(QPalette::Highlight, colorSeleccion);
    setPalette(p);

    this->maxZoom = 500;
    this->minZoom = 70;
    this->zoomFont = 100;
}

int Editor::anchoDelAreaNumeroDeLinea()
{
    int digitos = 1;
    int maximo = qMax(1, blockCount());
    while( maximo >= 10)
    {
        maximo = maximo / 10;
        ++digitos;
    }
    int espacio = 3 + fontMetrics().width(QLatin1Char('9')) * digitos;
    return espacio;
}

void Editor::actualizarAnchoDelAreaDeNumeroDeLinea(int cantidadBloque)
{
    setViewportMargins(anchoDelAreaNumeroDeLinea(), 0, 0, 0);
}

void Editor::actualizarAreaDeNumeroDeLinea(const QRect &rect, int dy)
{
    if( dy )
    {
        areaDeNumeroDeLinea->scroll(0, dy);
    }
    else
    {
        areaDeNumeroDeLinea->update(0, rect.y(), areaDeNumeroDeLinea->width(), rect.height());
    }

    if( rect.contains(viewport()->rect()))
    {
        actualizarAnchoDelAreaDeNumeroDeLinea(0);
    }
}

void Editor::colorearLineaActual()
{
    QSettings configuraciones("ConfiguracionDeColoreado", QSettings::IniFormat);
    QList<QTextEdit::ExtraSelection> seleccionesExtra;

    if( !isReadOnly() )
    {
        QTextEdit::ExtraSelection seleccion;

        /*Desde QtSettings*/
        QBrush colorLinea = configuraciones.value("ColorLineaActual",Qt::cyan).value<QBrush>();
        QColor colorDeLinea = colorLinea.color().lighter(190);
        seleccion.format.setBackground(colorDeLinea);
        seleccion.format.setProperty(QTextFormat::FullWidthSelection, true);
        seleccion.cursor = textCursor();
        seleccion.cursor.clearSelection();
        seleccionesExtra.append(seleccion);
    }
    setExtraSelections(seleccionesExtra);
}

void Editor::areaNumeroDeLineaPaintEvent(QPaintEvent *evento)
{    
    QSettings configuraciones("ConfiguracionDeColoreado", QSettings::IniFormat);

    QPainter painter(areaDeNumeroDeLinea);

    QBrush pincelArea = configuraciones.value("ColorAreaNumeroDeLinea", Qt::lightGray).value<QBrush>();
    QColor colorArea = pincelArea.color().lighter(120);
    painter.fillRect(evento->rect(), colorArea);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    /*Color del Numero!*/
    QBrush pincelTexto = configuraciones.value("ColorNumeroDeLinea",Qt::black).value<QBrush>();
    QColor colorTexto = pincelTexto.color();

    while (block.isValid() && top <= evento->rect().bottom())
    {
        if (block.isVisible() && bottom >= evento->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(colorTexto);
            painter.drawText(0, top, areaDeNumeroDeLinea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void Editor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();

    areaDeNumeroDeLinea->setGeometry(QRect(cr.left(), cr.top(), anchoDelAreaNumeroDeLinea(), cr.height()));
}

void Editor::closeEvent(QCloseEvent *evento)
{
    if( puedoGuardar() )
    {
        evento->accept();
    }
    else
    {
        evento->ignore();
    }
}

void Editor::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Tab)
    {
        textCursor().insertText("    ");
        /*Codigo para tabular texto seleccionado*/
        /*
        QString textoSeleccionado = textCursor().selectedText();
        if(!textoSeleccionado.isEmpty())
        {
            QStringList listaLineas = textoSeleccionado.split("\n");
            QString textoFinal = "";
            foreach( QString linea, listaLineas)
            {
                textoFinal += "\t";
                textoFinal += linea;
                textoFinal += "\n";
            }
            textCursor().insertText(textoFinal);

        }
        */
        //QPlainTextEdit::keyPressEvent(e);
    }
    else if( e->key() == Qt::Key_W && (e->modifiers() & Qt::ControlModifier==0))
    {
        QMessageBox::about(0,"","A punto de cerrar");
        QPlainTextEdit::keyPressEvent(e);
    }
    else
    {
        QPlainTextEdit::keyPressEvent(e);
    }
}

void Editor::keyReleaseEvent(QKeyEvent *e)
{
    /*Codigo para agregar ), " despues de ser digitados ( y "*/
    /*if( e->key() == Qt::Key_ParenLeft)
    {
        int pos = textCursor().position();
        textCursor().insertText(")");
        textCursor().setPosition(pos);
        e->accept();
    }

    if( e->key() == Qt::Key_QuoteDbl)
    {
        textCursor().insertText("\"");
    }*/

}

bool Editor::cargarArchivo(QString archivoACargar)
{
    QFile archivo( archivoACargar );

    if( !archivo.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(this, "Error", "No se pudo leer el archivo");
        return false;
    }

    QTextStream flujo(&archivo);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    setPlainText(flujo.readAll());
    QApplication::restoreOverrideCursor();

    establecerArchivoActual(archivoACargar);
    connect(document(), SIGNAL(contentsChanged()), this, SLOT(documentoModificado()));

    return true;
}

bool Editor::guardar()
{
    if( esSinTitulo )
    {
        return guardarComo();
    }
    else
    {
        return guardarArchivo(this->nombreArchivo);
    }
}

bool Editor::guardarComo()
{
    QString nombreDelArchivo = QFileDialog::getSaveFileName(this, "Guardar Como", QDir::homePath(),
                                                            tr("Archivos Legus (*.legus);; Archivo Java (*.java)"));

    if( nombreDelArchivo.isEmpty() )
    {
        return false;
    }

    return guardarArchivo(nombreDelArchivo);
}

bool Editor::guardarArchivo(QString archivoACargar)
{
    QFile archivo(archivoACargar);
    if( !archivo.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::critical(this,"Error", "No se puede salvar este archivo");
        return false;
    }

    QTextStream flujo(&archivo);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    flujo << toPlainText();
    QApplication::restoreOverrideCursor();

    establecerArchivoActual(archivoACargar);
    return true;
}

QString Editor::obtenerNombreAmigable()
{
    return obtenerNombreArchivoCompleto(this->nombreArchivo);
}

QString Editor::obtenerNombreArchivoCompleto(QString archivo)
{
    return QFileInfo(archivo).fileName();
}

void Editor::documentoModificado()
{
    setWindowModified(document()->isModified());
}

void Editor::establecerArchivoActual(QString archivo)
{
    this->nombreArchivo = QFileInfo(archivo).canonicalFilePath();
    esSinTitulo = false;
    document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(obtenerNombreAmigable()+" [*]");
}

bool Editor::puedoGuardar()
{
    if( document()->isModified())
    {
        QMessageBox::StandardButton ret;
                ret = QMessageBox::warning(this,"Guardar",
                             tr("¿Quieres guardar los cambios?"),
                             QMessageBox::Save | QMessageBox::Discard
                             | QMessageBox::Cancel);
                if (ret == QMessageBox::Save)
                    return guardar();
                else if( ret == QMessageBox::Discard)
                    return true;
                else if (ret == QMessageBox::Cancel)
                    return false;
    }
    return true;
}

QString Editor::obtenerNombreArchivo()
{
    return this->nombreArchivo;
}

void Editor::nuevoArchivo()
{
    this->esSinTitulo = true;
    this->nombreArchivo = "[Sin Nombre]";
    setWindowTitle(nombreArchivo+" [*]");

    connect(document(), SIGNAL(contentsChanged()),
            this, SLOT(documentoModificado()));
}

/*ZOOM*/
int Editor::obtenerZoomFont()
{
    return this->zoomFont;
}

void Editor::establecerZoomFont(int zoomFont)
{
    if( zoomFont != this->zoomFont )
    {
        this->zoomFont = zoomFont;
        QFont f = this->font();
        int valor = 12 * this->zoomFont / 100;
        f.setPointSize( valor );
        setFont(f);
        emit cambioZoomFont(this->zoomFont);
    }
}

void Editor::zoomAdentro()
{
    int zoom = this->zoomFont + 10;
    if( zoom > this->maxZoom )
    {
        zoom = this->maxZoom;
    }
    establecerZoomFont(zoom);
}

void Editor::zoomAfuera()
{
    int zoom = this->zoomFont - 10;
    if( zoom < this->minZoom )
    {
        zoom = this->minZoom;
    }
    establecerZoomFont(zoom);
}

void Editor::zoomReinicializar()
{
    establecerZoomFont(100);
}
