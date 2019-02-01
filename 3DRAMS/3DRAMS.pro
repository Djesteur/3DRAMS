TEMPLATE = app
TARGET = 3DRAMS

CONFIG += qt debug
QT += core gui widgets

QMAKE_CFLAGS *= -g -O0 -std=c11 -Wextra -Wall -pedantic
QMAKE_CXXFLAGS *= -g -O0 -std=c++1z -Wextra -Wall -pedantic
LIBS *= -lpthread
INCLUDEPATH *= ./Includes ./Includes/Modules ./Includes/Widgets

OBJECTS_DIR = ./Objs
MOC_DIR = ./Mocs

# Input
HEADERS += Includes/MainWindow.hpp \
           Includes/Modules/AbstractModule.hpp \
           Includes/Modules/InformationModule.hpp \
           Includes/Modules/ConnectionModule.hpp \
           Includes/Modules/SurveilModule.hpp \
           Includes/Modules/SeekerModule.hpp \
           Includes/ConnectionToPlugin.hpp \
           Includes/Request.hpp \
           Includes/Converter.hpp \
           Includes/RequestTransmitter.hpp \
           Includes/Research.hpp \
           Includes/Widgets/HexaWidget.hpp

SOURCES += Sources/main.cpp \
           Sources/MainWindow.cpp \
           Sources/Modules/AbstractModule.cpp \
           Sources/Modules/InformationModule.cpp \
           Sources/Modules/ConnectionModule.cpp \
           Sources/Modules/SurveilModule.cpp \
           Sources/Modules/SeekerModule.cpp \
           Sources/ConnectionToPlugin.cpp \
           Sources/Request.cpp \
           Sources/Converter.cpp \
           Sources/RequestTransmitter.cpp \
           Sources/Research.cpp \
           Sources/Widgets/HexaWidget.cpp