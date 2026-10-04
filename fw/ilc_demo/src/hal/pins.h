#pragma once

#include <Arduino.h>
#include <SPI.h>

/*
 * Назначение пинов Arduino Nano для управления индикатором ИЛЦ1-8/7ЛВ
 * через две микросхемы SN755870 в цепочке (SPI daisy-chain).
 *
 * Цепочка: MCU(MOSI) -> DD2.SI -> DD2.SO -> DD1.SI
 * DD2 ближе к МК, DD1 дальше. DIR=H (SI->SO) на обеих.
 *
 * За 128 тактов SPI:
 *   биты 0..63   уходят в DD1 (дальняя)
 *   биты 64..127  остаются в DD2 (ближняя)
 */

namespace hal {
namespace pins {

// --- SPI (аппаратный) ---
// MOSI: PB3 (D11)  – данные в DD2.SI
// SCK:  PB5 (D13)  – тактовый сигнал CLK
// MISO: PB4 (D12)  – не используется (SO цепочки не читаем)
// SS:   PB2 (D10)  – обязательно OUTPUT, иначе SPI уходит в slave mode

// --- Управляющие сигналы SN755870 (общие для обеих микросхем) ---
constexpr uint8_t CLR = PD2;  // D2  – очистка сдвигового регистра (H = сброс)
constexpr uint8_t OC2 = PD3;  // D3  – управление выходом (Timer2 PWM для яркости)
constexpr uint8_t OC1 = PD4;  // D4  – управление выходом
constexpr uint8_t LAT = PD5;  // D5  – защёлка данных в выходной регистр

// --- Питание накала ---
constexpr uint8_t EN  = PD6;  // D6  – включение преобразователя накала (опционально)

inline void setup() {
    // SPI: SS должен быть OUTPUT, иначе аппаратный SPI переходит в slave
    pinMode(SS, OUTPUT);
    digitalWrite(SS, HIGH);

    // Управляющие пины
    pinMode(CLR, OUTPUT);
    digitalWrite(CLR, LOW);    // нормальный режим (L)

    pinMode(OC1, OUTPUT);
    digitalWrite(OC1, LOW);    // постоянный LOW для ШИМ яркости через OC2

    pinMode(OC2, OUTPUT);
    digitalWrite(OC2, LOW);    // по умолчанию выходы Hi-Z (выкл)

    pinMode(LAT, OUTPUT);
    digitalWrite(LAT, HIGH);   // H = хранение данных в выходном регистре

    pinMode(EN, OUTPUT);
    digitalWrite(EN, LOW);     // накал выключен по умолчанию

    // Аппаратный SPI: mode 0 (CPOL=0, CPHA=0), данные на MOSI до SCK↑
    // SN755870 CLK: "Down-side edge trigger" — семплирует по SCK↓
    // MODE0 даёт установку данных до первого фронта, а не по фронту
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV16);  // 16MHz/16 = 1MHz
}

} // namespace pins
} // namespace hal
