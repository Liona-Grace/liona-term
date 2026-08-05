#include "LionaTerminal.h"

namespace terminal_defaults
{
    constexpr auto DefaultColorScheme = "Nord";
    constexpr auto DefaultShellProgram = "/bin/bash";
}

LionaTerminal::LionaTerminal(const QString& defaultPath, QWidget *parent) : QTermWidget(0, parent) { 
    setup(defaultPath);
}

void LionaTerminal::setup(const QString& defaultPath) {
    setupUi(defaultPath);
    setupActions();
}

void LionaTerminal::setupUi(const QString& defaultPath) {
    setColorScheme(terminal_defaults::DefaultColorScheme);
    setShellProgram(terminal_defaults::DefaultShellProgram);
    setWorkingDirectory(defaultPath);
    setContextMenuPolicy(Qt::CustomContextMenu);
    startShellProgram();
}

void LionaTerminal::setupActions() {
    copyAction = new QAction(tr("Copy"), this);
    copyAction->setShortcut(QKeySequence(QStringLiteral("Ctrl+Shift+C")));

    pasteAction = new QAction(tr("Paste"), this);
    pasteAction->setShortcut(QKeySequence(QStringLiteral("Ctrl+Shift+V")));

    connect(
        copyAction,
        &QAction::triggered,
        this,
        &QTermWidget::copyClipboard
    );

    connect(
        pasteAction,
        &QAction::triggered,
        this,
        &QTermWidget::pasteClipboard
    );

    connect(
        this,
        &QTermWidget::copyAvailable,
        copyAction,
        &QAction::setEnabled
    );

    QObject::connect(
        this,
        &QWidget::customContextMenuRequested,
        this,
        [this](const QPoint &position) {
            QMenu menu(this);

            menu.addAction(copyAction);
            menu.addAction(pasteAction);

            menu.exec(this->mapToGlobal(position));
        }
    );
}
