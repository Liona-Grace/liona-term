#pragma once

#include <QTabWidget>

class QAction;
class LionaTerminal;

class LionaTab : public QTabWidget
{
    Q_OBJECT

public:
    explicit LionaTab(const QString& defaultPath, QWidget* parent = nullptr);

    LionaTerminal* currentTerminal() const;
    QString currentWorkingDirectory() const;

public slots:
    LionaTerminal* addTerminal(const QString& path = {});
    void closeTerminal(int index);

signals:
    void navigationRequested(const QString& path);
    void activeTerminalChanged(LionaTerminal* terminal);

private:
    void setupUi();
    void setupActions();
    void connectTerminal(LionaTerminal* terminal);
    void updateTerminalTitle(LionaTerminal* terminal, const QString& path);

private:
    QString defaultPath;
    QAction* newTerminalAction = nullptr;
};
