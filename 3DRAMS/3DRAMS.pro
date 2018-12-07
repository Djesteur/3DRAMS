TEMPLATE = app
TARGET = 3DRAMS

CONFIG += qt debug
QT += core gui widgets

QMAKE_CFLAGS *= -g -O0 -std=c11 -Wextra -Wall -pedantic -lpthread -I./Includes -I./Includes/Modules
QMAKE_CXXFLAGS *= -g -O0 -std=c++1z -Wextra -Wall -pedantic -lpthread -I./Includes -I./Includes/Modules
LIBS = -lpthread

OBJECTS_DIR = ./Objs
MOC_DIR = ./Mocs

# Input
HEADERS += Includes/MainWindow.hpp \
           Includes/Modules/AbstractModule.hpp \
           Includes/Modules/InformationModule.hpp \
           Includes/Modules/ConnectionModule.hpp \
           Includes/Modules/SurveilModule.hpp \
           Includes/ConnectionToPlugin.hpp \
           Includes/Request.hpp \
           Includes/Converter.hpp

HEADERS += Sources/MainWindow.cpp \
           Sources/Modules/AbstractModule.cpp \
           Sources/Modules/InformationModule.cpp \
           Sources/Modules/ConnectionModule.cpp \
           Sources/Modules/SurveilModule.cpp \
           Sources/ConnectionToPlugin.cpp \
           Sources/Request.cpp \
           Sources/Converter.cpp
