#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filesystemworker.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_FirstFileSystemWidget_customContextMenuRequested(const QPoint &pos){
    ui->FirstFileSystemWidget->slotOpenContextMenu(pos);
}

void MainWindow::on_SecondFileSystemWidget_customContextMenuRequested(const QPoint &pos){
    ui->SecondFileSystemWidget->slotOpenContextMenu(pos);
}
