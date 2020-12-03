#include "texteditor.h"
#include "ui_texteditor.h"
#include <QKeyEvent>
#include <QApplication>
#include <QStyle>
#include <QFile>
#include <QMessageBox>
#include <QDebug>

TextEditor::TextEditor(QWidget *mwp, QWidget *parent) :
    QWidget(parent),
    main_window_parent_(mwp),
    ui(new Ui::TextEditor)
{
    ui->setupUi(this);
}

TextEditor::~TextEditor()
{
    delete ui;
}

bool TextEditor::SetFile(const QString& file_path){
    bool result = true;
    file_path_ = file_path;
    this->setWindowTitle(file_path);
    QFile file(file_path);
    if(file.open(QIODevice::ReadOnly)){
        ui->textEdit->setText(file.readAll());
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
        if(QMessageBox::question(this, "Save", "Do you want to save the changes?") == QMessageBox::Yes){
            QFile file(file_path_);
            if(file.open(QIODevice::WriteOnly)){
                file.write(ui->textEdit->toPlainText().toUtf8());
            }
        }
        main_window_parent_->setGeometry(this->x(), this->y()+37, this->width(), this->height());
        main_window_parent_->show();
        this->hide();
    }
}
