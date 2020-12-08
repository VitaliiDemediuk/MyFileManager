#include "filesystemwidget.h"
#include "ui_filesystemwidget.h"

#include "filesystemworker.h"
#include "fileoperations.h"
#include "hreffilelistdialog.h"
#include <QFileSystemModel>
#include <QInputDialog>
#include <QModelIndex>
#include <QDebug>
#include <QHeaderView>
#include <QDesktopServices>
#include <QUrl>
#include <QMenu>
#include <QAction>
#include <QFileInfo>

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
    ContextMenusInitialization();
    HideColumns();
    ui->DirTreeView->setAnimated(true);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setFocusProxy(ui->FileListView);
}

FileSystemWidget::~FileSystemWidget(){
    delete ui;
}

QString FileSystemWidget::GetFile() const{
    QModelIndex cur_index = ui->FileListView->currentIndex();
    if(cur_index.isValid()){
        return file_model_->filePath(cur_index);
    }else{
        return "";
    }
}

//SLOTS

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

//Context menu slots

void FileSystemWidget::slotOpenContextMenu(const QPoint &pos){
    if(ui->DirTreeView->geometry().contains(pos)){
        DirTreeViewContextMenu(pos);
    }else if(ui->FileListView->geometry().contains(pos)){
        FileListViewContextMenu(pos);
    }
}

void FileSystemWidget::slotPaste(){
    if(last_model_index_.model() == nullptr){
        file_system_worker_.Paste(file_model_->rootPath(), this);
    }else if(last_model_index_.model() == dir_model_){
        file_system_worker_.Paste(dir_model_->filePath(last_model_index_), this);
    }
}

void FileSystemWidget::slotCreateNewFile(){
    QString file_name = QInputDialog::getText(this, "New file name", "New file name");
    if(!file_name.isEmpty()){
        if(last_model_index_.model() == nullptr){
            file_system_worker_.CreateNewFile(file_model_->rootPath(), file_name, this);
        }else if(last_model_index_.model() == dir_model_){
            file_system_worker_.CreateNewFile(dir_model_->filePath(last_model_index_), file_name, this);
        }
    }
}

void FileSystemWidget::slotCopyFile(){
    file_system_worker_.CopyFile(file_model_->filePath(last_model_index_), this);
}

void FileSystemWidget::slotCutFile(){
    file_system_worker_.CutFile(file_model_->filePath(last_model_index_), this);
}

void FileSystemWidget::slotRenameFile(){
    QString dir_name = QInputDialog::getText(this, "New name", "New name");
    if(!dir_name.isEmpty()){
        file_system_worker_.RenameFile(file_model_->filePath(last_model_index_), dir_name, this);
    }
}

void FileSystemWidget::slotDeleteFile(){
    file_system_worker_.DeleteFile(file_model_->filePath(last_model_index_), this);
}

void FileSystemWidget::slotCreateNewDir(){
    QString dir_name = QInputDialog::getText(this, "New folder name", "New folder name");
    if(!dir_name.isEmpty()){
        if(last_model_index_.model() == nullptr){
            file_system_worker_.CreateNewDir(file_model_->rootPath(), dir_name, this);
        }else if(last_model_index_.model() == dir_model_){
            file_system_worker_.CreateNewDir(dir_model_->filePath(last_model_index_), dir_name, this);
        }
    }
}

void FileSystemWidget::slotCopyDir(){
    file_system_worker_.CopyDir(dir_model_->filePath(last_model_index_), this);
}

void FileSystemWidget::slotCutDir(){
    file_system_worker_.CutDir(dir_model_->filePath(last_model_index_), this);
}

void FileSystemWidget::slotRenameDir(){
    QString dir_name = QInputDialog::getText(this, "New name", "New name");
    if(!dir_name.isEmpty()){
        file_system_worker_.RenameDir(dir_model_->filePath(last_model_index_), dir_name, this);
    }
}

void FileSystemWidget::slotDeleteDir(){
    file_system_worker_.DeleteDir(dir_model_->filePath(last_model_index_), this);
}

void FileSystemWidget::slotHrefFileList(){
    HRefFileListDialog* dialog = new HRefFileListDialog(this);
    dialog->SetFile(file_model_->filePath(last_model_index_));
    dialog->show();
}

void FileSystemWidget::slotTagList(){

}

//PRIVATE METHODS--------------------------------------------------------

void FileSystemWidget::ContextMenusInitialization(){

//CREATE ACTION
    QAction *create_file_action = new QAction("New file", this);
    connect(create_file_action, SIGNAL(triggered()), this, SLOT(slotCreateNewFile()));

    QAction *create_dir_action = new QAction("New folder", this);
    connect(create_dir_action, SIGNAL(triggered()), this, SLOT(slotCreateNewDir()));

//EMPTY FILE LIST CONTEXT MENU
    empty_file_list_context_menu_ = new QMenu(this);

    empty_file_list_context_menu_->addAction(create_file_action);

    empty_file_list_context_menu_->addAction(create_dir_action);

    QAction *past_file_action = new QAction("Past", this);
    empty_file_list_context_menu_->addAction(past_file_action);
    connect(past_file_action, SIGNAL(triggered()), this, SLOT(slotPaste()));

//FILE LIST CONTEXT MENU
    file_list_context_menu_ = new QMenu(this);

    QAction *copy_file_action = new QAction("Copy file", this);
    file_list_context_menu_->addAction(copy_file_action);
    connect(copy_file_action, SIGNAL(triggered()), this, SLOT(slotCopyFile()));

    QAction *cut_file_action = new QAction("Cut file", this);
    file_list_context_menu_->addAction(cut_file_action);
    connect(cut_file_action, SIGNAL(triggered()), this, SLOT(slotCutFile()));

    QAction *rename_file_action = new QAction("Rename file", this);
    file_list_context_menu_->addAction(rename_file_action);
    connect(rename_file_action, SIGNAL(triggered()), this, SLOT(slotRenameFile()));

    QAction *delete_file_action = new QAction("Delete file", this);
    file_list_context_menu_->addAction(delete_file_action);
    connect(delete_file_action, SIGNAL(triggered()), this, SLOT(slotDeleteFile()));

    //Operations with html files
    html_sub_menu_ = new QMenu("html", this);
    QAction *href_file_list_action = new QAction("Href file list", this);
    html_sub_menu_->addAction(href_file_list_action);
    connect(href_file_list_action, SIGNAL(triggered()), this, SLOT(slotHrefFileList()));
    QAction *tag_list_action = new QAction("Tag list", this);
    html_sub_menu_->addAction(tag_list_action);
    connect(href_file_list_action, SIGNAL(triggered()), this, SLOT(slotTagList()));

//DIR TREE CONTEXT MENU
    dir_tree_context_menu_ = new QMenu(this);

    dir_tree_context_menu_->addAction(create_file_action);

    dir_tree_context_menu_->addAction(create_dir_action);

    QAction *paste_dir_action = new QAction("Past", this);
    dir_tree_context_menu_->addAction(paste_dir_action);
    connect(paste_dir_action, SIGNAL(triggered()), this, SLOT(slotPaste()));

    QAction *copy_dir_action = new QAction("Copy folder", this);
    dir_tree_context_menu_->addAction(copy_dir_action);
    connect(copy_dir_action, SIGNAL(triggered()), this, SLOT(slotCopyDir()));

    QAction *cut_dir_action = new QAction("Cut folder", this);
    dir_tree_context_menu_->addAction(cut_dir_action);
    connect(cut_dir_action, SIGNAL(triggered()), this, SLOT(slotCutDir()));

    QAction *rename_dir_action = new QAction("Rename folder", this);
    dir_tree_context_menu_->addAction(rename_dir_action);
    connect(rename_dir_action, SIGNAL(triggered()), this, SLOT(slotRenameDir()));

    QAction *delete_dir_action = new QAction("Delete folder", this);
    dir_tree_context_menu_->addAction(delete_dir_action);
    connect(delete_dir_action, SIGNAL(triggered()), this, SLOT(slotDeleteDir()));

}

void FileSystemWidget::FileListViewContextMenu(const QPoint &pos){
    if(file_model_ != nullptr){
        last_model_index_ = ui->FileListView->indexAt(ui->FileListView->mapFromParent(pos));
        if(last_model_index_.isValid()){
            if(QFileInfo(file_model_->filePath(last_model_index_)).suffix() == "html"){
                file_list_context_menu_->addMenu(html_sub_menu_);
            }else{
                file_list_context_menu_->removeAction(html_sub_menu_->menuAction());
            }
            file_list_context_menu_->exec(mapToGlobal(pos));
        }else{
            empty_file_list_context_menu_->exec(mapToGlobal(pos));
        }
    }
}

void FileSystemWidget::DirTreeViewContextMenu(const QPoint &pos){

    if(dir_model_ != nullptr){
        QPoint new_pos = ui->DirTreeView->mapFromParent(pos);
        new_pos.setY(new_pos.y() + ui->DirTreeView->header()->size().height()/2);
        last_model_index_ = ui->DirTreeView->indexAt(ui->DirTreeView->mapFromParent(new_pos));
        if(last_model_index_.isValid()){
            dir_tree_context_menu_->exec(mapToGlobal(pos));
        }
    }
}

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
