#pragma once

#include "../display/ilc1_8.h"

/*
 * Демо: полный тест всех сегментов.
 * Все эффекты исполняются одновременно:
 *   1. Секундное кольцо — бегущий сегмент (20мс)
 *   2. 7-сегментные блоки — перебор 0..9 на всех разрядах (100мс)
 *   3. Текстовые плашки — поочерёдное включение (500мс)
 *   4. Диск подсветки — мигание (300мс)
 */

namespace demo {

class DemoTestAll {
public:
    DemoTestAll(ILC1_8Display& display);

    void begin();
    void run();

private:
    ILC1_8Display& _disp;

    // Секундное кольцо
    uint8_t        _sec;
    unsigned long  _lastSec;

    // 7-сегментные блоки
    uint8_t        _digit;
    unsigned long  _lastDigit;

    // Плашки
    uint8_t        _labelIdx;
    unsigned long  _lastLabel;

    // Диск
    bool           _diskState;
    unsigned long  _lastDisk;
};

} // namespace demo
