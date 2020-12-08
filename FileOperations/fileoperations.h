#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include "abstractfileoperations.h"

class FileOperations : public AbstractFileOperations{
public:
    virtual QStringList GetSuffixesFromDir(const QString& dir_path) const override;
    virtual bool MergeFiles(const QString& first_path, const QString& second_path, const QString& new_path, QWidget* parent = nullptr) const override;
    virtual std::set<QString> GetHtmlTagSet(const QString &file_path, QWidget* parent = nullptr) const override;
    virtual bool ReplaceHtmlTags(const QString &file_path, const QString &old_tag, const QString &new_tag, QWidget* parent = nullptr) const override;
private:
    void ReplaceHtmlBeginTags(QString &html_text, const QString &old_tag, const QString &new_tag) const;
    void ReplaceHtmlEndTags(QString &html_text, const QString &old_tag, const QString &new_tag) const;
};

#endif // FILEOPERATIONS_H
