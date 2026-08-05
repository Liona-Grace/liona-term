#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    initialize();
}

void MainWindow::initialize() {
    // init custom scheme dir
    QTermWidget::addCustomColorSchemeDir(
        QStringLiteral(LIONA_COLOR_SCHEME_DIR)
    );
    auto homePath = QDir::rootPath();

    // initialize the splitter
    splitter = new QSplitter(Qt::Horizontal, this);

    fileExplorer = new LionaFileExplorer(homePath, splitter);
    terminal = new LionaTerminal(homePath, splitter);

    // add widgets to the splitter
    splitter->addWidget(fileExplorer);
    splitter->addWidget(terminal);

    splitter->setSizes({220, 900});
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    setCentralWidget(splitter);
    resize(1200, 760);
    setWindowTitle(QStringLiteral("Liona Term"));
}
