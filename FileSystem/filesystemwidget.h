#ifndef FILESYSTEMWIDGET_H
#define FILESYSTEMWIDGET_H

#include <QWidget>
#include "osinitializerstrategy.h"
#include "osinitializercreator.h"
#include "abstractfilesystemworker.h"

class QFileSystemModel;

namespace Ui {
class FileSystemWidget;
}

class FileSystemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileSystemWidget(QWidget *parent = nullptr);
    ~FileSystemWidget();

private slots:
    void on_RootsComboBox_currentTextChanged(const QString &arg1);

    void on_DirTreeView_collapsed(const QModelIndex &index);

    void on_DirTreeView_expanded(const QModelIndex &index);

    void on_DirTreeView_activated(const QModelIndex &index);

    void on_FileListView_doubleClicked(const QModelIndex &index);

private:
    Ui::FileSystemWidget *ui;
    QFileSystemModel *file_model_ = nullptr;
    QFileSystemModel *dir_model_ = nullptr;
    AbstractFileSystemWorker* file_system_worker_;
    std::shared_ptr<OSInitializerStrategy> os_initializer_;    
    void HideColumns();
    void FillRootsComboBox();
    void ChangeFileModelPath(const QString &root_path);
    void ChangeDirModelRootPath(const QString &root_path);
};

#endif // FILESYSTEMWIDGET_H
