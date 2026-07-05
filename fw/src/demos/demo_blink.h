#pragma once

#include "../display/ilc1_8.h"

/*
 * Демо: периодическое включение/выключение всех сегментов.
 *
 * Все 128 выходов SN755870 одновременно переводятся в HIGH на 250мс,
 * затем в LOW на 250мс. Управление — через запись единиц/нулей
 * в регистры (фреймбуфер), без использования дополнительных пинов.
 *
 * Назначение: проверка работоспособности микросхем SN755870
 * при напряжении VDDH = 30В.
 */

namespace demo {

class DemoBlink {
public:
    DemoBlink(ILC1_8Display& display);

    void begin();
    void run();

private:
    ILC1_8Display& _disp;
    bool           _state;       // true = все горят, false = все погашены
    unsigned long  _lastToggle;
};

} // namespace demo
