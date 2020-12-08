#include "hreffilelistdialog.h"
#include "ui_hreffilelistdialog.h"

#include <QCheckBox>
#include <QDebug>
#include <QFile>
#include <QRegExp>
#include <QTextDocument>
#include <QStringList>
#include <QMessageBox>

HRefFileListDialog::HRefFileListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HRefFileListDialog)
{
    ui->setupUi(this);
    check_box_list_.append(ui->BmpCheckBox);
    check_box_list_.append(ui->PngCheckBox);
    check_box_list_.append(ui->JpgCheckBox);
    check_box_list_.append(ui->JpegCheckBox);
    check_box_list_.append(ui->GifCheckBox);
    check_box_list_.append(ui->WebpCheckBox);
    check_box_list_.append(ui->SvgCheckBox);
    check_box_list_.append(ui->HeifCheckBox);
    check_box_list_.append(ui->PdfCheckBox);
    check_box_list_.append(ui->Mp4CheckBox);
    for(auto check_box : check_box_list_){
        connect(check_box, SIGNAL(clicked(bool)), this, SLOT(FileTypeCheckBoxSlot(bool)));
    }

}

HRefFileListDialog::~HRefFileListDialog(){
    delete ui;
}

void HRefFileListDialog::SetFile(const QString& file_path){
    file_path_ = file_path;
}

//SLOTS-----------------------------------------------------

void HRefFileListDialog::on_OtherTypesCheckBox_clicked(bool checked){
    ui->OtherTypesLineEdit->setEnabled(checked);
    RefreshSearchButton();
}

void HRefFileListDialog::FileTypeCheckBoxSlot(bool checked){
    if(checked){
        ++checked_checkbox_count_;
    }else{
        --checked_checkbox_count_;
    }
    if(check_box_list_.size() == checked_checkbox_count_){
        ui->AllTypesCheckBox->setChecked(true);
    }else{
        ui->AllTypesCheckBox->setChecked(false);
    }
    RefreshSearchButton();
}

void HRefFileListDialog::on_OtherTypesLineEdit_textChanged(const QString &arg1)
{
    RefreshSearchButton();
}

void HRefFileListDialog::on_AllTypesCheckBox_clicked(bool checked){
    if(checked){
        checked_checkbox_count_ = check_box_list_.size();
    }else{
        checked_checkbox_count_ = 0;
    }
    for(auto check_box : check_box_list_){
        check_box->setChecked(checked);
    }
    RefreshSearchButton();
}

void HRefFileListDialog::on_CancelPushButton_clicked(){
    this->close();
}

void HRefFileListDialog::on_SearchPushButton_clicked(){
    QFile file(file_path_);
    if(file.open(QIODevice::ReadOnly)){
        QString reg_exp_str_begin = "(<[^>]*href[\\s]*=[\\s]*\")([^>]*.(";
        QString reg_exp_str_end = "))(\"[^>]*>)";
        QString reg_exp_str_middle;
        AddStandardFileTypeSuffix(reg_exp_str_middle);
        AddOtherFileTypeSuffix(reg_exp_str_middle);
        QRegExp reg_exp(reg_exp_str_begin + reg_exp_str_middle + reg_exp_str_end);
        QString html = file.readAll();
        int res_index = reg_exp.indexIn(html);
        ui->ResultPlainTextEdit->clear();
        while(res_index != -1){
            QStringList result = reg_exp.capturedTexts();
            ui->ResultPlainTextEdit->appendPlainText(result[2]);
            res_index = reg_exp.indexIn(html, res_index+1);
        }
        file.close();
    }else{
        QMessageBox::critical(this, "Error opening", "Error opening " + file_path_ + "for reading!");
    }
}

//PRIVATE METHODS-----------------------------------------------------

void HRefFileListDialog::RefreshSearchButton(){
    bool should_be_enable = !(checked_checkbox_count_ == 0 and
            (!ui->OtherTypesCheckBox->isEnabled()
             or ui->OtherTypesLineEdit->text().isEmpty()));
    ui->SearchPushButton->setEnabled(should_be_enable);

}

void HRefFileListDialog::AddStandardFileTypeSuffix(QString &reg_exp_str_middle){
    for(auto el : check_box_list_){
        if(el->isChecked()){
            if(!reg_exp_str_middle.isEmpty()){
                reg_exp_str_middle.append('|');
            }
            reg_exp_str_middle.append(el->text());
        }
    }
}

void HRefFileListDialog::AddOtherFileTypeSuffix(QString &reg_exp_str_middle){
    if(ui->OtherTypesCheckBox->isChecked()){
        QString text = ui->OtherTypesLineEdit->text();
        text.replace(',', '|');
        if(!reg_exp_str_middle.isEmpty() and !text.isEmpty()){
            reg_exp_str_middle.append('|');
        }
        reg_exp_str_middle.append(text);
    }
}
