#include "sn755870.h"
#include <Arduino.h>
#include <util/delay.h>
#include "../hal/pwm.h"

void SN755870Chain::begin() {
    hal::pins::setup();
    hal::pwm::setup();

    clearRegisters();   // сбросить обе микросхемы в исходное состояние
    clearBuffer();
    sendFrame();        // вытолкнуть пустой кадр
}

void SN755870Chain::clearRegisters() {
    // CLR: H -> все выходы принудительно в H (SN755870 асинхронный сброс выходов).
    // После снятия CLR выходы возвращаются к защёлкнутому состоянию,
    // поэтому сразу защёлкиваем нули во избежание случайного свечения.
    digitalWrite(hal::pins::CLR, HIGH);
    _delay_us(1);
    digitalWrite(hal::pins::CLR, LOW);
    _delay_us(1);

    // Отправить пустой буфер и защёлкнуть, чтобы гарантировать OFF на выходах
    clearBuffer();
    spiWriteFrame();
    latch();
}

void SN755870Chain::latch() {
    // LAT: H->L->H
    digitalWrite(hal::pins::LAT, LOW);
    _delay_us(1);
    digitalWrite(hal::pins::LAT, HIGH);
}

void SN755870Chain::spiWriteFrame() {
    // Байт 15 первым (бит 127 на MOSI), байт 0 последним (бит 0).
    // MSB-first внутри каждого байта.
    // Инверсия: SN755870 имеет обратную логику (0 = сегмент горит).
    for (int8_t i = BYTES - 1; i >= 0; i--) {
        SPI.transfer(buffer[i] ^ 0xFF);
    }
}

void SN755870Chain::sendFrame() {
    spiWriteFrame();
    latch();
}

void SN755870Chain::setBrightness(uint8_t value) {
    hal::pwm::setBrightness(value);
}

void SN755870Chain::setBit(uint8_t bitIndex, bool on) {
    if (bitIndex >= BITS) return;
    uint8_t byteIdx = bitIndex >> 3;           // / 8
    uint8_t bitPos  = bitIndex & 0x07;         // бит 0..7 внутри байта (0=LSB)
    if (on) {
        buffer[byteIdx] |= (1 << bitPos);
    } else {
        buffer[byteIdx] &= ~(1 << bitPos);
    }
}

bool SN755870Chain::getBit(uint8_t bitIndex) const {
    if (bitIndex >= BITS) return false;
    uint8_t byteIdx = bitIndex >> 3;
    uint8_t bitPos  = bitIndex & 0x07;
    return (buffer[byteIdx] >> bitPos) & 0x01;
}

void SN755870Chain::clearBuffer() {
    for (uint8_t i = 0; i < BYTES; i++) {
        buffer[i] = 0x00;
    }
}
