set DIRECTORIO=%CD%
cd /d %1
cmd /C %DIRECTORIO%\lejos\bin\nxjc.bat %1\%2
cmd /C %DIRECTORIO%\lejos\bin\nxj.bat -u %3