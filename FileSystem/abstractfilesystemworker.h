#ifndef ABSTRACTFILESYSTEMWORKER_H
#define ABSTRACTFILESYSTEMWORKER_H

#include <QString>
#include <QObject>
class QWidget;

class AbstractFileSystemWorker : protected QObject{
public:
    AbstractFileSystemWorker();
    virtual ~AbstractFileSystemWorker() = 0;
//General operations
    virtual bool Paste(const QString &path, QWidget *parent) = 0;
//File's standard operations
    virtual bool CreateNewFile(const QString &path, const QString &name, QWidget *parent) const = 0;
    virtual bool CopyFile(const QString &path, QWidget *parent) = 0;
    virtual bool CutFile(const QString &path, QWidget *parent) = 0;
    virtual bool RenameFile(const QString &path, const QString &old_name, const QString &new_name, QWidget *parent) const = 0;
    virtual bool DeleteFile(const QString &path, QWidget *parent) const = 0;
//Directory's standard operations
    virtual bool CreateNewDir(const QString &path, const QString &name, QWidget *parent) const = 0;
    virtual bool CopyDir(const QString &path, QWidget *parent) = 0;
    virtual bool CutDir(const QString &path, QWidget *parent) = 0;
    virtual bool RenameDir(const QString &path, const QString &old_name, const QString &new_name, QWidget *parent) const = 0;
    virtual bool DeleteDir(const QString &path, QWidget *parent) const = 0;
};

#endif // ABSTRACTFILESYSTEMWORKER_H
