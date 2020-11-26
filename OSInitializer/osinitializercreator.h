#ifndef OSINITIALIZERCREATOR_H
#define OSINITIALIZERCREATOR_H

#include "osinitializerstrategy.h"
#include <memory>

class OSInitializerCreator{
public:
    OSInitializerCreator() = delete;
    OSInitializerCreator(const OSInitializerCreator&) = delete;
    OSInitializerCreator(OSInitializerCreator&&) = delete;
    OSInitializerCreator& operator= (OSInitializerCreator&) = delete;
    static std::shared_ptr<OSInitializerStrategy> Create();
    ~OSInitializerCreator() = delete;
};

#endif // OSINITIALIZERCREATOR_H
