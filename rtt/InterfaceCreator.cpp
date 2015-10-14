#include <InterfaceCreator.h>

StatBatteryInterface * InterfaceCreator::createBattery(int batteryConst) {
    switch(batteryConst) {
        case BATTERY_DIEHARDER:
            return new DieharderBattery();
            break;
        case BATTERY_NIST_STS:
            throw std::runtime_error("NIST STS battery is not yet implemented");
            break;
        case BATTERY_TU01_SMALLCRUSH:
            throw std::runtime_error("TU01 Small Crush battery is not yet implemented");
            break;
        case BATTERY_TU01_CRUSH:
            throw std::runtime_error("TU01 Crush battery is not yet implemented");
            break;
        case BATTERY_TU01_BIGCRUSH:
            throw std::runtime_error("TU01 Big Crush battery is not yet implemented");
            break;
        case BATTERY_EACIRC:
            throw std::runtime_error("EACirc battery is not yet implemented");
            break;
        default:
            throw std::runtime_error("unknown statistical battery requested");
            break;
        }
    return NULL;
}

void InterfaceCreator::destroyBattery(StatBatteryInterface * battery) {
    delete battery;
}