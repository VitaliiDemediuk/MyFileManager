#ifndef HREFFILELISTDIALOG_H
#define HREFFILELISTDIALOG_H

#include <QDialog>

class QCheckBox;

namespace Ui {
class HRefFileListDialog;
}

class HRefFileListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HRefFileListDialog(QWidget *parent = nullptr);
    ~HRefFileListDialog();
    void SetFile(const QString& file_path);

private slots:
    void FileTypeCheckBoxSlot(bool checked);
    void on_OtherTypesCheckBox_clicked(bool checked);

    void on_AllTypesCheckBox_clicked(bool checked);

    void on_CancelPushButton_clicked();

    void on_OtherTypesLineEdit_textChanged(const QString &arg1);

    void on_SearchPushButton_clicked();

private:
    Ui::HRefFileListDialog *ui;
    QString file_path_;
    QList<QCheckBox*> check_box_list_;
    int checked_checkbox_count_ = 0;
    void RefreshSearchButton();
    void AddStandardFileTypeSuffix(QString &reg_exp_str_middle);
    void AddOtherFileTypeSuffix(QString &reg_exp_str_middle);
};

#endif // HREFFILELISTDIALOG_H
