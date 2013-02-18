#include "ColoreadoDeCodigo.h"

ColoreadoDeCodigo::ColoreadoDeCodigo(QTextDocument *padre)
    :QSyntaxHighlighter(padre)
{

    ReglaDeColoreado regla;

    QSettings configuraciones("ConfiguracionDeColoreado", QSettings::IniFormat);

    /*Esto deberia ser jalado de un archivo de configuracion*/
    QBrush colorTextoPalabraReservada = configuraciones.value("ColorTextoPalabraReservada",Qt::darkBlue).value<QBrush>();
    int estiloPalabraReservada= configuraciones.value("EstiloPalabraReservada",QFont::Bold).value<int>();

    formatoPalabraReservada.setForeground(colorTextoPalabraReservada);
    formatoPalabraReservada.setFontWeight(estiloPalabraReservada);

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
    QBrush brush = configuraciones.value("ColorComentario", Qt::darkGreen).value<QBrush>();
    formatoComentarioMultilinea.setForeground(brush);
    inicioDeExpresionComentario = QRegExp("/\\*");
    finDeExpresionComentario = QRegExp("\\*/");

    // Numeros
    QBrush brushNumeros = configuraciones.value("ColorNumero",Qt::darkRed).value<QBrush>();
    formatoNumeros.setForeground(brushNumeros);
    regla.patron = QRegExp("\\b[0-9]+");
    regla.formato = formatoNumeros;
    reglasDeColoreado.append(regla);

    // Cadenas
    QBrush brushCadenas = configuraciones.value("ColorCadena",Qt::darkGreen).value<QBrush>();
    formatoCadenas.setForeground(brushCadenas);
    regla.patron = QRegExp("\".*\"");
    regla.formato = formatoCadenas;
    reglasDeColoreado.append(regla);

    // Tipos de Dato


    // Parentesis y Corchetes
    QBrush brushParenCor = configuraciones.value("ColorParentesisCorchete",Qt::darkRed).value<QBrush>();
    formatoParentesisCorchetes.setForeground(brushParenCor);
    formatoParentesisCorchetes.setFontWeight(estiloPalabraReservada);
    QStringList ParenCor;
    ParenCor << "\\b(\\b" << "\\b)\\b"
             <<"\\b[\\b"<<"\\b]\\b";
    foreach(QString patron, ParenCor)
    {
        regla.formato = formatoParentesisCorchetes;
        regla.patron = QRegExp(patron);
        reglasDeColoreado.append(regla);
    }

    // Funciones
    QBrush brushFunciones = configuraciones.value("ColorFunciones", Qt::darkMagenta).value<QBrush>();
    formatoFunciones.setForeground(brushFunciones);
    formatoFunciones.setFontWeight(estiloPalabraReservada);
    regla.formato = formatoFunciones;
    regla.patron = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    reglasDeColoreado.append(regla);
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
