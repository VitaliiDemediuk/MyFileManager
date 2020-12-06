#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filesystemworker.h"
#include "texteditor.h"
#include "mergefilesdialog.h"
#include <QDebug>
#include <QRect>
#include <QKeyEvent>
#include <QPropertyAnimation>
#include <QApplication>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , text_editor_widget_(new TextEditor(this, parent))
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete text_editor_widget_;
}

void MainWindow::on_FirstFileSystemWidget_customContextMenuRequested(const QPoint &pos){
    ui->FirstFileSystemWidget->slotOpenContextMenu(pos);
}

void MainWindow::on_SecondFileSystemWidget_customContextMenuRequested(const QPoint &pos){
    ui->SecondFileSystemWidget->slotOpenContextMenu(pos);
}


void MainWindow::keyPressEvent(QKeyEvent *pe){
    if(pe->key() == Qt::Key_F4){
        QString file_path;
        if(ui->FirstFileSystemWidget->hasFocus()){
            file_path = ui->FirstFileSystemWidget->GetFile();
        } else if(ui->SecondFileSystemWidget->hasFocus()){
            file_path = ui->SecondFileSystemWidget->GetFile();
        }
        if(!file_path.isEmpty()){
            OpenTextEditor(file_path);
        }
    }
}

void MainWindow::OpenTextEditor(const QString& file_path){
    if(text_editor_widget_->SetFile(file_path)){
        text_editor_widget_->setGeometry(this->x(), this->y()+37, this->width(), this->height());
        text_editor_widget_->show();
        this->hide();
    }
}

void MainWindow::on_actionMarge_file_triggered()
{
    MergeFilesDialog *dialog = new MergeFilesDialog(this);
    dialog->show();
}
