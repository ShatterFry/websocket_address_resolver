open the "Developer Command Prompt" from the Visual Studio toolchain
cl /c main.cpp
link main.obj Ws2_32.lib
main.exe address port