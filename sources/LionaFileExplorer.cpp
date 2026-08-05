#include "LionaFileExplorer.h"

LionaFileExplorer::LionaFileExplorer(const QString& defaultPath, QWidget* parent) : QWidget(parent) {
    setup(defaultPath);
}

void LionaFileExplorer::setup(const QString& path) {
    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(
        QDir::AllDirs |
        QDir::Files |
        QDir::NoDotAndDotDot
    );
    fileModel->setRootPath(path);

    setupUi(path);
    setupActions();
}

void LionaFileExplorer::setupUi(const QString& defaultPath) {
    auto *boxLayout = new QVBoxLayout(this);
    boxLayout->setContentsMargins(0, 0, 0, 0);
    boxLayout->setSpacing(2);

    pathLineEdit = new QLineEdit(this);
    pathLineEdit->setPlaceholderText("Path...");

    filesystemTreeView = new QTreeView(this);
    filesystemTreeView->setModel(fileModel);
    filesystemTreeView->setRootIndex(fileModel->index(defaultPath));

    // Chỉ giữ cột tên file/thư mục.
    filesystemTreeView->hideColumn(1);
    filesystemTreeView->hideColumn(2);
    filesystemTreeView->hideColumn(3);

    filesystemTreeView->setHeaderHidden(true);
    filesystemTreeView->setAnimated(true);
    filesystemTreeView->setIndentation(16);
    filesystemTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    boxLayout->addWidget(pathLineEdit);
    boxLayout->addWidget(filesystemTreeView);
}

void LionaFileExplorer::setupActions()
{
    copyAction = new QAction(tr("Copy path"), this);

    connect(
        filesystemTreeView,
        &QWidget::customContextMenuRequested,
        this,
        [this](const QPoint& position)
        {
            const QModelIndex index =
                filesystemTreeView->indexAt(position);

            if (!index.isValid())
            {
                return;
            }

            filesystemTreeView->setCurrentIndex(index);

            QMenu menu(filesystemTreeView);
            menu.addAction(copyAction);

            menu.exec(
                filesystemTreeView->viewport()->mapToGlobal(position)
            );
        }
    );

    connect(
        copyAction,
        &QAction::triggered,
        this,
        [this]()
        {
            const QModelIndex index =
                filesystemTreeView->currentIndex();

            if (!index.isValid())
            {
                return;
            }

            const QFileInfo info = fileModel->fileInfo(index);

            QApplication::clipboard()->setText(
                info.absoluteFilePath()
            );
        }
    );

    connect(
        filesystemTreeView,
        &QTreeView::clicked,
        this,
        [this](const QModelIndex& index)
        {
            const QFileInfo info = fileModel->fileInfo(index);

            if (info.isDir())
            {
                pathLineEdit->setText(
                    info.absoluteFilePath()
                );
            }
            else
            {
                pathLineEdit->setText(
                    info.absolutePath()
                );
            }
        }
    );

    connect(
        pathLineEdit,
        &QLineEdit::returnPressed,
        this,
        [this]()
        {
            navigateToPath(
                pathLineEdit->text().trimmed()
            );
        }
    );
}

void LionaFileExplorer::navigateToPath(const QString& inputPath) {
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
        filesystemTreeView->expand(index);

    filesystemTreeView->setCurrentIndex(targetIndex);
    filesystemTreeView->scrollTo(
        targetIndex,
        QAbstractItemView::PositionAtCenter
    );

    if (fileInfo.isDir())
        filesystemTreeView->expand(targetIndex);
}
