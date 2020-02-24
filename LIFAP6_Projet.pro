######################################################################
# Automatically generated by qmake (3.1) Wed Feb 12 14:48:34 2020
######################################################################

TEMPLATE = app
CONFIG += c++17
CONFIG -= app_bundle
CONFIG -= qt

OBJECTS_DIR= obj/

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += src/board.hpp src/bonus.hpp src/spot.hpp \
    src/gaddag.hpp \
    src/letterbag.hpp \
    src/letterspecs.hpp \
    src/node.hpp \
    src/playerbag.hpp
SOURCES += src/board.cpp src/bonus.cpp src/spot.cpp src/test_board.cpp \
    src/gaddag.cpp \
    src/letterbag.cpp \
    src/letterspecs.cpp \
    src/node.cpp \
    src/playerbag.cpp