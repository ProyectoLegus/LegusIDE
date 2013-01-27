#ifndef EDITOR_H
#define EDITOR_H

#include <QtGui>
#include <QPlainTextEdit>
#include <QObject>

class Editor : public QPlainTextEdit
{
    Q_OBJECT

public:
    Editor(QWidget *padre = 0);

    void areaNumeroDeLineaPaintEvent(QPaintEvent *evento);
    int  anchoDelAreaNumeroDeLinea();

    void nuevoArchivo();
    bool cargarArchivo(QString archivo);
    bool guardar();
    bool guardarComo();
    bool guardarArchivo(QString archivo);

    QString obtenerNombreArchivo();
    QString obtenerNombreAmigable();

protected:
    void resizeEvent(QResizeEvent *e);
    void closeEvent(QCloseEvent *);
    void keyPressEvent(QKeyEvent *e);

private slots:
    void actualizarAnchoDelAreaDeNumeroDeLinea(int cantidadBloque);
    void colorearLineaActual();
    void actualizarAreaDeNumeroDeLinea(const QRect &rect, int dy);
    void documentoModificado();

private:
    QWidget *areaDeNumeroDeLinea;
    QString nombreArchivo;
    bool esSinTitulo;

    void establecerArchivoActual(QString archivo);
    bool puedoGuardar();
    QString obtenerNombreArchivoCompleto(QString archivo);

};

class AreaNumeroDeLinea : public QWidget
{
public:
    AreaNumeroDeLinea(Editor *editor) : QWidget(editor)
    {
        this->editor = editor;
    }

    QSize sizeHint() const
    {
        return QSize(editor->anchoDelAreaNumeroDeLinea(), 0);
    }

protected:
    void paintEvent(QPaintEvent *evento)
    {
        editor->areaNumeroDeLineaPaintEvent(evento);
    }

private:
    Editor *editor;
};

#endif // EDITOR_H
