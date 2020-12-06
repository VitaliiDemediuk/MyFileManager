#include "fileoperations.h"
#include <QDir>
#include <QSet>
#include <QWidget>
#include <QFileInfo>
#include <QMessageBox>


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
