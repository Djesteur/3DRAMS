#include <thread>

#include <QApplication>

#include "MainWindow.hpp"

#include "ConnectionToPlugin.hpp"

int main(int argc, char *argv[]) {

    ConnectionToPlugin plugin;

    QApplication a(argc, argv);
    MainWindow w{plugin};

    std::thread communicationThread{[&plugin]() {  plugin.run(); }};

    w.show();
    a.exec();

    communicationThread.join();

    return 0;
}
