#include "demo_scan.h"

namespace demo {

DemoScan::DemoScan(ILC1_8Display& display)
    : _disp(display), _currentBit(0), _lastUpdate(0)
{
}

void DemoScan::begin() {
    _disp.filamentOn();
    _disp.clearAll();

    _currentBit = 0;
    _lastUpdate = millis();

    _disp.driver().setBit(0, true);
    _disp.refresh();
}

void DemoScan::run() {
    unsigned long now = millis();

    if (now - _lastUpdate >= 10) {
        _lastUpdate = now;

        _disp.driver().setBit(_currentBit, false);

        _currentBit++;
        if (_currentBit >= 128) {
            _currentBit = 0;
        }

        _disp.driver().setBit(_currentBit, true);
        _disp.refresh();
    }
}

} // namespace demo
