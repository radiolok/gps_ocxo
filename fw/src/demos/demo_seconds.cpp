#include "demo_seconds.h"

namespace demo {

DemoSeconds::DemoSeconds(ILC1_8Display& display)
    : _disp(display), _current(1), _lastUpdate(0)
{
}

void DemoSeconds::begin() {
    _disp.clearAll();
    _disp.setBacklight(true);
    _current = 1;
    _lastUpdate = millis();
}

void DemoSeconds::run() {
    unsigned long now = millis();
    if (now - _lastUpdate >= 16) {   // ~60 fps
        _lastUpdate = now;

        _disp.setSecond(_current);

        _current++;
        if (_current > 60) {
            _current = 1;
        }

        _disp.refresh();
    }
}

} // namespace demo
