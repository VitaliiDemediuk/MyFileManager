#include "filesystemworker.h"

#include <QDir>
#include <QFile>
#include <QWidget>
#include <QMessageBox>
#include <QClipboard>
#include <QMimeData>
#include <QDebug>
#include <QUrl>
#include <QFileInfo>

FileSystemWorker::FileSystemWorker(){
    connect(clipboard_, SIGNAL(dataChanged()), this, SLOT(clipboardChange()));
}

AbstractFileSystemWorker* FileSystemWorker::GetInstance(){
    static FileSystemWorker instance;
    return &instance;
}

bool FileSystemWorker::Paste(const QString &path, QWidget *parent){
    QString file_path = clipboard_->mimeData()->urls().at(1).toLocalFile();
    bool is_correct = true;
    if(!file_path.isEmpty()){
        is_correct = false;
        if(should_cut_){
            is_correct = QFile::rename(file_path, path+QFileInfo(file_path).filePath());
        }else{
            is_correct = QFile::copy(file_path, path+QFileInfo(file_path).filePath());
        }
        if(!is_correct){
            QMessageBox::critical(parent, "Paste error", "Paste error!");
        }
    }
    return is_correct;
}

//File's standard operations----------------------------------------------
bool FileSystemWorker::CreateNewFile(const QString &path, const QString &name, QWidget *parent) const{
    bool is_exists = QFile::exists(path+name);
    if(!is_exists){
        QFile file(path);
        if(!file.open(QIODevice::WriteOnly)){
            QMessageBox::critical(parent, "Creating error", "Can not creating file!");
        }
    }else{
        QMessageBox::critical(parent, "Creating error", "File with this name already exists!");
    }
    return !is_exists;
}

bool FileSystemWorker::CopyFile(const QString &path, QWidget *parent){
    QMimeData data;
    data.setUrls({QUrl::fromLocalFile(path)});
    clipboard_->setMimeData(&data);
    return true;
}

bool FileSystemWorker::CutFile(const QString &path, QWidget *parent){
    bool result = CopyFile(path, parent);
    should_cut_ = result;
    return result;
}

bool FileSystemWorker::RenameFile(const QString &path, const QString &old_name, const QString &new_name, QWidget *parent) const{
    bool is_rename = QFile::rename(path + old_name, path + new_name);
    if(!is_rename){
        QMessageBox::critical(parent, "Renaming error", "Can not rename this file!");
    }
    return is_rename;
}

bool FileSystemWorker::DeleteFile(const QString &path, QWidget *parent) const{
    bool is_deleted = QFile::moveToTrash(path);
    if(!is_deleted){
        if(!QFile::remove(path)){
            QMessageBox::critical(parent, "Deleting error", "Can not delete this file!");
        }
    }
    return is_deleted;
}

//Directory's standard operations----------------------------------------------
bool FileSystemWorker::CreateNewDir(const QString &path, const QString &name, QWidget *parent) const{
    QDir dir(path);
    if(!dir.exists(name)){
        if(!dir.mkdir(name)){
            QMessageBox::critical(parent, "Creating error", "Can not creating directory!");
            return false;
        }
    }else{
        QMessageBox::critical(parent, "Creating error", "Directory with this name already exists!");
        return false;
    }
    return true;
}

bool FileSystemWorker::CopyDir(const QString &path, QWidget *parent){
    QMimeData data;
    data.setUrls({QUrl::fromLocalFile(path)});
    clipboard_->setMimeData(&data);
    return true;
}

bool FileSystemWorker::CutDir(const QString &path, QWidget *parent){
    bool result = CopyDir(path, parent);
    should_cut_ = result;
    return result;
}

bool FileSystemWorker::RenameDir(const QString &path, const QString &old_name, const QString &new_name, QWidget *parent) const{
    bool is_rename = QDir(path).rename(old_name, new_name);
    if(!is_rename){
        QMessageBox::critical(parent, "Renaming error", "Can not rename this directory!");
    }
    return is_rename;
}

bool FileSystemWorker::DeleteDir(const QString &path, QWidget *parent) const{
    bool is_deleted = QFile::moveToTrash(path);
    if(!is_deleted){
        if(!QDir(path).removeRecursively()){
            QMessageBox::critical(parent, "Deleting error", "Can not delete this file!");
        }
    }
    return is_deleted;
}

//SLOT-------------------------------------------------------------------------------------

void FileSystemWorker::clipboardChange(){
    should_cut_ = false;
}
