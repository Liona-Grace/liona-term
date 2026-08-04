#include "MainApplication.h"
#include "MainWindow.h"

MainApplication::MainApplication(int& argc, char* argv[]) : argc(argc), argv(argv) { }

MainApplication::~MainApplication() {}

int MainApplication::run() {
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
