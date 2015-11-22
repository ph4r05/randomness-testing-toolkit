#include "rtt/interfacecreator.h"

namespace rtt {

batteries::Interface *InterfaceCreator::createBattery(int batteryConst) {
    switch(batteryConst) {
    case Constants::BATTERY_DIEHARDER:
        return new batteries::Dieharder();
        break;
    case Constants::BATTERY_NIST_STS:
        return new batteries::NistSts();
        break;
    case Constants::BATTERY_TU01_SMALLCRUSH:
        //throw std::runtime_error("TU01 Small Crush battery is not yet implemented");
        return new batteries::TestU01(Constants::BATTERY_TU01_SMALLCRUSH);
        break;
    case Constants::BATTERY_TU01_CRUSH:
        //throw std::runtime_error("TU01 Crush battery is not yet implemented");
        return new batteries::TestU01(Constants::BATTERY_TU01_CRUSH);
        break;
    case Constants::BATTERY_TU01_BIGCRUSH:
        //throw std::runtime_error("TU01 Big Crush battery is not yet implemented");
        return new batteries::TestU01(Constants::BATTERY_TU01_BIGCRUSH);
        break;
    case Constants::BATTERY_EACIRC:
        throw std::runtime_error("EACirc battery is not yet implemented");
        break;
    default:
        throw std::runtime_error("unknown statistical battery requested");
        break;
    }
    return NULL;
}

void InterfaceCreator::destroyBattery(batteries::Interface * battery) {
    delete battery;
}

} // namespace rtt