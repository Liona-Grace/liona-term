#include "LionaTerminal.h"

LionaTerminal::LionaTerminal(QWidget *parent) : QTermWidget(0, parent) { }

void LionaTerminal::setup(QString defaultPath) {
    setupUi(defaultPath);
    setupActions();
}

void LionaTerminal::setupUi(QString defaultPath) {
    this->setColorScheme(COLOR_SCHEME_DEFAULT);
    this->setShellProgram(SHELL_PROGRAM_DEFAULT);
    this->setWorkingDirectory(defaultPath);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->startShellProgram();
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
