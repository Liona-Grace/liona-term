#include "LionaFilesystem.h"

LionaFilesystem::LionaFilesystem(QFileSystemModel* fileModel, QWidget* parent) : QWidget(parent), fileModel(fileModel) { }

void LionaFilesystem::setup(const QString& path) {
    setupUi(path);
    setupActions();
}

void LionaFilesystem::setupUi(QString defaultPath) {
    boxLayout = new QVBoxLayout(this);
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

    boxLayout->addWidget(pathLineEdit);
    boxLayout->addWidget(filesystemTreeView);
}

void LionaFilesystem::setupActions() {
    connect(
        filesystemTreeView,
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
}

void LionaFilesystem::navigateToPath(const QString& inputPath) {
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
