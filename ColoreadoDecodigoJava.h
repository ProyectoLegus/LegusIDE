#ifndef COLOREADODECODIGOJAVA_H
#define COLOREADODECODIGOJAVA_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
    class QTextDocument;
QT_END_NAMESPACE


class ColoreadoDeCodigoJava : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    ColoreadoDeCodigoJava(QTextDocument *padre = 0);

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

#endif // COLOREADODECODIGOJAVA_H
