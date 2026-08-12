#include "MainWindow.h"

#include <qtermwidget.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    initialize();
}

void MainWindow::initialize() {
    // init custom scheme dir
    QTermWidget::addCustomColorSchemeDir(
        QStringLiteral(":/liona-term/color-schemes")
    );
    auto homePath = QDir::homePath();

    // initialize the splitter
    splitter = new QSplitter(Qt::Horizontal, this);

    fileExplorer = new LionaFileExplorer(homePath, splitter);
    terminalTabs = new LionaTab(homePath, splitter);

    connect(
        terminalTabs,
        &LionaTab::navigationRequested,
        fileExplorer,
        &LionaFileExplorer::navigateToPath
    );

    // add widgets to the splitter
    splitter->addWidget(fileExplorer);
    splitter->addWidget(terminalTabs);

    splitter->setSizes({220, 900});
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    setCentralWidget(splitter);
    resize(1200, 760);
    setWindowTitle(QStringLiteral("Liona Term"));
}
