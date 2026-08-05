#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QDir>
#include <QMainWindow>
#include <QSplitter>

#include "LionaTerminal.h"
#include "LionaFileExplorer.h"

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private:
    void initialize();

private:
    QSplitter* splitter = nullptr;
    LionaFileExplorer* fileExplorer = nullptr;
    LionaTerminal* terminal = nullptr;
};

#endif // __MAINWINDOW_H__