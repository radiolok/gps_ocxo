#include "demo_clock.h"

namespace demo {

DemoClock::DemoClock(ILC1_8Display& display)
    : _disp(display), _hours(0), _minutes(0), _seconds(0), _hundredths(0), _lastTick(0)
{
}

void DemoClock::begin() {
    _disp.clearAll();
    _disp.setBacklight(true);
    _hours = 0;
    _minutes = 0;
    _seconds = 0;
    _hundredths = 0;
    _lastTick = millis();
}

void DemoClock::run() {
    unsigned long now = millis();
    // Обновление каждые 10мс (сотые доли)
    if (now - _lastTick >= 10) {
        _lastTick = now;

        _hundredths++;
        if (_hundredths >= 100) {
            _hundredths = 0;
            _seconds++;
            // Обновлять секундную стрелку
            _disp.setSecond(_seconds == 0 ? 60 : _seconds);

            if (_seconds >= 60) {
                _seconds = 0;
                _minutes++;
                if (_minutes >= 60) {
                    _minutes = 0;
                    _hours++;
                    if (_hours >= 24) {
                        _hours = 0;
                    }
                }
            }
        }

        // Левый блок: HH.MM
        _disp.setDigit(0, 0, _hours / 10);
        _disp.setDigit(0, 1, _hours % 10);
        _disp.setDot(0, true);
        _disp.setDigit(0, 2, _minutes / 10);
        _disp.setDigit(0, 3, _minutes % 10);

        // Правый блок: SS
        _disp.setDigit(1, 0, _seconds / 10);
        _disp.setDigit(1, 1, _seconds % 10);
        _disp.setDot(1, true);
        _disp.setDigit(1, 2, _hundredths / 10);
        _disp.setDigit(1, 3, _hundredths % 10);

        _disp.refresh();
    }
}

} // namespace demo
