#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TextEditor;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_FirstFileSystemWidget_customContextMenuRequested(const QPoint &pos);
    void on_SecondFileSystemWidget_customContextMenuRequested(const QPoint &pos);

protected:
//Key event
    void keyPressEvent(QKeyEvent *pe);
private:
    Ui::MainWindow *ui;
    TextEditor *text_editor_widget_;
    void OpenTextEditor(const QString& file_path);

};
#endif // MAINWINDOW_H
