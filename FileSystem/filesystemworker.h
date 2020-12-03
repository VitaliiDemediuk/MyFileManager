#ifndef FILESYSTEMWORKER_H
#define FILESYSTEMWORKER_H

#include "abstractfilesystemworker.h"
#include <QString>
#include <QObject>
#include <QGuiApplication>

enum class ClipBoardType{
    kFile = 0,
    kDir,
    kNone
};

class FileSystemWorker : public AbstractFileSystemWorker{
    Q_OBJECT
public:
    static AbstractFileSystemWorker& GetInstance();
    FileSystemWorker( const FileSystemWorker&) = delete;
    FileSystemWorker& operator=( FileSystemWorker&) = delete;
//General operations
    bool Paste(const QString &path, QWidget *parent) override;
//File's standard operations
    bool CreateNewFile(const QString &path, const QString &name, QWidget *parent) const override;
    bool CopyFile(const QString &path, QWidget *parent) override;
    bool CutFile(const QString &path, QWidget *parent) override;
    bool RenameFile(const QString &path, const QString &new_name, QWidget *parent) const override;
    bool DeleteFile(const QString &path, QWidget *parent) const override;
//Directory's standard operations
    bool CreateNewDir(const QString &path, const QString &name, QWidget *parent) const override;
    bool CopyDir(const QString &path, QWidget *parent) override;
    bool CutDir(const QString &path, QWidget *parent) override;
    bool RenameDir(const QString &path, const QString &new_name, QWidget *parent) const override;
    bool DeleteDir(const QString &path, QWidget *parent) const override;
private slots:
    void clipboardChange();
private:
    FileSystemWorker();
    bool PasteFile(const QString &file_path, const QString &paste_path, QWidget *parent);
    bool PasteDir(const QString &dir_path, const QString &paste_path, QWidget *parent);
    QClipboard *clipboard_ = QGuiApplication::clipboard();
    bool should_cut_ = false;
};

#endif // FILESYSTEMWORKER_H
