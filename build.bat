@echo off
call "C:\Program Files\Microsoft Visual Studio\18\Insiders\VC\Auxiliary\Build\vcvars64.bat"
if not exist build mkdir build
cd build
if not exist Makefile cmake -G "NMake Makefiles" -DBUILD_SHARED_LIBS=ON -DENABLE_SDL=OFF -DENABLE_DECODER=OFF -DENABLE_ENCODER=OFF ..
nmake
pause
