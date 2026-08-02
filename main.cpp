#include <QApplication>
#include <QFileSystemModel>
#include <QMainWindow>
#include <QSplitter>
#include <QTreeView>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto *window = new QMainWindow;
    auto *splitter = new QSplitter(Qt::Horizontal);

    auto *model = new QFileSystemModel(splitter);
    model->setRootPath(QDir::homePath());

    auto *tree = new QTreeView(splitter);
    tree->setModel(model);
    tree->setRootIndex(model->index(QDir::homePath()));

    window->setCentralWidget(splitter);
    window->resize(1200, 750);
    window->show();

    return app.exec();
}
