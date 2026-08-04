#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    initialize();
}

void MainWindow::initialize() {
    // init custom scheme dir
    QTermWidget::addCustomColorSchemeDir(
        QStringLiteral(LIONA_COLOR_SCHEME_DIR)
    );
    homePath = QDir::rootPath();

    // initialize the splitter
    splitter = new QSplitter(Qt::Horizontal, this);

    // initialize the file model
    fileModel = new QFileSystemModel(splitter);
    fileModel->setFilter(
        QDir::AllDirs |
        QDir::Files |
        QDir::NoDotAndDotDot
    );

    fileModel->setRootPath(homePath);

    initializeLeftSide();
    initializeRightSide();

    // add widgets to the splitter
    splitter->addWidget(leftSideWidget);
    splitter->addWidget(terminal);

    splitter->setSizes({220, 900});
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    setCentralWidget(splitter);
    resize(1200, 760);
    setWindowTitle(QStringLiteral("Liona Term"));
}

void MainWindow::initializeLeftSide() {
    leftSideWidget = new QWidget(splitter);
    leftSideLayout = new QVBoxLayout(leftSideWidget);
    leftSideLayout->setContentsMargins(0, 0, 0, 0);
    leftSideLayout->setSpacing(2);

    pathLineEdit = new QLineEdit(leftSideWidget);
    pathLineEdit->setPlaceholderText("Path...");

    treeView = new QTreeView(leftSideWidget);
    treeView->setModel(fileModel);
    treeView->setRootIndex(fileModel->index(homePath));

    // Chỉ giữ cột tên file/thư mục.
    treeView->hideColumn(1);
    treeView->hideColumn(2);
    treeView->hideColumn(3);

    treeView->setHeaderHidden(true);
    treeView->setAnimated(true);
    treeView->setIndentation(16);

    connect(
    treeView,
    &QTreeView::clicked,
    this,
    [this](const QModelIndex& index)
    {
        QFileInfo info = fileModel->fileInfo(index);

        if (info.isDir())
        {
            pathLineEdit->setText(info.absoluteFilePath());
        }
    }
);

connect(
    pathLineEdit,
    &QLineEdit::returnPressed,
    this,
    [this]()
    {
        navigateToPath(pathLineEdit->text().trimmed());
    }
);

    leftSideLayout->addWidget(pathLineEdit);
    leftSideLayout->addWidget(treeView);
}

void MainWindow::initializeRightSide() {
    terminal = new QTermWidget(0, splitter);
    terminal->setColorScheme(QStringLiteral("Nord"));
    terminal->setShellProgram(QStringLiteral("/bin/bash"));
    terminal->setWorkingDirectory(homePath);
    terminal->startShellProgram();

    copyAction = new QAction(tr("Copy"), this);
    copyAction->setShortcut(QKeySequence(QStringLiteral("Ctrl+Shift+C")));

    pasteAction = new QAction(tr("Paste"), this);
    pasteAction->setShortcut(QKeySequence(QStringLiteral("Ctrl+Shift+V")));

    connect(
        copyAction,
        &QAction::triggered,
        terminal,
        &QTermWidget::copyClipboard
    );

    connect(
        pasteAction,
        &QAction::triggered,
        terminal,
        &QTermWidget::pasteClipboard
    );

    connect(
        terminal,
        &QTermWidget::copyAvailable,
        copyAction,
        &QAction::setEnabled
    );

    terminal->setContextMenuPolicy(Qt::CustomContextMenu);

    QObject::connect(
        terminal,
        &QWidget::customContextMenuRequested,
        terminal,
        [this](const QPoint &position) {
            QMenu menu(terminal);

            menu.addAction(copyAction);
            menu.addAction(pasteAction);

            menu.exec(terminal->mapToGlobal(position));
        }
    );
}

void MainWindow::navigateToPath(const QString& inputPath)
{
    QFileInfo fileInfo(inputPath);

    if (!fileInfo.exists())
    {
        pathLineEdit->setStyleSheet("color: red;");
        return;
    }

    const QString path = fileInfo.absoluteFilePath();
    const QModelIndex targetIndex = fileModel->index(path);

    if (!targetIndex.isValid())
    {
        pathLineEdit->setStyleSheet("color: red;");
        return;
    }

    pathLineEdit->setStyleSheet({});
    pathLineEdit->setText(path);

    QModelIndex parentIndex = targetIndex.parent();
    QList<QModelIndex> parents;

    while (parentIndex.isValid())
    {
        parents.prepend(parentIndex);
        parentIndex = parentIndex.parent();
    }

    for (const QModelIndex& index : parents)
        treeView->expand(index);

    treeView->setCurrentIndex(targetIndex);
    treeView->scrollTo(
        targetIndex,
        QAbstractItemView::PositionAtCenter
    );

    if (fileInfo.isDir())
        treeView->expand(targetIndex);
}
