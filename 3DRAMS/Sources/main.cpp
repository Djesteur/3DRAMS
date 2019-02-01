#include <thread>

#include <QApplication>

#include "MainWindow.hpp"

#include "ConnectionToPlugin.hpp"

#include "RequestTransmitter.hpp"

int main(int argc, char *argv[]) {

	RequestTransmitter transmitter;

    ConnectionToPlugin plugin{transmitter};

    std::thread communicationThread{[&plugin]() { plugin.run(); }};

    QApplication a{argc, argv};
    MainWindow w{transmitter};

    w.show();
    a.exec();

    plugin.stopProgram();
    communicationThread.join();

    return 0;
}
