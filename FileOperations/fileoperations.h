#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include "abstractfileoperations.h"

class FileOperations : public AbstractFileOperations{
public:
    virtual QStringList GetSuffixesFromDir(const QString& dir_path) const;
};

#endif // FILEOPERATIONS_H
