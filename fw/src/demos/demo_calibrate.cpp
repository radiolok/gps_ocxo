#include "demo_calibrate.h"
#include "../display/segment_map.h"

namespace demo {

DemoCalibrate::DemoCalibrate(ILC1_8Display& display)
    : _disp(display), _current(1), _lastUpdate(0), _firstStep(true)
{
}

void DemoCalibrate::begin() {
    Serial.begin(9600);
    Serial.println(F("=== Калибровка секундного кольца ==="));
    Serial.println(F("Формат: S=<номер> bit=<бит фреймбуфера>"));
    Serial.println(F("Записывайте: на какой позиции циферблата горит сегмент."));
    Serial.println(F("---"));

    _disp.clearAll();
    _disp.filamentOn();

    _current = 1;
    _firstStep = true;
    _lastUpdate = millis();
}

void DemoCalibrate::run() {
    unsigned long now = millis();

    if (now - _lastUpdate >= 500) {
        _lastUpdate = now;

        _disp.driver().setBit(seg::secondBit(_current), false);

        _current++;
        if (_current > 60) {
            _current = 1;
            Serial.println(F("--- цикл завершён ---"));
        }

        _disp.driver().setBit(seg::secondBit(_current), true);
        _disp.refresh();

        Serial.print(F("S="));
        Serial.print(_current);
        Serial.print(F("\tbit="));
        Serial.println(seg::secondBit(_current));
    }
}

} // namespace demo
