#include "linuxinitializerstrategy.h"

#include <QVector>
#include <QDir>

QVector<QString> LinuxInitializerStrategy::GetRootsForComboBox() const{
    static const QVector<QString> list {QDir::homePath(), "/"};
    return list;
}

LinuxInitializerStrategy::~LinuxInitializerStrategy(){}
