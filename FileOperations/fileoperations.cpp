#include "fileoperations.h"
#include <QDir>
#include <QSet>
#include <QFileInfo>

QStringList FileOperations::GetSuffixesFromDir(const QString& dir_path) const{
    QSet<QString> suffix_set;
    for(const QString& file_name : QDir(dir_path).entryList(QDir::Files)){
        suffix_set.insert(QFileInfo(file_name).suffix());
    }
    return QStringList(suffix_set.begin(), suffix_set.end());
}
