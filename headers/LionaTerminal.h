#pragma once

#include <QAction>
#include <QMenu>
#include <qtermwidget.h>

class LionaTerminal : public QTermWidget {
public:
    explicit LionaTerminal(const QString& defaultPath, QWidget *parent = nullptr);
    ~LionaTerminal() = default;

private:
    void setup(const QString& defaultPath);
    void setupUi(const QString& defaultPath);
    void setupActions();
    void saveHistoryAs();

private:
    QAction *copyAction = nullptr;
    QAction *pasteAction = nullptr;
    QAction *saveAsAction = nullptr;
};
