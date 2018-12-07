#include <thread>

#include <QApplication>

#include "MainWindow.hpp"

#include "ConnectionToPlugin.hpp"

int main(int argc, char *argv[]) {

	std::list<Request*> request, result;
	std::mutex requestMutex, resultMutex;

    ConnectionToPlugin plugin;

    QApplication a(argc, argv);
    MainWindow w{plugin, request, result, requestMutex, resultMutex};

    std::thread communicationThread{[&plugin]() { plugin.run(); }};

    w.show();
    a.exec();

    plugin.stopProgram();
    communicationThread.join();

    return 0;
}
