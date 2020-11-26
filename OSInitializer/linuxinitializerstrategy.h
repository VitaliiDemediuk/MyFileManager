#ifndef LINUXINITIALIZERSTRATEGY_H
#define LINUXINITIALIZERSTRATEGY_H

#include "osinitializerstrategy.h"

class LinuxInitializerStrategy : public OSInitializerStrategy {
public:
    explicit LinuxInitializerStrategy() = default;
    QVector<QString> GetRootsForComboBox() const override;
    ~LinuxInitializerStrategy() override;
};

#endif // LINUXINITIALIZERSTRATEGY_H
