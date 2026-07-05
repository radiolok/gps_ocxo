#include "demo_blink.h"

namespace demo {

DemoBlink::DemoBlink(ILC1_8Display& display)
    : _disp(display), _state(false), _lastToggle(0)
{
}

void DemoBlink::begin() {
    _state = false;
    _lastToggle = millis();

    _disp.filamentOn();
    _disp.setBacklight(false);

    // Погасить всё
    for (uint8_t i = 0; i < 128; i++) {
        _disp.driver().setBit(i, false);
    }
    _disp.refresh();
}

void DemoBlink::run() {
    unsigned long now = millis();

    if (now - _lastToggle >= 100) {
        _lastToggle = now;
        _state = !_state;

        // Запись напрямую в буфер: все 128 бит = 1 или 0
        for (uint8_t i = 0; i < 128; i++) {
            _disp.driver().setBit(i, _state);
        }
        _disp.refresh();
    }
}

} // namespace demo
