#ifndef FILESYSTEMWIDGET_H
#define FILESYSTEMWIDGET_H

#include <QWidget>

namespace Ui {
class FileSystemWidget;
}

class FileSystemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileSystemWidget(QWidget *parent = nullptr);
    ~FileSystemWidget();

private:
    Ui::FileSystemWidget *ui;
};

#endif // FILESYSTEMWIDGET_H
