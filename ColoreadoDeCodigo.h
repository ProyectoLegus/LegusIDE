#ifndef RESALTADORDECODIGO_H
#define RESALTADORDECODIGO_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
    class QTextDocument;
QT_END_NAMESPACE


class ColoreadoDeCodigo : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    ColoreadoDeCodigo( QTextDocument *padre = 0 );

protected:
    void highlightBlock(const QString &texto);

private:
    struct ReglaDeColoreado
    {
        QRegExp         patron;
        QTextCharFormat formato;
    };
    QVector<ReglaDeColoreado> reglasDeColoreado;

    QRegExp inicioDeExpresionComentario;
    QRegExp finDeExpresionComentario;

    QTextCharFormat formatoPalabraReservada;
    QTextCharFormat formatoComentarioMultilinea;
    QTextCharFormat formatoFunciones;
    QTextCharFormat formatoCadenas;
};

#endif // RESALTADORDECODIGO_H
