@echo off
echo Hello, World!

rem call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
cl

pushd %~dp0
del "main.obj"
del "GameName.obj"
cl /c /Fo: ".\bin\x64\GameName.obj" /std:c++20 "src\cpp\main.cpp"
link /OUT:".\bin\x64\GameName.exe" ".\bin\x64\GameName.obj" "Ws2_32.lib"
popd

pause
