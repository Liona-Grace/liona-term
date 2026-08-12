#include "LionaTab.h"

#include "LionaTerminal.h"

#include <QAction>
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QKeySequence>
#include <QResizeEvent>
#include <QTabBar>
#include <QTimer>
#include <QToolButton>

LionaTab::LionaTab(const QString& defaultPath, QWidget* parent)
    : QTabWidget(parent), defaultPath(defaultPath)
{
    setupUi();
    setupActions();
    addTerminal(defaultPath);
}

void LionaTab::setupUi()
{
    setTabsClosable(true);
    setMovable(true);
    setDocumentMode(true);

    addButton = new QToolButton(tabBar());
    addButton->setText(QStringLiteral("+"));
    addButton->setToolTip(tr("New terminal"));
    addButton->setAutoRaise(true);
    addButton->show();

    connect(addButton, &QToolButton::clicked, this, [this]()
    {
        addTerminal();
    });

    connect(tabBar(), &QTabBar::tabMoved, this, [this]()
    {
        updateAddButtonPosition();
    });
}

void LionaTab::setupActions()
{
    newTerminalAction = new QAction(tr("New terminal"), this);
    newTerminalAction->setShortcut(QKeySequence(QStringLiteral("Ctrl+Shift+T")));
    addAction(newTerminalAction);

    connect(newTerminalAction, &QAction::triggered, this, [this]()
    {
        addTerminal();
    });

    connect(this, &QTabWidget::tabCloseRequested, this, &LionaTab::closeTerminal);

    connect(this, &QTabWidget::currentChanged, this, [this](int)
    {
        LionaTerminal* terminal = currentTerminal();
        emit activeTerminalChanged(terminal);

        if (!terminal)
            return;

        const QString path = terminal->workingDirectory();

        if (!path.isEmpty())
            emit navigationRequested(path);
    });
}

LionaTerminal* LionaTab::addTerminal(const QString& path)
{
    QString workingPath = path;

    if (workingPath.isEmpty())
        workingPath = currentWorkingDirectory();

    if (workingPath.isEmpty() || !QFileInfo(workingPath).isDir())
        workingPath = defaultPath;

    auto* terminal = new LionaTerminal(workingPath, this);
    connectTerminal(terminal);

    const int index = addTab(terminal, QString());
    updateTerminalTitle(terminal, workingPath);
    setCurrentIndex(index);
    terminal->setFocus();
    QTimer::singleShot(0, this, [this]() { updateAddButtonPosition(); });

    return terminal;
}

void LionaTab::closeTerminal(int index)
{
    QWidget* terminal = widget(index);

    if (!terminal)
        return;

    removeTab(index);
    terminal->deleteLater();
    updateAddButtonPosition();

    if (count() == 0)
        QApplication::quit();
}

void LionaTab::resizeEvent(QResizeEvent* event)
{
    QTabWidget::resizeEvent(event);
    updateAddButtonPosition();
}

void LionaTab::updateAddButtonPosition()
{
    if (!addButton || count() == 0)
        return;

    const QRect lastTabRect = tabBar()->tabRect(count() - 1);
    const int spacing = 2;
    int availableRight = tabBar()->width();

    // QTabBar creates its own navigation buttons when the tabs overflow.
    // Keep the add button to their left instead of covering them.
    const auto tabBarButtons = tabBar()->findChildren<QToolButton*>(
        QString(),
        Qt::FindDirectChildrenOnly
    );
    for (QToolButton* button : tabBarButtons)
    {
        if (button != addButton && button->isVisible())
            availableRight = qMin(availableRight, button->geometry().left());
    }

    const int maximumX = availableRight - addButton->width() - spacing;
    const int x = qMax(0, qMin(lastTabRect.right() + spacing, maximumX));
    const int y = (tabBar()->height() - addButton->height()) / 2;

    addButton->move(x, y);
    addButton->raise();
}

LionaTerminal* LionaTab::currentTerminal() const
{
    return qobject_cast<LionaTerminal*>(currentWidget());
}

QString LionaTab::currentWorkingDirectory() const
{
    LionaTerminal* terminal = currentTerminal();
    return terminal ? terminal->workingDirectory() : QString();
}

void LionaTab::connectTerminal(LionaTerminal* terminal)
{
    connect(
        terminal,
        &LionaTerminal::navigationRequested,
        this,
        [this, terminal](const QString& path)
        {
            if (terminal == currentTerminal())
            {
                updateTerminalTitle(terminal, path);
                emit navigationRequested(path);
            }
        }
    );

    connect(
        terminal,
        &QTermWidget::currentDirectoryChanged,
        this,
        [this, terminal](const QString& path)
        {
            updateTerminalTitle(terminal, path);
        }
    );

    connect(terminal, &QTermWidget::finished, this, [this, terminal]()
    {
        const int index = indexOf(terminal);

        if (index >= 0)
            closeTerminal(index);
    });
}

void LionaTab::updateTerminalTitle(LionaTerminal* terminal, const QString& path)
{
    const int index = indexOf(terminal);

    if (index < 0)
        return;

    QString title = QFileInfo(path).fileName();

    if (title.isEmpty())
        title = QDir::rootPath();

    setTabText(index, title);
    setTabToolTip(index, path);
}
