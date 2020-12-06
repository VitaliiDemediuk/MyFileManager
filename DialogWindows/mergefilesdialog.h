#ifndef MERGEFILESDIALOG_H
#define MERGEFILESDIALOG_H

#include <QDialog>

class AbstractFileOperations;

namespace Ui {
class MergeFilesDialog;
}

class MergeFilesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MergeFilesDialog(QWidget *parent = nullptr);
    ~MergeFilesDialog();

private slots:
    void on_FirstFilePushButton_clicked();

    void on_SecondFilePushButton_clicked();

    void on_NewFilePushButton_clicked();

    void on_MergePushButton_clicked();

private:
    Ui::MergeFilesDialog *ui;
    AbstractFileOperations* file_operation_;
    void RefreshMergeButton();
};

#endif // MERGEFILESDIALOG_H
