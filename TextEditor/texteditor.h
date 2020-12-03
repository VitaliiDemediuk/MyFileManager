#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QWidget>
#include <QFile>

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
private:
    QWidget *main_window_parent_;
    QString file_path_;
    Ui::TextEditor *ui;
};

#endif // TEXTEDITOR_H
