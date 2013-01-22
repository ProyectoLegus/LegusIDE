#include "ColoreadoDeCodigo.h"

ColoreadoDeCodigo::ColoreadoDeCodigo(QTextDocument *padre)
    :QSyntaxHighlighter(padre)
{

    ReglaDeColoreado regla;

    /*Esto deberia ser jalado de un archivo de configuracion*/
    formatoPalabraReservada.setForeground(Qt::darkBlue);
    formatoPalabraReservada.setFontWeight(QFont::Bold);

    QStringList patronesDePalabrasReservadas;

    patronesDePalabrasReservadas << "\\bmod\\b" << "\\div\\b" << "\\bigual\\b" << "\\ba\\b"
                                 << "\\bdistinto\\b" << "\\bde\\b" << "\\by\\b" << "\\bo\\b"
                                 << "\\bno\\b" << "\\bsi\\b" << "\\bentonces\\b" << "\\bfin\\b"
                                 << "\\bsino\\b" << "\\bcaso\\b" << "\\bmientras\\b" << "\\bhaga\\b"
                                 << "\\bejecute\\b" << "\\brepetir\\b" << "\\binfinitas\\b"
                                 << "\\bveces\\b" << "\\bpara\\b" << "\\bdesde\\b" << "\\brepita\\b"
                                 << "\\bcada\\b" << "\\ben\\b" << "\\bfuncion\\b" << "\\bretorna\\b"
                                 << "\\bretornar\\b" << "\\butilizar\\b" << "\\bcomo\\b"
                                 << "\\bdefinir\\b" << "\\bverdadero\\b" << "\\bfalso\\b"
                                 << "\\bhasta\\b" ;

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

void ColoreadoDeCodigo::highlightBlock(const QString &texto)
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
