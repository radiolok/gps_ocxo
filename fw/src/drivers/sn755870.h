#pragma once

#include <stdint.h>
#include "../hal/pins.h"

/*
 * Драйвер цепочки из двух микросхем SN755870.
 *
 * Микросхемы включены последовательно (daisy-chain):
 *   MCU(MOSI) -> DD2.SI -> DD2.SO -> DD1.SI
 * DIR=H (SI->SO) на обеих.
 *
 * Фреймбуфер: 128 бит (16 байт).
 * Биты 0..63   — DD2 (ближняя к МК).
 * Биты 64..127 — DD1 (дальняя от МК).
 *
 * Порядок выдачи по SPI: байт 15 первым (бит 127 первым на MOSI),
 * байт 0 последним (бит 0 последним). MSB-first внутри байта.
 *
 * Управление:
 *   CLR: H = сброс всех выходов в H
 *   OC1=L, OC2: ШИМ яркости (H=DATA, L=Hi-Z)
 *   LAT: H->L->H фиксирует сдвиговый регистр в выходной
 */

class SN755870Chain {
public:
    static constexpr uint8_t CHIPS   = 2;
    static constexpr uint8_t BITS    = 64 * CHIPS;   // 128
    static constexpr uint8_t BYTES   = BITS / 8;      // 16

    // Фреймбуфер. Бит=1 — сегмент горит (OUT = H).
    uint8_t buffer[BYTES];

    SN755870Chain() { clearBuffer(); }

    void begin();
    void clearRegisters();   // аппаратный сброс (импульс CLR)
    void latch();            // защёлкнуть данные (импульс LAT)
    void sendFrame();        // отправить buffer[] по SPI и защёлкнуть
    void setBrightness(uint8_t value); // 0-255

    // Включить/выключить накал
    void filamentOn()  { digitalWrite(hal::pins::EN, HIGH); }
    void filamentOff() { digitalWrite(hal::pins::EN, LOW); }

    // Прямой доступ к биту фреймбуфера
    void setBit(uint8_t bitIndex, bool on);
    bool getBit(uint8_t bitIndex) const;
    void clearBuffer();

private:
    void spiWriteFrame();
};
