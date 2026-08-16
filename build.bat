@echo off
set "PATH=C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.11.1\mingw_64\bin;%PATH%"

qmake QTGUI.pro
mingw32-make

pause