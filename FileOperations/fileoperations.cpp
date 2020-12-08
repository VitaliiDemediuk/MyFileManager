#include "fileoperations.h"
#include <QDir>
#include <QSet>
#include <QWidget>
#include <QFileInfo>
#include <QMessageBox>
#include <set>


QStringList FileOperations::GetSuffixesFromDir(const QString& dir_path) const{
    QSet<QString> suffix_set;
    for(const QString& file_name : QDir(dir_path).entryList(QDir::Files)){
        suffix_set.insert(QFileInfo(file_name).suffix());
    }
    return QStringList(suffix_set.begin(), suffix_set.end());
}

bool FileOperations::MergeFiles(const QString& first_path, const QString& second_path, const QString& new_path, QWidget* parent) const{
    bool have_not_error = true;
    QFile first_file(first_path);
    have_not_error = first_file.open(QIODevice::ReadOnly);
    if(have_not_error){
        QFile second_file(second_path);
        second_file.open(QIODevice::ReadOnly);
        if(have_not_error){
            QFile new_file(new_path);
            have_not_error = new_file.open(QIODevice::WriteOnly);
            if(have_not_error){
                new_file.write(first_file.readAll());
                new_file.write(second_file.readAll());
            }else{
                QMessageBox::critical(parent, "Error", "Can not open " + first_path + "!");
            }
        }else{
            QMessageBox::critical(parent, "Error", "Can not open " + second_path + "!");
        }
    }else{
        QMessageBox::critical(parent, "Error", "Can not open " + new_path + "!");
    }
    return have_not_error;
}

std::set<QString> FileOperations::GetHtmlTagSet(const QString &file_path, QWidget* parent) const{
    std::set<QString> result;
    QRegExp reg_exp("<([^>^\\/^\\s]+)");
    QFile file(file_path);
    if(file.open(QIODevice::ReadOnly)){
        QString html = file.readAll();
        file.close();
        int res_index = reg_exp.indexIn(html);
        while(res_index != -1){
            QStringList parsing_result = reg_exp.capturedTexts();
            if(parsing_result[1] != "!DOCTYPE"){
                result.insert(parsing_result[1]);
            }
            res_index = reg_exp.indexIn(html, res_index+1);
        }
    }else{
        QMessageBox::critical(parent, "Error opening", "Error opening " + file_path + "for reading!");
    }
    return result;
}

bool FileOperations::ReplaceHtmlTags(const QString &file_path, const QString &old_tag, const QString &new_tag, QWidget* parent) const{
    QFile file(file_path);
    bool have_not_error = true;
    if(file.open(QIODevice::ReadOnly | QIODevice::Text) and QFile::exists(file_path)){
        QString html_text = file.readAll();
        file.close();
        ReplaceHtmlBeginTags(html_text, old_tag, new_tag);
        ReplaceHtmlEndTags(html_text, old_tag, new_tag);
        if(file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
            file.write(html_text.toUtf8());
        }
        file.close();
    }else{
        QMessageBox::critical(parent, "Error opening", "Error opening " + file_path + "for reading!");
        have_not_error = false;
    }
    return have_not_error;
}

//PRIVATE--------------------------------------------------------------------

void FileOperations::ReplaceHtmlBeginTags(QString &html_text, const QString &old_tag, const QString &new_tag) const{
    html_text.replace(QRegExp(QString("<[\\s]*%1([^>]*)>").arg(old_tag)), QString("<%1\\1>").arg(new_tag));
}

void FileOperations::ReplaceHtmlEndTags(QString &html_text, const QString &old_tag, const QString &new_tag) const{
    html_text.replace(QRegExp(QString("<[\\s]*\\/[\\s]*%1([^>]*)>").arg(old_tag)), QString("</%1\\1>").arg(new_tag));
}
