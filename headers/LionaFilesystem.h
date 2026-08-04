#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTreeView>
#include <QModelIndex>
#include <QFileInfo>
#include <QFileSystemModel>

class LionaFilesystem : public QWidget {
public:
    explicit LionaFilesystem(QFileSystemModel* fileModel, QWidget* parent = nullptr);
    ~LionaFilesystem() = default;

public:
    void setup(const QString& path);

private:
    void setupUi(QString defaultPath);
    void setupActions();
    void navigateToPath(const QString& path);

private:
    QVBoxLayout* boxLayout = nullptr;
    QLineEdit* pathLineEdit = nullptr;
    QTreeView* filesystemTreeView = nullptr;
    QFileSystemModel* fileModel = nullptr;
};
