#pragma once

#include "../display/ilc1_8.h"

/*
 * Демо: калибровка секундного кольца.
 *
 * Медленный (500мс) перебор S1..S60 с выводом в Serial.
 * Позволяет сопоставить физическую позицию на индикаторе
 * с номером S в прошивке.
 *
 * Для каждого шага выводится: "S=N  bit=B"
 * Запишите соответствие: физическая позиция (цифра на циферблате) -> S номер.
 */

namespace demo {

class DemoCalibrate {
public:
    DemoCalibrate(ILC1_8Display& display);

    void begin();
    void run();

private:
    ILC1_8Display& _disp;
    uint8_t        _current;
    unsigned long  _lastUpdate;
    bool           _firstStep;
};

} // namespace demo
