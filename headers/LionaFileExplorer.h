#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTreeView>
#include <QModelIndex>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QAction>
#include <QMenu>
#include <QApplication>
#include <QClipboard>

class LionaFileExplorer : public QWidget {
public:
    explicit LionaFileExplorer(const QString& defaultPath, QWidget* parent = nullptr);
    ~LionaFileExplorer() = default;
    void navigateToPath(const QString& path);

private:
    void setup(const QString& path);
    void setupUi(const QString& defaultPath);
    void setupActions();

private:
    QLineEdit* pathLineEdit = nullptr;
    QTreeView* filesystemTreeView = nullptr;
    QFileSystemModel* fileModel = nullptr;

    QAction *copyAction = nullptr;
};
