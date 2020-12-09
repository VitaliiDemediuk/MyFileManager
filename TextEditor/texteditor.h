#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QWidget>
#include <QFile>
#include "searchhightlight.h"

namespace Ui {
class TextEditor;
}

class TextEditor : public QWidget
{
    Q_OBJECT

public:
    explicit TextEditor(QWidget *mwp, QWidget *parent = nullptr);
    ~TextEditor();
    bool SetFile(const QString& file_path);

protected:
//Key event
    void keyPressEvent(QKeyEvent *pe);
private slots:
    void on_WordLineEdit_textChanged(const QString &text);

    void on_WordLineEdit_textEdited(const QString &arg1);

private:
    QWidget *main_window_parent_;
    QString file_path_;
    SearchHighLight* search_highLight_;
    QByteArray plain_text_hash_;
    Ui::TextEditor *ui;
};

#endif // TEXTEDITOR_H
