#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QDir>
#include <QMainWindow>
#include <QSplitter>

#include "LionaFileExplorer.h"
#include "LionaTab.h"

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private:
    void initialize();

private:
    QSplitter* splitter = nullptr;
    LionaFileExplorer* fileExplorer = nullptr;
    LionaTab* terminalTabs = nullptr;
};

#endif // __MAINWINDOW_H__
