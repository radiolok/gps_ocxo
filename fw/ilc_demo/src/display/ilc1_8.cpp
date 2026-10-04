#include "ilc1_8.h"
#include "font.h"

ILC1_8Display::ILC1_8Display() {
}

void ILC1_8Display::begin() {
    _chain.begin();
    clearAll();
    refresh();
}

void ILC1_8Display::refresh() {
    _chain.sendFrame();
}

// ---- Секундное кольцо ----

void ILC1_8Display::setSecond(uint8_t n) {
    clearSeconds();
    setSecondRaw(n, true);
}

void ILC1_8Display::setSecondRaw(uint8_t n, bool on) {
    if (n < 1 || n > 60) return;
    _chain.setBit(seg::secondBit(n), on);
}

void ILC1_8Display::clearSeconds() {
    for (uint8_t i = 0; i < seg::S_COUNT; i++) {
        _chain.setBit(seg::S_ALL[i], false);
    }
}

// ---- 7-сегментные блоки ----

void ILC1_8Display::setDigitSegment(uint8_t block, uint8_t pos, uint8_t seg, bool on) {
    uint8_t idx = seg::digitSegment(block, pos, seg);
    _chain.setBit(idx, on);
}

void ILC1_8Display::setDigit(uint8_t block, uint8_t pos, uint8_t value) {
    uint8_t mask = font::digit(value);
    setDigitRaw(block, pos, mask);
}

void ILC1_8Display::setDigitRaw(uint8_t block, uint8_t pos, uint8_t mask) {
    if (block > 1 || pos > 3) return;
    for (uint8_t seg = 0; seg < 7; seg++) {
        setDigitSegment(block, pos, seg, (mask >> seg) & 0x01);
    }
}

void ILC1_8Display::setDot(uint8_t block, bool on) {
    if (block == 0) {
        _chain.setBit(seg::DOT1, on);
    } else {
        _chain.setBit(seg::DOT2, on);
    }
}

void ILC1_8Display::clearBlock(uint8_t block) {
    for (uint8_t pos = 0; pos < 4; pos++) {
        setDigitRaw(block, pos, font::BLANK);
    }
    setDot(block, false);
}

void ILC1_8Display::clearAllDigits() {
    clearBlock(0);
    clearBlock(1);
}

// ---- Текстовые плашки ----

void ILC1_8Display::setLabelData(bool on) { _chain.setBit(seg::LABEL_DATA, on); }
void ILC1_8Display::setLabelTV(bool on)   { _chain.setBit(seg::LABEL_TV, on); }
void ILC1_8Display::setLabelPV(bool on)   { _chain.setBit(seg::LABEL_PV, on); }
void ILC1_8Display::setLabelVvod(bool on) { _chain.setBit(seg::LABEL_VVOD, on); }
void ILC1_8Display::setLabelSec(bool on)  { _chain.setBit(seg::LABEL_SEC, on); }

void ILC1_8Display::clearAllLabels() {
    setLabelData(false);
    setLabelTV(false);
    setLabelPV(false);
    setLabelVvod(false);
    setLabelSec(false);
}

// ---- Подсветка ----

void ILC1_8Display::setBacklight(bool on) {
    _chain.setBit(seg::DISK, on);
}

// ---- Управление ----

void ILC1_8Display::setBrightness(uint8_t value) {
    _chain.setBrightness(value);
}

void ILC1_8Display::filamentOn()  { _chain.filamentOn(); }
void ILC1_8Display::filamentOff() { _chain.filamentOff(); }

void ILC1_8Display::clearAll() {
    _chain.clearBuffer();
}
