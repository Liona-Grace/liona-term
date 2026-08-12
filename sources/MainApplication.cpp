#include "MainApplication.h"
#include "MainWindow.h"

#include <QFile>

MainApplication::MainApplication(int& argc, char* argv[]) : argc(argc), argv(argv) { }

MainApplication::~MainApplication() {}

int MainApplication::run() {
    QApplication app(argc, argv);

    QFile styleFile(QStringLiteral(":/liona-term/styles/liona_style.qss"));
    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text))
        app.setStyleSheet(QString::fromUtf8(styleFile.readAll()));

    MainWindow window;
    window.show();

    return app.exec();
}
