#ifndef ABSTRACTFILEOPERATIONS_H
#define ABSTRACTFILEOPERATIONS_H

#include <QString>
#include <QStringList>

class AbstractFileOperations{
public:
    virtual ~AbstractFileOperations() = 0;
    virtual QStringList GetSuffixesFromDir(const QString& dir_path) const = 0;
};

#endif // ABSTRACTFILEOPERATIONS_H
