@echo off
set CURRP=%~dp0
set BASEP="%CURRP%..\.."
cls
cd "%BASEP%\C++\build"
make
move UpdateBuildVersion.exe "%BASEP%\"
cd "%CURRP%"
