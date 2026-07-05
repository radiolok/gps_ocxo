#pragma once

#include "../display/ilc1_8.h"

/*
 * Демо: часы на 7-сегментных блоках.
 * Отображает счётчик времени HH:MM на левом блоке и SS:ms на правом.
 * При отсутствии карты сегментов — заглушка (цифры не будут соответствовать
 * реальным позициям, но код логически верен).
 */

namespace demo {

class DemoClock {
public:
    DemoClock(ILC1_8Display& display);

    void begin();
    void run();

private:
    ILC1_8Display& _disp;
    uint8_t        _hours;
    uint8_t        _minutes;
    uint8_t        _seconds;
    uint8_t        _hundredths;  // сотые доли секунды
    unsigned long  _lastTick;
};

} // namespace demo
