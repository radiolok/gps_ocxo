#pragma once

#include "../display/ilc1_8.h"

/*
 * Демо: бегущая секундная стрелка.
 * Последовательно зажигает сегменты s1..s60 с интервалом ~16мс,
 * имитируя движение секундной стрелки часов.
 * Полный круг ~ 1 секунда (60 * 16.67ms ≈ 1000ms).
 */

namespace demo {

class DemoSeconds {
public:
    DemoSeconds(ILC1_8Display& display);

    void begin();
    void run();   // вызывать в loop(), сам отслеживает тайминг

private:
    ILC1_8Display& _disp;
    uint8_t        _current;     // текущий сегмент 1..60
    unsigned long  _lastUpdate;
};

} // namespace demo
