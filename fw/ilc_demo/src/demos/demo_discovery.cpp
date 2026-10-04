#include "demo_discovery.h"

namespace demo {

DemoDiscovery::DemoDiscovery(ILC1_8Display& display)
    : _disp(display), _currentBit(0), _lastUpdate(0), _reported(false)
{
}

void DemoDiscovery::begin() {
    Serial.begin(9600);
    Serial.println(F("=== ИЛЦ1-8/7ЛВ: обнаружение сегментов ==="));
    Serial.println(F("Наблюдайте, какой сегмент загорается при каждом номере бита."));
    Serial.println(F("Биты 0..63   = DD1 (дальняя от МК)"));
    Serial.println(F("Биты 64..127 = DD2 (ближняя к МК)"));
    Serial.println(F("---"));

    _disp.clearAll();
    _disp.filamentOn();
    _disp.setBacklight(true);
    _currentBit = 0;
    _reported = true;
    _lastUpdate = millis();

    // Зажечь нулевой бит
    _disp.driver().setBit(0, true);
    _disp.refresh();

    Serial.print(F("Bit "));
    Serial.println(_currentBit);
}

void DemoDiscovery::run() {
    if (_currentBit < 0) return;  // done

    unsigned long now = millis();

    if (!_reported && now - _lastUpdate > 500) {
        // Выводим номер бита через 500ms после переключения
        Serial.print(F("Bit "));
        Serial.println(_currentBit);
        _reported = true;
    }

    if (now - _lastUpdate >= 1000) {   // 1.5 секунды на бит
        _lastUpdate = now;

        // Погасить текущий бит
        _disp.driver().setBit(_currentBit, false);

        _currentBit++;
        if (_currentBit >= 128) {
            Serial.println(F("--- Все 128 битов проверены. Готово. ---"));
            _disp.clearAll();
            _disp.refresh();
            _currentBit = -1;
            return;
        }

        // Зажечь следующий
        _disp.driver().setBit(_currentBit, true);
        _disp.refresh();

        _reported = false;
    }
}

} // namespace demo
