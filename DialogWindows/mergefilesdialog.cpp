#include "mergefilesdialog.h"
#include "ui_mergefilesdialog.h"
#include "fileoperations.h"
#include <QMessageBox>

#include <QFileDialog>

MergeFilesDialog::MergeFilesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MergeFilesDialog),
    file_operation_(new FileOperations)
{
    ui->setupUi(this);
}

MergeFilesDialog::~MergeFilesDialog(){
    delete ui;
}

void MergeFilesDialog::on_FirstFilePushButton_clicked(){
    QString file_path = QFileDialog::getOpenFileName();
    ui->FirstFileLineEdit->setText(file_path);
    RefreshMergeButton();
}

void MergeFilesDialog::on_SecondFilePushButton_clicked(){
    QString file_path = QFileDialog::getOpenFileName();
    ui->SecondFileLineEdit->setText(file_path);
    RefreshMergeButton();
}

void MergeFilesDialog::on_NewFilePushButton_clicked(){
    QString file_path = QFileDialog::getSaveFileName();
    ui->NewFileLineEdit->setText(file_path);
    RefreshMergeButton();
}

void MergeFilesDialog::RefreshMergeButton(){
    if(!ui->FirstFileLineEdit->text().isEmpty() and
       !ui->SecondFileLineEdit->text().isEmpty() and
       !ui->NewFileLineEdit->text().isEmpty())
    {
        ui->MergePushButton->setEnabled(true);
    }else{
        ui->MergePushButton->setEnabled(false);
    }
}

void MergeFilesDialog::on_MergePushButton_clicked(){
    bool successful_merge = file_operation_->MergeFiles(ui->FirstFileLineEdit->text(), ui->SecondFileLineEdit->text(), ui->NewFileLineEdit->text(), this);
    if(successful_merge){
        QMessageBox::information(this, "Info", "Files merged successfully");
    }
}
