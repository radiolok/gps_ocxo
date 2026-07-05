#pragma once

#include <stdint.h>
#include "../drivers/sn755870.h"
#include "segment_map.h"

/*
 * Драйвер индикатора ИЛЦ1-8/7ЛВ.
 *
 * Инкапсулирует SN755870Chain, предоставляя высокоуровневый API
 * для управления сегментами индикатора:
 *   - секундные сегменты (s1..s60)
 *   - два 4-разрядных 7-сегментных блока с точками
 *   - текстовые плашки (дата, тв, пв, ввод, сек)
 *   - диск подсветки
 *
 * Все изменения буферизуются в фреймбуфере.
 * refresh() отправляет буфер в SN755870 и защёлкивает данные.
 */

class ILC1_8Display {
public:
    ILC1_8Display();

    void begin();
    void refresh();          // отправить фреймбуфер в железо

    // ---- Секундное кольцо ----
    void setSecond(uint8_t n);       // зажечь сегмент s<n> (1..60), остальные погасить
    void setSecondRaw(uint8_t n, bool on); // прямой доступ к s<n>
    void clearSeconds();             // погасить все секундные сегменты

    // ---- 7-сегментные блоки ----
    // block: 0 = левый, 1 = правый
    // pos:   0..3 (разряд, 0 = самый левый)
    // value: 0..9
    void setDigit(uint8_t block, uint8_t pos, uint8_t value);
    void setDigitRaw(uint8_t block, uint8_t pos, uint8_t mask);  // маска a..g
    void setDot(uint8_t block, bool on);  // центральная точка блока
    void clearBlock(uint8_t block);
    void clearAllDigits();

    // ---- Текстовые плашки ----
    void setLabelData(bool on);
    void setLabelTV(bool on);
    void setLabelPV(bool on);
    void setLabelVvod(bool on);
    void setLabelSec(bool on);
    void clearAllLabels();

    // ---- Подсветка ----
    void setBacklight(bool on);

    // ---- Управление ----
    void setBrightness(uint8_t value);  // 0..255
    void filamentOn();
    void filamentOff();
    void clearAll();                    // погасить всё

    // Низкоуровневый доступ к фреймбуферу (для демо обнаружения)
    SN755870Chain& driver() { return _chain; }

private:
    SN755870Chain _chain;

    void setDigitSegment(uint8_t block, uint8_t pos, uint8_t seg, bool on);
};
