#pragma once

#include <QAction>
#include <QMenu>
#include <qtermwidget.h>

#define COLOR_SCHEME_DEFAULT "Nord"
#define SHELL_PROGRAM_DEFAULT "/bin/bash"

class LionaTerminal : public QTermWidget {
public:
    explicit LionaTerminal(QWidget *parent = nullptr);
    ~LionaTerminal() = default;

public:
    void setup(QString defaultPath);

private:
    void setupUi(QString defaultPath);
    void setupActions();

private:
    QAction *copyAction = nullptr;
    QAction *pasteAction = nullptr;
};