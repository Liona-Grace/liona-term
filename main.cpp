#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QMainWindow>
#include <QModelIndex>
#include <QSplitter>
#include <QTreeView>

#include <qtermwidget.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;

    auto *splitter = new QSplitter(Qt::Horizontal, &window);

    auto *fileModel = new QFileSystemModel(splitter);
    fileModel->setFilter(
        QDir::AllDirs |
        QDir::Files |
        QDir::NoDotAndDotDot
    );

    const QString homePath = QDir::homePath();
    fileModel->setRootPath(homePath);

    auto *treeView = new QTreeView(splitter);
    treeView->setModel(fileModel);
    treeView->setRootIndex(fileModel->index(homePath));

    // Chỉ giữ cột tên file/thư mục.
    treeView->hideColumn(1);
    treeView->hideColumn(2);
    treeView->hideColumn(3);

    treeView->setHeaderHidden(true);
    treeView->setAnimated(true);
    treeView->setIndentation(16);

    auto *terminal = new QTermWidget(0, splitter);
    terminal->setShellProgram(QStringLiteral("/bin/bash"));
    terminal->setWorkingDirectory(homePath);
    terminal->startShellProgram();

    splitter->addWidget(treeView);
    splitter->addWidget(terminal);

    // Tree khoảng 20%, terminal khoảng 80%.
    splitter->setSizes({220, 900});
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    window.setCentralWidget(splitter);
    window.resize(1200, 760);
    window.setWindowTitle(QStringLiteral("Liona Term"));
    window.show();

    return app.exec();
}
