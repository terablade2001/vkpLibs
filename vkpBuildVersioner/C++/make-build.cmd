set CURRP=%~dp0
cd ../BuildVersion
UpdateBuildVersion.exe
cd %CURRP%/build
make -j8
Example.exe
cd %CURRP%