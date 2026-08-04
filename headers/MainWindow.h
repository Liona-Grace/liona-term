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

#include "LionaTerminal.h"
#include "LionaFilesystem.h"

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private:
    void initialize();
    void initializeLeftSide();
    void initializeRightSide();

private:
    // common
    QSplitter* splitter = nullptr;
    QFileSystemModel* fileModel = nullptr;
    QString homePath;

    // left side
    LionaFilesystem* filesystem = nullptr;

    // right side
    LionaTerminal* terminal = nullptr;
};

#endif // __MAINWINDOW_H__