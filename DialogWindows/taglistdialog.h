#ifndef TAGLISTDIALOG_H
#define TAGLISTDIALOG_H

#include <QDialog>
#include "abstractfileoperations.h"

namespace Ui {
class TagListDialog;
}

class TagListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagListDialog(QWidget *parent = nullptr);
    ~TagListDialog();
    void SetFile(const QString& file_name);

private slots:
    void on_CancelPushButton_clicked();

    void on_NewTagLineEdit_textChanged(const QString &text);

    void on_ReplacePushButton_clicked();

private:
    QString file_path_;
    Ui::TagListDialog *ui;
    std::shared_ptr<AbstractFileOperations> file_operations_;
    std::set<QString> tag_set_;
};

#endif // TAGLISTDIALOG_H
