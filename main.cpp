#include "mainwindow.h"

#include <QApplication>
#include "filesystemworker.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    FileSystemWorker::GetInstance()->CopyDir("/home/vitalii/sss", nullptr);
    FileSystemWorker::GetInstance()->Paste("/home/vitalii/Desktop", nullptr);
    return a.exec();
}
