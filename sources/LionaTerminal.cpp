#include "LionaTerminal.h"

#include <QFileInfo>
#include <QProcessEnvironment>

namespace terminal_defaults
{
    constexpr auto DefaultColorScheme = "Nord";

    QString shellProgram()
    {
        const QString configuredShell =
            QProcessEnvironment::systemEnvironment().value(QStringLiteral("SHELL"));
        const QFileInfo configuredShellInfo(configuredShell);

        if (configuredShellInfo.isFile() && configuredShellInfo.isExecutable())
            return configuredShellInfo.absoluteFilePath();

        return QStringLiteral("/bin/sh");
    }
}

LionaTerminal::LionaTerminal(const QString& defaultPath, QWidget *parent) : QTermWidget(0, parent) { 
    setup(defaultPath);
}

void LionaTerminal::setup(const QString& defaultPath) {
    setupUi(defaultPath);
    setupActions();
}

void LionaTerminal::setupUi(const QString& defaultPath) {
    copyAction = new QAction(tr("Copy"), this);
    copyAction->setShortcut(QKeySequence(QStringLiteral("Ctrl+Shift+C")));

    pasteAction = new QAction(tr("Paste"), this);
    pasteAction->setShortcut(QKeySequence(QStringLiteral("Ctrl+Shift+V")));

    setColorScheme(terminal_defaults::DefaultColorScheme);
    setShellProgram(terminal_defaults::shellProgram());
    setWorkingDirectory(defaultPath);
    setContextMenuPolicy(Qt::CustomContextMenu);
    startShellProgram();

    addAction(copyAction);
    addAction(pasteAction);
}

void LionaTerminal::setupActions() {
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
