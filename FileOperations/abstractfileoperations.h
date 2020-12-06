#ifndef ABSTRACTFILEOPERATIONS_H
#define ABSTRACTFILEOPERATIONS_H

#include <QString>
#include <QStringList>

class QWidget;

class AbstractFileOperations{
public:
    virtual ~AbstractFileOperations() = 0;
    virtual QStringList GetSuffixesFromDir(const QString& dir_path) const = 0;
    virtual bool MergeFiles(const QString& first_path, const QString& second_path, const QString& new_path, QWidget* parent) const = 0;
};

#endif // ABSTRACTFILEOPERATIONS_H
