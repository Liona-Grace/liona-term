#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    initialize();
}

void MainWindow::initialize() {
    // init custom scheme dir
    QTermWidget::addCustomColorSchemeDir(
        QStringLiteral(LIONA_COLOR_SCHEME_DIR)
    );
    homePath = QDir::rootPath();

    // initialize the splitter
    splitter = new QSplitter(Qt::Horizontal, this);

    // initialize the file model
    fileModel = new QFileSystemModel(splitter);
    fileModel->setFilter(
        QDir::AllDirs |
        QDir::Files |
        QDir::NoDotAndDotDot
    );

    fileModel->setRootPath(homePath);

    initializeLeftSide();
    initializeRightSide();

    // add widgets to the splitter
    splitter->addWidget(filesystem);
    splitter->addWidget(terminal);

    splitter->setSizes({220, 900});
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    setCentralWidget(splitter);
    resize(1200, 760);
    setWindowTitle(QStringLiteral("Liona Term"));
}

void MainWindow::initializeLeftSide() {
    filesystem = new LionaFilesystem(fileModel, splitter);
    filesystem->setup(homePath);
}

void MainWindow::initializeRightSide() {
    terminal = new LionaTerminal(splitter);
    terminal->setup(homePath);
}
