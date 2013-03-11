#include "Editor.h"

Editor::Editor(QWidget *padre)
    :QPlainTextEdit(padre),
     completador(0)
{
    this->areaDeNumeroDeLinea = new AreaNumeroDeLinea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(actualizarAnchoDelAreaDeNumeroDeLinea(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(actualizarAreaDeNumeroDeLinea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(colorearLineaActual()));

    actualizarAnchoDelAreaDeNumeroDeLinea(0);
    colorearLineaActual();
    /*Tamaño del Tabulador*/
    //setTabStopWidth(20);

    QSettings configuraciones("ConfiguracionDeColoreado", QSettings::IniFormat);

    /*Letra, deberia venir de configuracion*/
    QFont f = configuraciones.value("Fuente",QFont("courier")).value<QFont>();
    int tamanio = configuraciones.value("FuenteTamanio",13).toInt();

    f.setPointSize(tamanio);
    setFont(f);

    QPalette p = palette();
    QColor colorSeleccion = configuraciones.value("ColorSeleccion",QColor(Qt::blue).lighter(160)).value<QColor>();
    p.setColor(QPalette::Highlight, colorSeleccion);
    p.setColor(QPalette::Background, Qt::gray);
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
    // Update Intellisense
    if(blockNumber > 0)
    {
        /*
        QTextBlock bloque = document()->findBlockByNumber(blockNumber-1);
        QString text = bloque.text();

        QStringList lista = text.split(" ");

        QStringListModel *model = (QStringListModel*)completador->model();
        QStringList list = model->stringList();

        list.append(lista);
        list.removeDuplicates();
        list.removeAll(" ");
        list.removeAll("");
        list.sort();

        model->setStringList(list);


        QStringListModel *nModel = new QStringListModel(list, completador);

        if( this->completador != 0)
        {
            this->completador->setModel(nModel);
        }*/
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

void Editor::focusInEvent(QFocusEvent *e)
{
    if( completador )
    {
        completador->setWidget(this);
    }
    QPlainTextEdit::focusInEvent(e);
}

void Editor::keyPressEvent(QKeyEvent *e)
{
    // Codigo para completador.!
    if( this->completador &&
        this->completador->popup()->isVisible())
    {
        switch(e->key())
        {
            case Qt::Key_Enter: case Qt::Key_Return:
            case Qt::Key_Escape: case Qt::Key_Tab:
            case Qt::Key_Backtab:
                e->ignore();
                return;
            default:
                break;
        }
    }

    // QSETTINEABLE -> Atajo para levantar el popup
    bool atajo = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space);

    if( !completador || !atajo)
    {
        // Ejecutar los demas
        if(e->key() == Qt::Key_Tab)
        {
            textCursor().insertText("    ");
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

    bool shiftCtrl = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if( !completador || ( shiftCtrl && e->text().isEmpty() ))
    {
        return;
    }

    QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-=");
    bool tieneModificador = (e->modifiers() != Qt::NoModifier) && !shiftCtrl;
    QString prefijoDeCompletacion = textoBajoCursor();

    /*Menos de 3 caracteres no auto completa*/
    /*QSETTINEABLE*/
    if (!atajo && (tieneModificador || e->text().isEmpty() || prefijoDeCompletacion.length() < 3
                      || eow.contains(e->text().right(1))))
    {
        completador->popup()->hide();
        return;
    }

    if( prefijoDeCompletacion != completador->completionPrefix() )
    {
        completador->setCompletionPrefix( prefijoDeCompletacion );
        completador->popup()->setCurrentIndex(completador->completionModel()->index(0,0));
    }

    QRect rectaCursor = cursorRect();
    rectaCursor.setWidth(completador->popup()->sizeHintForColumn(0) +
                         completador->popup()->verticalScrollBar()->sizeHint().width());
    completador->complete(rectaCursor);
    QFont f = completador->popup()->font();
    int valor = 12 * this->zoomFont / 100;
    f.setPointSize( valor );
    completador->popup()->setFont(f);
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

QString Editor::obtenerNombreSinExtension()
{
    QString str = this->nombreArchivo.mid(0, nombreArchivo.lastIndexOf("."));
    return str.mid(str.lastIndexOf("/")+1,str.length());
}

QString Editor::obtenerFolder()
{
    return this->nombreArchivo.mid(0, nombreArchivo.lastIndexOf("/"));
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

void Editor::establecerCompletador(QCompleter *comp)
{
    if(completador)
    {
        QObject::disconnect(completador, 0, this, 0);
    }

    this->completador = comp;
    if( !completador )
    {
        return;
    }

    this->completador->setWidget(this);
    // QSetting
    this->completador->setCompletionMode(QCompleter::PopupCompletion);
    this->completador->setCaseSensitivity(Qt::CaseInsensitive);
    this->completador->connect(completador, SIGNAL(activated(QString)),
                               this, SLOT(insertarCompletacion(QString)));
}

QCompleter *Editor::obtenerCompletador()
{
    return completador;
}

void Editor::insertarCompletacion(QString completacion)
{
    if( completador->widget() != this)
    {
        return;
    }
    QTextCursor cursor = textCursor();
    int extra = completacion.length() - completador->completionPrefix().length();
    cursor.movePosition(QTextCursor::Left);
    cursor.movePosition(QTextCursor::EndOfWord);
    cursor.insertText(completacion.right(extra));
    setTextCursor(cursor);
}

QString Editor::textoBajoCursor()
{
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    return cursor.selectedText();
}

bool Editor::tieneTitulo()
{
    return this->esSinTitulo;
}
