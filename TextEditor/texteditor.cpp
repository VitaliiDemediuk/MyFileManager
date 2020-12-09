#include "texteditor.h"
#include "ui_texteditor.h"
#include <QKeyEvent>
#include <QApplication>
#include <QStyle>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QCryptographicHash>
#include <QByteArray>

#include "searchhightlight.h"

TextEditor::TextEditor(QWidget *mwp, QWidget *parent) :
    QWidget(parent),
    main_window_parent_(mwp),
    ui(new Ui::TextEditor)
{
    ui->setupUi(this);
    search_highLight_ = new SearchHighLight(ui->PlainTextEdit->document());
}

TextEditor::~TextEditor(){
    delete ui;
}

bool TextEditor::SetFile(const QString& file_path){
    bool result = true;
    file_path_ = file_path;
    this->setWindowTitle(file_path);
    QFile file(file_path);
    if(file.open(QIODevice::ReadOnly)){
        QString plain_text = file.readAll();
        plain_text_hash_ = QCryptographicHash::hash(plain_text.toUtf8(), QCryptographicHash::Md5);
        ui->PlainTextEdit->insertPlainText(std::move(plain_text));
        result = true;
    }else{
        QMessageBox::critical(main_window_parent_, "Opening error", "Can not open " + file_path);
        result = false;
    }
    file.close();
    return result;
}

void TextEditor::keyPressEvent(QKeyEvent *pe){
    if(pe->key() == Qt::Key_F4){
        QByteArray new_plain_text = ui->PlainTextEdit->toPlainText().toUtf8();
        QByteArray new_plain_text_hash = QCryptographicHash::hash(new_plain_text, QCryptographicHash::Md5);
        if(plain_text_hash_ != new_plain_text_hash){
            if(QMessageBox::question(this, "Save", "Do you want to save the changes?") == QMessageBox::Yes){
                QFile file(file_path_);
                if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
                    file.write(new_plain_text);
                    file.close();
                }
            }
        }
        main_window_parent_->setGeometry(this->x(), this->y()+37, this->width(), this->height());
        main_window_parent_->show();
        this->hide();
    }
}

//SLOTS-----------------------------------------------------------------

void TextEditor::on_WordLineEdit_textChanged(const QString &text){
    search_highLight_->searchText(text);
}

void TextEditor::on_WordLineEdit_textEdited(const QString &text){
    if(!text.isEmpty()){
        search_highLight_->searchText(text);
    }
}
