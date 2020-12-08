#ifndef FILESYSTEMWIDGET_H
#define FILESYSTEMWIDGET_H

#include <QWidget>
#include <QModelIndex>
#include "osinitializerstrategy.h"
#include "osinitializercreator.h"
#include "abstractfilesystemworker.h"
#include "abstractfileoperations.h"

class QMenu;

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
    QString GetFile() const;
public slots:
    void slotOpenContextMenu(const QPoint &pos);
private slots:
    void on_RootsComboBox_currentTextChanged(const QString &arg1);
    void on_DirTreeView_collapsed(const QModelIndex &index);
    void on_DirTreeView_expanded(const QModelIndex &index);
    void on_DirTreeView_activated(const QModelIndex &index);
    void on_FileListView_doubleClicked(const QModelIndex &index);
    void on_SuffixComboBox_textActivated(const QString &suffix);
//Context menu slots
    void slotPaste();
    void slotCreateNewFile();
    void slotCopyFile();
    void slotCutFile();
    void slotRenameFile();
    void slotDeleteFile();
    void slotCreateNewDir();
    void slotCopyDir();
    void slotCutDir();
    void slotRenameDir();
    void slotDeleteDir();
    void slotHrefFileList();
    void slotTagList();
private:
    Ui::FileSystemWidget *ui;
    QFileSystemModel *file_model_ = nullptr;
    QFileSystemModel *dir_model_ = nullptr;
    AbstractFileSystemWorker& file_system_worker_;
    std::shared_ptr<AbstractFileOperations> file_operations_;
    std::shared_ptr<OSInitializerStrategy> os_initializer_;    
    QModelIndex last_model_index_;
    QMenu *file_list_context_menu_;
    QMenu *empty_file_list_context_menu_;
    QMenu *dir_tree_context_menu_;
    QMenu *html_sub_menu_;
    void ContextMenusInitialization();
    void HideColumns();
    void FillRootsComboBox();
    void SetFilter(const QString& suffix);
    void RefreshSuffixComboBox(const QString& dir_path);
    void ChangeFileModelPath(const QString &root_path);
    void ChangeDirModelRootPath(const QString &root_path);
    void FileListViewContextMenu(const QPoint &pos);
    void DirTreeViewContextMenu(const QPoint &pos);
};

#endif // FILESYSTEMWIDGET_H
