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
    QList<QTextEdit::ExtraSelection> seleccionesExtra;

    if( !isReadOnly() )
    {
        QTextEdit::ExtraSelection seleccion;

        /*Desde QtSettings*/
        QColor colorDeLinea = QColor(Qt::cyan).lighter(190);

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
    QPainter painter(areaDeNumeroDeLinea);
    painter.fillRect(evento->rect(), QColor(Qt::lightGray).lighter(120));

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
    while (block.isValid() && top <= evento->rect().bottom())
    {
        if (block.isVisible() && bottom >= evento->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
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
        QPlainTextEdit::keyPressEvent(e);
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
