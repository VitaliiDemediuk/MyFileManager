#ifndef OSINITIALIZERSTRATEGY_H
#define OSINITIALIZERSTRATEGY_H

#include <QString>

class OSInitializerStrategy{
public:
    OSInitializerStrategy() = default;
    virtual QVector<QString> GetRootsForComboBox() const = 0;
    virtual ~OSInitializerStrategy() = 0;
};

#endif // OSINITIALIZERSTRATEGY_H
