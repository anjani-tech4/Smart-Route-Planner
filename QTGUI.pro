QT += widgets

CONFIG += c++17

INCLUDEPATH += src

SOURCES += \
    src/main.cpp \
    src/dijkstra.cpp \
    src/gui.cpp

HEADERS += \
    src/dijkstra.h \
    src/pq.h \
    src/gui.h

DISTFILES += data.csv
