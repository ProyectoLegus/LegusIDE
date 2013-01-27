#include "ColoreadoDecodigoJava.h"

ColoreadoDeCodigoJava::ColoreadoDeCodigoJava(QTextDocument *padre)
    :QSyntaxHighlighter(padre)
{

    ReglaDeColoreado regla;

    /*Esto deberia ser jalado de un archivo de configuracion*/
    formatoPalabraReservada.setForeground(Qt::darkBlue);
    formatoPalabraReservada.setFontWeight(QFont::Bold);

    QStringList patronesDePalabrasReservadas;

    patronesDePalabrasReservadas << "\\bchar\\b" << "\\int\\b" << "\\bString\\b" << "\\bboolean\\b"
                                 << "\\bdouble\\b" << "\\bif\\b" << "\\bfor\\b" << "\\bwhile\\b"
                                 << "\\bpublic\\b" << "\\bvoid\\b" << "\\bstatic\\b" << "\\bprivate\\b"
                                 << "\\breturn\\b" << "\\bswitch\\b" << "\\bcase\\b" << "\\belse\\b"
                                 << "\\bdo\\b" << "\\bimport\\b" ;

    foreach(const QString &patron, patronesDePalabrasReservadas)
    {
        regla.patron = QRegExp(patron,Qt::CaseInsensitive);
        regla.formato = formatoPalabraReservada;
        reglasDeColoreado.append(regla);
    }

    //Comentario
    formatoComentarioMultilinea.setForeground(Qt::darkGreen);

    inicioDeExpresionComentario = QRegExp("/\\*");
    finDeExpresionComentario = QRegExp("\\*/");
}

void ColoreadoDeCodigoJava::highlightBlock(const QString &texto)
{
    foreach(const ReglaDeColoreado &regla, reglasDeColoreado)
    {
        QRegExp expresion(regla.patron);
        int indice = expresion.indexIn(texto);
        while( indice >= 0)
        {
            int longitud = expresion.matchedLength();
            setFormat(indice, longitud, regla.formato);
            indice = expresion.indexIn(texto, indice + longitud);
        }
    }

    setCurrentBlockState(0);
    int indiceInicio = 0;

    if( previousBlockState() != 1)
    {
        indiceInicio = inicioDeExpresionComentario.indexIn(texto);
    }

    while( indiceInicio >= 0)
    {
        int indiceFinal = finDeExpresionComentario.indexIn(texto, indiceInicio);
        int longitudComentario;
        if( indiceFinal == -1)
        {
            setCurrentBlockState(1);
            longitudComentario = texto.length() - indiceInicio;
        }
        else
        {
            longitudComentario = indiceFinal - indiceInicio + finDeExpresionComentario.matchedLength();
        }

        setFormat(indiceInicio, longitudComentario, formatoComentarioMultilinea);
        indiceInicio = inicioDeExpresionComentario.indexIn(texto, indiceInicio + longitudComentario);
    }
}
