#pragma once

#include "../display/ilc1_8.h"

/*
 * Демо: быстрый перебор электродов.
 *
 * Последовательно зажигает по одному биту (0..127) с интервалом 10мс,
 * в бесконечном цикле. Позволяет визуально убедиться, что каждый выход
 * SN755870 работает.
 */

namespace demo {

class DemoScan {
public:
    DemoScan(ILC1_8Display& display);

    void begin();
    void run();

private:
    ILC1_8Display& _disp;
    uint8_t        _currentBit;
    unsigned long  _lastUpdate;
};

} // namespace demo
