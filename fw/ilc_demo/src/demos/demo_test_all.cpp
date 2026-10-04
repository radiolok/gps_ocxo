#include "demo_test_all.h"

namespace demo {

DemoTestAll::DemoTestAll(ILC1_8Display& display)
    : _disp(display)
    , _sec(1), _lastSec(0)
    , _digit(0), _lastDigit(0)
    , _labelIdx(0), _lastLabel(0)
    , _diskState(false), _lastDisk(0)
{
}

void DemoTestAll::begin() {
    _disp.clearAll();
    _disp.filamentOn();
    _disp.setBacklight(false);

    unsigned long now = millis();
    _lastSec   = now;
    _lastDigit = now;
    _lastLabel = now;
    _lastDisk  = now;

    // Начальное состояние
    _sec = 1;
    _disp.setSecond(_sec);
    _digit = 0;
    _labelIdx = 0;
    _disp.setLabelData(true);
    _diskState = false;

    _disp.refresh();
}

void DemoTestAll::run() {
    unsigned long now = millis();
    bool dirty = false;

    // 1. Секундное кольцо: бегущий сегмент каждые 20мс
    if (now - _lastSec >= 20) {
        _lastSec = now;
        _sec++;
        if (_sec > 60) _sec = 1;
        _disp.setSecond(_sec);
        dirty = true;
    }

    // 2. 7-сегментные блоки: 0..9 каждые 100мс
    if (now - _lastDigit >= 100) {
        _lastDigit = now;
        for (uint8_t b = 0; b < 2; b++) {
            for (uint8_t p = 0; p < 4; p++) {
                _disp.setDigit(b, p, _digit);
            }
            _disp.setDot(b, _diskState);
        }
        _digit++;
        if (_digit > 9) _digit = 0;
        dirty = true;
    }

    // 3. Плашки: поочерёдно каждые 500мс
    if (now - _lastLabel >= 500) {
        _lastLabel = now;
        _disp.clearAllLabels();
        switch (_labelIdx) {
            case 0: _disp.setLabelData(true); break;
            case 1: _disp.setLabelTV(true);   break;
            case 2: _disp.setLabelPV(true);   break;
            case 3: _disp.setLabelVvod(true); break;
            case 4: _disp.setLabelSec(true);  break;
        }
        _labelIdx++;
        if (_labelIdx >= 5) _labelIdx = 0;
        dirty = true;
    }

    // 4. Диск + точки: мигание каждые 300мс
    if (now - _lastDisk >= 300) {
        _lastDisk = now;
        _diskState = !_diskState;
        _disp.setBacklight(_diskState);
        _disp.setDot(0, _diskState);
        _disp.setDot(1, _diskState);
        dirty = true;
    }

    if (dirty) {
        _disp.refresh();
    }
}

} // namespace demo
