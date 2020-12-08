#include "taglistdialog.h"
#include "ui_taglistdialog.h"
#include "fileoperations.h"

TagListDialog::TagListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TagListDialog),
    file_operations_(new FileOperations())
{
    ui->setupUi(this);
}

TagListDialog::~TagListDialog(){
    delete ui;
}

void TagListDialog::SetFile(const QString& file_name){
    file_path_ = file_name;
    tag_set_ = file_operations_->GetHtmlTagSet(file_path_, this);
    for(const auto& tag : tag_set_){
        ui->TagListPlainTextEdit->appendPlainText(tag);
        ui->TagListComboBox->addItem(tag);
    }
    ui->NewTagLineEdit->setEnabled(!tag_set_.empty());
    this->setWindowTitle("Tag List (" + file_path_ + ")");
}

//SLOTS-----------------------------------------------------

void TagListDialog::on_CancelPushButton_clicked(){
    this->close();
}

void TagListDialog::on_NewTagLineEdit_textChanged(const QString &text){
        ui->ReplacePushButton->setEnabled(!text.isEmpty());
}

void TagListDialog::on_ReplacePushButton_clicked()
{
    QString old_tag = ui->TagListComboBox->currentText();
    QString new_tag = ui->NewTagLineEdit->text();
    file_operations_->ReplaceHtmlTags(file_path_, old_tag, new_tag);
    ui->TagListComboBox->setItemText(ui->TagListComboBox->currentIndex(), ui->NewTagLineEdit->text());
    ui->TagListPlainTextEdit->clear();
    ui->TagListComboBox->clear();
    tag_set_.erase(old_tag);
    tag_set_.insert(new_tag);
    for(const auto& tag : tag_set_){
        ui->TagListPlainTextEdit->appendPlainText(tag);
        ui->TagListComboBox->addItem(tag);
    }
    ui->TagListComboBox->setCurrentText(new_tag);
}
