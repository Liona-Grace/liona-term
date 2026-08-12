#include "LionaTerminal.h"

#include <QApplication>
#include <QBuffer>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QProcessEnvironment>

namespace terminal_defaults
{
    constexpr auto DefaultColorScheme = "Nord";
    constexpr int HistorySize = 5000;

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

    saveAsAction = new QAction(tr("Save as..."), this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);

    navigateAction = new QAction(tr("Navigate to"), this);

    setCustomKeyBindingsDir(QStringLiteral(":/liona-term/kb-layouts"));
    setKeyBindings(QStringLiteral("default"));
    setColorScheme(terminal_defaults::DefaultColorScheme);
    setShellProgram(terminal_defaults::shellProgram());
    setWorkingDirectory(defaultPath);
    setHistorySize(terminal_defaults::HistorySize);
    setContextMenuPolicy(Qt::CustomContextMenu);
    startShellProgram();

    addAction(copyAction);
    addAction(pasteAction);
    addAction(saveAsAction);
    addAction(navigateAction);
}

void LionaTerminal::setupActions() {
    connect(
        this,
        &QTermWidget::finished,
        qApp,
        &QApplication::quit
    );

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
        saveAsAction,
        &QAction::triggered,
        this,
        &LionaTerminal::saveHistoryAs
    );

    connect(
        navigateAction,
        &QAction::triggered,
        this,
        [this]()
        {
            const QString path = workingDirectory();

            if (!path.isEmpty())
                emit navigationRequested(path);
        }
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
            menu.addSeparator();
            menu.addAction(navigateAction);
            menu.addAction(saveAsAction);

            menu.exec(this->mapToGlobal(position));
        }
    );
}

void LionaTerminal::saveHistoryAs()
{
    const QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("Save terminal history"),
        QStringLiteral("terminal-history.txt"),
        tr("Text files (*.txt);;All files (*)")
    );

    if (filePath.isEmpty())
        return;

    QBuffer historyBuffer;
    historyBuffer.open(QIODevice::WriteOnly);
    saveHistory(&historyBuffer);

    QStringList lines = QString::fromUtf8(historyBuffer.data()).split(QLatin1Char('\n'));

    for (QString& line : lines)
    {
        while (!line.isEmpty() && line.back().isSpace())
            line.chop(1);
    }

    while (!lines.isEmpty() && lines.back().isEmpty())
        lines.removeLast();

    QByteArray history = lines.join(QLatin1Char('\n')).toUtf8();

    if (!history.isEmpty())
        history.append('\n');

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(
            this,
            tr("Save failed"),
            tr("Cannot write to %1").arg(filePath)
        );
        return;
    }

    if (file.write(history) != history.size())
    {
        QMessageBox::warning(
            this,
            tr("Save failed"),
            tr("Cannot write all terminal history to %1").arg(filePath)
        );
    }
}
