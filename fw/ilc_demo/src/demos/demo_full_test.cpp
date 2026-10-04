#include "demo_full_test.h"
#include "../display/segment_map.h"

namespace demo {

DemoFullTest::DemoFullTest(ILC1_8Display& display)
    : _disp(display)
    , _secCurrent(1)
    , _lastSec(0)
    , _digCurrent(0)
    , _lastDig(0)
    , _blinkState(false)
    , _lastBlink(0)
{
}

void DemoFullTest::begin() {
    _disp.clearAll();
    _disp.filamentOn();

    unsigned long now = millis();
    _lastSec  = now;
    _lastDig  = now;
    _lastBlink = now;

    // Зажечь первый секундный сегмент
    _secCurrent = 1;
    _disp.driver().setBit(seg::secondBit(_secCurrent), true);

    // Все 8 разрядов — сегмент 'a' (0)
    _digCurrent = 0;
    for (uint8_t b = 0; b < 2; b++) {
        for (uint8_t p = 0; p < 4; p++) {
            _disp.driver().setBit(seg::digitSegment(b, p, _digCurrent), true);
        }
    }

    // Плашки + диск включены
    _blinkState = true;
    _disp.setBacklight(true);
    _disp.setLabelData(true);
    _disp.setLabelTV(true);
    _disp.setLabelPV(true);
    _disp.setLabelVvod(true);
    _disp.setLabelSec(true);

    _disp.refresh();
}

void DemoFullTest::run() {
    unsigned long now = millis();
    bool dirty = false;

    // 1. Секундное кольцо: перебор каждые 10мс
    if (now - _lastSec >= 10) {
        _lastSec = now;

        _disp.driver().setBit(seg::secondBit(_secCurrent), false);

        _secCurrent++;
        if (_secCurrent > 60) {
            _secCurrent = 1;
        }

        _disp.driver().setBit(seg::secondBit(_secCurrent), true);
        dirty = true;
    }

    // 2. 7-сегментные блоки: перебор a..g каждые 100мс
    if (now - _lastDig >= 100) {
        _lastDig = now;

        // Погасить текущий сегмент на всех разрядах
        for (uint8_t b = 0; b < 2; b++) {
            for (uint8_t p = 0; p < 4; p++) {
                _disp.driver().setBit(seg::digitSegment(b, p, _digCurrent), false);
            }
        }

        _digCurrent++;
        if (_digCurrent > 6) {
            _digCurrent = 0;
        }

        // Зажечь новый сегмент на всех разрядах
        for (uint8_t b = 0; b < 2; b++) {
            for (uint8_t p = 0; p < 4; p++) {
                _disp.driver().setBit(seg::digitSegment(b, p, _digCurrent), true);
            }
        }
        dirty = true;
    }

    // 3. Диск + плашки: моргание каждые 50мс
    if (now - _lastBlink >= 50) {
        _lastBlink = now;

        _blinkState = !_blinkState;
        _disp.setBacklight(_blinkState);
        _disp.setLabelData(_blinkState);
        _disp.setLabelTV(_blinkState);
        _disp.setLabelPV(_blinkState);
        _disp.setLabelVvod(_blinkState);
        _disp.setLabelSec(_blinkState);
        dirty = true;
    }

    if (dirty) {
        _disp.refresh();
    }
}

} // namespace demo
