#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QDir>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QMainWindow>
#include <QModelIndex>
#include <QSplitter>
#include <QTreeView>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QMenu>
#include <QAction>

#include <qtermwidget.h>

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private:
    void initialize();
    void initializeLeftSide();
    void initializeRightSide();

    void navigateToPath(const QString& path);

private:
    // left side
    QWidget* leftSideWidget;
    QVBoxLayout* leftSideLayout;
    QTreeView* treeView;
    QLineEdit* pathLineEdit;

    // common
    QSplitter* splitter;
    QFileSystemModel* fileModel;
    QString homePath;

    // right side
    QTermWidget* terminal;

    QAction *copyAction = nullptr;
    QAction *pasteAction = nullptr;
};

#endif // __MAINWINDOW_H__