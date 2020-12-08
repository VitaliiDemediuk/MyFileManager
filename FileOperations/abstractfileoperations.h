#ifndef ABSTRACTFILEOPERATIONS_H
#define ABSTRACTFILEOPERATIONS_H

#include <QString>
#include <QStringList>
#include <set>

class QWidget;

class AbstractFileOperations{
public:
    virtual ~AbstractFileOperations() = 0;
    virtual QStringList GetSuffixesFromDir(const QString& dir_path) const = 0;
    virtual bool MergeFiles(const QString& first_path, const QString& second_path, const QString& new_path, QWidget* parent = nullptr) const = 0;
    virtual std::set<QString> GetHtmlTagSet(const QString &file_path, QWidget* parent = nullptr) const = 0;
    virtual bool ReplaceHtmlTags(const QString &file_path, const QString &old_tag, const QString &new_tag, QWidget* parent= nullptr) const = 0;
};

#endif // ABSTRACTFILEOPERATIONS_H
