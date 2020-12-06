#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include "abstractfileoperations.h"

class FileOperations : public AbstractFileOperations{
public:
    virtual QStringList GetSuffixesFromDir(const QString& dir_path) const override;
    virtual bool MergeFiles(const QString& first_path, const QString& second_path, const QString& new_path, QWidget* parent) const override;
};

#endif // FILEOPERATIONS_H
