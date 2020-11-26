#include "osinitializercreator.h"

#include "linuxinitializerstrategy.h"
#include <QSysInfo>

std::shared_ptr<OSInitializerStrategy> OSInitializerCreator::Create(){
    std::shared_ptr<OSInitializerStrategy> result;
    if(QSysInfo::kernelType() == "linux"){
        result = std::shared_ptr<OSInitializerStrategy>(new LinuxInitializerStrategy());
    }
    return result;
}
