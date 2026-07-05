#pragma once

#include <Arduino.h>

/*
 * ШИМ яркости на пине OC2 (PD3 / D3).
 * Используется Timer2 (8-битный) на пине D3 (OC2B).
 *
 * Принцип:
 *   OC1 = L (постоянно)
 *   OC2 = ШИМ:
 *     HIGH (фаза DATA)   – сегменты светятся согласно фреймбуферу
 *     LOW  (фаза Hi-Z)   – все сегменты погашены
 *
 * Скважность задаёт яркость. При питании 35В+ ограничить максимум
 * (например 70-80%) для защиты сегментов.
 */

namespace hal {
namespace pwm {

// Частота ШИМ на D3 (Timer2, 8-bit):
//   prescaler=1:    16MHz/256 = 62.5 kHz
//   prescaler=8:    16MHz/2048 = ~7.8 kHz
//   prescaler=64:   16MHz/16384 = ~977 Hz
// Выбран prescaler=1 (62.5kHz) — выше порога слышимости
constexpr uint8_t PWM_PIN = 3;  // OC2B на PD3

// Лимит максимальной яркости. При питании 35В+ ограничен 180 (~70%)
// для защиты сегментов от перегорания. 255 = без ограничения.
static uint8_t _maxBrightness = 180;

void setup() {
    pinMode(PWM_PIN, OUTPUT);

    // Timer2: Fast PWM, неинвертированный режим на OC2B
    // WGM2[2:0] = 011 (Fast PWM, TOP=0xFF)
    // COM2B[1:0] = 10 (Clear OC2B on compare match, set at BOTTOM)
    TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(CS20);  // prescaler = 1

    OCR2B = 180;  // ~70% по умолчанию (защита сегментов при VDDH ≥ 35В)
}

// value: 0 (выкл) .. 255 (полная яркость).
// Автоматически ограничивается значением _maxBrightness.
inline void setBrightness(uint8_t value) {
    if (value > _maxBrightness) {
        value = _maxBrightness;
    }
    OCR2B = value;
}

// Получить текущее значение яркости (0..255)
inline uint8_t getBrightness() {
    return OCR2B;
}

// Установить верхний предел яркости (защита сегментов при высоком VDDH)
inline void setMaxBrightness(uint8_t maxValue) {
    _maxBrightness = maxValue;
    // Если текущая яркость выше нового лимита — обрезать
    if (OCR2B > _maxBrightness) {
        OCR2B = _maxBrightness;
    }
}

// Получить текущий лимит яркости
inline uint8_t getMaxBrightness() {
    return _maxBrightness;
}

} // namespace pwm
} // namespace hal
