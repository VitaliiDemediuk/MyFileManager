#include "filesystemwidget.h"
#include "ui_filesystemwidget.h"

#include "filesystemworker.h"
#include "fileoperations.h"
#include <QFileSystemModel>
#include <QModelIndex>
#include <QDebug>
#include <QHeaderView>
#include <QDesktopServices>
#include <QUrl>
#include <QMenu>
#include <QAction>

FileSystemWidget::FileSystemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileSystemWidget),
    file_system_worker_(FileSystemWorker::GetInstance()),
    file_operations_(new FileOperations())
{
    ui->setupUi(this);
    os_initializer_ = OSInitializerCreator::Create();
    FillRootsComboBox();
    ChangeDirModelRootPath(ui->RootsComboBox->itemText(0));
    HideColumns();
    ui->DirTreeView->setAnimated(true);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
}

FileSystemWidget::~FileSystemWidget(){
    delete ui;
}

void FileSystemWidget::on_RootsComboBox_currentTextChanged(const QString &root_path){
    ChangeDirModelRootPath(root_path);
}

void FileSystemWidget::on_SuffixComboBox_textActivated(const QString &suffix){
    file_model_->setNameFilterDisables(false);
    SetFilter(suffix);
}

void FileSystemWidget::on_DirTreeView_collapsed(const QModelIndex &index){
    ui->DirTreeView->resizeColumnToContents(0);
    ChangeFileModelPath(dir_model_->filePath(index));
}

void FileSystemWidget::on_DirTreeView_expanded(const QModelIndex &index){
    ui->DirTreeView->resizeColumnToContents(0);
    ChangeFileModelPath(dir_model_->filePath(index));
}

void FileSystemWidget::on_DirTreeView_activated(const QModelIndex &index){
    if(ui->DirTreeView->isExpanded(index)){
        ChangeFileModelPath(dir_model_->filePath(index));
    }else{
        ui->DirTreeView->expand(index);
    }
}

void FileSystemWidget::on_FileListView_doubleClicked(const QModelIndex &index){
    QDesktopServices::openUrl(QUrl::fromLocalFile(file_model_->filePath(index)));
}

//PRIVATE METHODS--------------------------------------------------------

void FileSystemWidget::HideColumns(){
    ui->DirTreeView->setColumnHidden(1, true);
    ui->DirTreeView->setColumnHidden(2, true);
    ui->DirTreeView->setColumnHidden(3, true);
}

void FileSystemWidget::FillRootsComboBox(){
    for(auto& root : os_initializer_->GetRootsForComboBox()){
        ui->RootsComboBox->addItem(root);
    }
}

void FileSystemWidget::SetFilter(const QString& suffix){
    if(suffix != "All files"){
        file_model_->setNameFilters({"*."+suffix});
    }else{
        file_model_->setNameFilters({"*"});
    }
}

void FileSystemWidget::ChangeFileModelPath(const QString &path){
    if(file_model_ != nullptr){
        delete this->file_model_;
    }
    file_model_ = new QFileSystemModel(this);
    file_model_->setFilter(QDir::Files);
    ui->FileListView->setModel(file_model_);
    QModelIndex file_model_index = file_model_->setRootPath(path);
    ui->FileListView->setRootIndex(file_model_index);
    ui->PathLineEdit->setText(path);
    RefreshSuffixComboBox(path);
}

void FileSystemWidget::ChangeDirModelRootPath(const QString &root_path){
    if(dir_model_ == nullptr){
        dir_model_ = new QFileSystemModel();
        dir_model_->setFilter(QDir::Dirs | QDir::NoDot
                              | QDir::NoDotDot);
        ui->DirTreeView->setModel(dir_model_);
    }
    QModelIndex dir_model_index = dir_model_->setRootPath(root_path);
    ui->DirTreeView->setRootIndex(dir_model_index);
    ui->DirTreeView->clicked(dir_model_->index(0, 0));
}

void FileSystemWidget::RefreshSuffixComboBox(const QString& dir_path){
    ui->SuffixComboBox->clear();
    ui->SuffixComboBox->addItem("All files");
    ui->SuffixComboBox->addItems(file_operations_->GetSuffixesFromDir(dir_path));
}
