#pragma once

#include "../display/ilc1_8.h"

/*
 * Демо: параллельный тест всех групп сегментов.
 *
 * Три одновременных процесса:
 *   1. Секундное кольцо — последовательный перебор s1..s60, 10мс/сегмент
 *   2. 7-сегментные блоки — все 8 разрядов синхронно перебирают a,b,c,d,e,f,g, 100мс/сегмент
 *   3. Диск подсветки + плашки (дата, тв, пв, ввод, сек) — моргают с частотой 50мс
 */

namespace demo {

class DemoFullTest {
public:
    DemoFullTest(ILC1_8Display& display);

    void begin();
    void run();

private:
    ILC1_8Display& _disp;

    // Секундное кольцо
    uint8_t        _secCurrent;   // 1..60
    unsigned long  _lastSec;

    // 7-сегментные блоки
    uint8_t        _digCurrent;   // 0..6 (a..g)
    unsigned long  _lastDig;

    // Плашки + диск
    bool           _blinkState;
    unsigned long  _lastBlink;
};

} // namespace demo
