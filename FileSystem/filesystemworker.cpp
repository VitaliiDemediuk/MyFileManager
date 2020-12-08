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

AbstractFileSystemWorker& FileSystemWorker::GetInstance(){
    static FileSystemWorker instance;
    return instance;
}

bool FileSystemWorker::Paste(const QString &path, QWidget *parent){
    QList<QUrl> url_list = clipboard_->mimeData()->urls();
    QString file_path;
    if(url_list.size()){
         file_path = url_list.at(0).toLocalFile();
    }
    bool is_correct = true;
    if(!file_path.isEmpty()){
        if(QDir(file_path).exists()){
            is_correct = PasteDir(file_path, path, parent);
        }else if(QFile::exists(file_path)){
            is_correct = PasteFile(file_path, path, parent);
        }else{
            QMessageBox::critical(parent, "Paste error", "File or directory no longer exists!");
            is_correct = false;
        }

    }
    return is_correct;
}

//File's standard operations----------------------------------------------
bool FileSystemWorker::CreateNewFile(const QString &path, const QString &name, QWidget *parent) const{
    bool is_exists = QFile::exists(path + QDir::separator() + name);
    if(!is_exists){
        QFile file(path + QDir::separator() + name);
        if(!file.open(QIODevice::NewOnly)){
            QMessageBox::critical(parent, "Creating error", "Can not creating file!");
        }else{
            file.close();
        }
    }else{
        QMessageBox::critical(parent, "Creating error", "File with this name already exists!");
    }
    return !is_exists;
}

bool FileSystemWorker::CopyFile(const QString &path, QWidget *parent){
    QMimeData *data = new QMimeData();
    data->setUrls({QUrl::fromLocalFile(path)});
    clipboard_->setMimeData(data);
    return true;
}

bool FileSystemWorker::CutFile(const QString &path, QWidget *parent){
    bool result = CopyFile(path, parent);
    should_cut_ = result;
    return result;
}

bool FileSystemWorker::RenameFile(const QString &path, const QString &new_name, QWidget *parent) const{
    bool is_rename = QFile::rename(path, QFileInfo(path).dir().path() + QDir::separator() + new_name);
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
    QMimeData *data = new QMimeData();
    data->setUrls({QUrl::fromLocalFile(path)});
    clipboard_->setMimeData(data);
    return true;
}

bool FileSystemWorker::CutDir(const QString &path, QWidget *parent){
    bool result = CopyDir(path, parent);
    should_cut_ = result;
    return result;
}

bool FileSystemWorker::RenameDir(const QString &path, const QString &new_name, QWidget *parent) const{
    QDir dir(path);
    QString dir_name = dir.dirName();
    dir.cdUp();
    bool is_rename = dir.rename(dir_name, new_name);
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

//SLOTS-------------------------------------------------------------------------------------

void FileSystemWorker::clipboardChange(){
    should_cut_ = false;
}

//PRIVATE METHODS---------------------------------------------------------------------------

bool FileSystemWorker::PasteFile(const QString& file_path, const QString &paste_path, QWidget *parent){
    bool is_correct = true;
    if(should_cut_){
        is_correct = QFile::rename(file_path, paste_path+QDir::separator()+QFileInfo(file_path).fileName());
        should_cut_ = false;
    }else{
        is_correct = QFile::copy(file_path, paste_path+QDir::separator()+QFileInfo(file_path).fileName());
    }
    if(!is_correct){
        QMessageBox::critical(parent, "Paste error", "Paste error!");
    }
    return is_correct;
}

bool FileSystemWorker::PasteDir(const QString& dir_path, const QString &paste_path, QWidget *parent){
    bool is_correct = true;
    if(should_cut_){
        should_cut_ = false;
        is_correct = PasteDir(dir_path, paste_path, parent);
        QDir(dir_path).removeRecursively();
    }else{
        QDir src_dir(dir_path);
        QString root_dir_name = QFileInfo(dir_path).fileName();
        QDir dst_dir(paste_path);
        if(!dst_dir.exists(root_dir_name)){
            dst_dir.mkdir(root_dir_name);
        }else{
            QMessageBox::critical(parent, "Paste error", "Directory " + root_dir_name + " no longer exists!");
            return false;
        }
        for(const QString& d : src_dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
                QString dst_path = paste_path + QDir::separator() + root_dir_name;
                PasteDir(dir_path+ QDir::separator() + d, dst_path, parent);
        }
        for(const QString& f : src_dir.entryList(QDir::Files)){
            QFile::copy(dir_path + QDir::separator() + f, paste_path + QDir::separator() + root_dir_name + QDir::separator() + f);
        }
    }
    if(!is_correct){
        QMessageBox::critical(parent, "Paste error", "Paste error!");
    }
    return is_correct;
}
