#pragma once

#include <stdint.h>

/*
 * ============================================================================
 * Карта соответствия: логический сегмент -> физический бит SN755870.
 * ============================================================================
 *
 * Сгенерировано из disp.net (KiCad netlist панели).
 *
 * Daisy-chain SPI: MCU(MOSI) -> DD2.SI -> DD2.SO -> DD1.SI
 *
 * Порядок SPI: бит 127 первым, бит 0 последним.
 *
 * Индексация битов 0..127:
 *   биты 0..63   -> DD2 (ближняя к МК микросхема)
 *   биты 64..127 -> DD1 (дальняя от МК)
 *
 * Формула пересчёта OUT# -> бит фреймбуфера:
 *   DD2: bit = OUT_number - 1     (OUT1->0,  OUT64->63)
 *   DD1: bit = 63 + OUT_number   (OUT1->64, OUT60->123)
 *
 * Неиспользованных выходов: DD1.OUT61..OUT64 (4 шт, NC).
 */

namespace seg {

// ---- Секундное кольцо (60 сегментов) ----
// HL1: s0..s59; ПО: S1..S60 (S1=s0 на HL1)
constexpr uint8_t S1  = 32;   // DD2.OUT33
constexpr uint8_t S2  = 34;   // DD2.OUT35
constexpr uint8_t S3  = 36;   // DD2.OUT37
constexpr uint8_t S4  = 38;   // DD2.OUT39
constexpr uint8_t S5  = 40;   // DD2.OUT41
constexpr uint8_t S6  = 42;   // DD2.OUT43
constexpr uint8_t S7  = 44;   // DD2.OUT45
constexpr uint8_t S8  = 46;   // DD2.OUT47
constexpr uint8_t S9  = 48;   // DD2.OUT49
constexpr uint8_t S10 = 50;   // DD2.OUT51
constexpr uint8_t S11 = 52;   // DD2.OUT53
constexpr uint8_t S12 = 54;   // DD2.OUT55
constexpr uint8_t S13 = 56;   // DD2.OUT57
constexpr uint8_t S14 = 58;   // DD2.OUT59
constexpr uint8_t S15 = 60;   // DD2.OUT61
constexpr uint8_t S16 = 62;   // DD2.OUT63
constexpr uint8_t S17 = 63;   // DD2.OUT64
constexpr uint8_t S18 = 64;   // DD1.OUT1
constexpr uint8_t S19 = 65;   // DD1.OUT2
constexpr uint8_t S20 = 66;   // DD1.OUT3
constexpr uint8_t S21 = 67;   // DD1.OUT4
constexpr uint8_t S22 = 68;   // DD1.OUT5
constexpr uint8_t S23 = 69;   // DD1.OUT6
constexpr uint8_t S24 = 71;   // DD1.OUT8
constexpr uint8_t S25 = 72;   // DD1.OUT9
constexpr uint8_t S26 = 73;   // DD1.OUT10
constexpr uint8_t S27 = 74;   // DD1.OUT11
constexpr uint8_t S28 = 75;   // DD1.OUT12
constexpr uint8_t S29 = 76;   // DD1.OUT13
constexpr uint8_t S30 = 77;   // DD1.OUT14
constexpr uint8_t S31 = 108;   // DD1.OUT45
constexpr uint8_t S32 = 109;   // DD1.OUT46
constexpr uint8_t S33 = 110;   // DD1.OUT47
constexpr uint8_t S34 = 111;   // DD1.OUT48
constexpr uint8_t S35 = 112;   // DD1.OUT49
constexpr uint8_t S36 = 113;   // DD1.OUT50
constexpr uint8_t S37 = 114;   // DD1.OUT51
constexpr uint8_t S38 = 116;   // DD1.OUT53
constexpr uint8_t S39 = 117;   // DD1.OUT54
constexpr uint8_t S40 = 118;   // DD1.OUT55
constexpr uint8_t S41 = 119;    // DD1.OUT56
constexpr uint8_t S42 = 120;    // DD1.OUT57
constexpr uint8_t S43 = 121;    // DD1.OUT58
constexpr uint8_t S44 = 122;    // DD1.OUT59
constexpr uint8_t S45 = 0;    // DD2.OUT51
constexpr uint8_t S46 = 2;    // DD2.OUT3
constexpr uint8_t S47 = 4;  // DD2.OUT5
constexpr uint8_t S48 = 6;  // DD2.OUT7
constexpr uint8_t S49 = 8;  // DD2.OUT9
constexpr uint8_t S50 = 10;  // DD2.OUT11
constexpr uint8_t S51 = 12;  // DD2.OUT13
constexpr uint8_t S52 = 14;  // DD2.OUT15
constexpr uint8_t S53 = 16;  // DD2.OUT17
constexpr uint8_t S54 = 18;  // DD2.OUT19
constexpr uint8_t S55 = 20;  // DD2.OUT21
constexpr uint8_t S56 = 22;  // DD2.OUT23
constexpr uint8_t S57 = 24;  // DD2.OUT25
constexpr uint8_t S58 = 26;  // DD2.OUT27
constexpr uint8_t S59 = 28;  // DD2.OUT29
constexpr uint8_t S60 = 30;  // DD2.OUT31

constexpr uint8_t S_MIN   = S1;
constexpr uint8_t S_MAX   = S60;
constexpr uint8_t S_COUNT = 60;

// Лукап-таблица для доступа ко всем секундным сегментам по индексу 0..59
constexpr uint8_t S_ALL[S_COUNT] = {
    S1,  S2,  S3,  S4,  S5,  S6,  S7,  S8,  S9,  S10,
    S11, S12, S13, S14, S15, S16, S17, S18, S19, S20,
    S21, S22, S23, S24, S25, S26, S27, S28, S29, S30,
    S31, S32, S33, S34, S35, S36, S37, S38, S39, S40,
    S41, S42, S43, S44, S45, S46, S47, S48, S49, S50,
    S51, S52, S53, S54, S55, S56, S57, S58, S59, S60
};

// Бит фреймбуфера для секундного сегмента n (1..60)
inline uint8_t secondBit(uint8_t n) {
    if (n < 1 || n > 60) return 0;
    return S_ALL[n - 1];
}

// ---- 7-сегментные блоки ----
// Блок 0 = левый (HL1 цифры 1-4), блок 1 = правый (HL1 цифры 5-8)
// pos 0..3 = разряд (слева направо)
// seg 0..6 = сегмент a..g

// Порядок сегментов в таблицах: a, b, c, d, e, f, g (seg 0..6)

// Блок 0 (левый), разряд 0 (HL1 цифра 1): 1a-1g
constexpr uint8_t B0D0_SEG[] = {5, 7, 9, 11, 123, 3, 1};

// Блок 0 (левый), разряд 1 (HL1 цифра 2): 2a-2g
constexpr uint8_t B0D1_SEG[] = {21, 23, 25, 27, 15, 19, 17};

// Блок 0 (левый), разряд 2 (HL1 цифра 3): 3a-3g
constexpr uint8_t B0D2_SEG[] = {37, 39, 41, 43, 31, 35, 33};

// Блок 0 (левый), разряд 3 (HL1 цифра 4): 4a-4g
constexpr uint8_t B0D3_SEG[] = {53, 55, 57, 59, 47, 51, 49};

// Блок 1 (правый), разряд 0 (HL1 цифра 5): 5a-5g
constexpr uint8_t B1D0_SEG[] = {107, 101, 103, 104, 105, 106, 102};

// Блок 1 (правый), разряд 1 (HL1 цифра 6): 6a-6g
constexpr uint8_t B1D1_SEG[] = {100, 94, 96, 97, 98, 99, 95};

// Блок 1 (правый), разряд 2 (HL1 цифра 7): 7a-7g
constexpr uint8_t B1D2_SEG[] = {92, 86, 88, 89, 90, 91, 87};

// Блок 1 (правый), разряд 3 (HL1 цифра 8): 8a-8g
constexpr uint8_t B1D3_SEG[] = {85, 79, 81, 82, 83, 84, 80};

inline uint8_t digitSegment(uint8_t block, uint8_t pos, uint8_t seg) {
    if (pos > 3 || seg > 6) return 0;
    if (block == 0) {
        const uint8_t* tbl = nullptr;
        switch (pos) {
            case 0: tbl = B0D0_SEG; break;
            case 1: tbl = B0D1_SEG; break;
            case 2: tbl = B0D2_SEG; break;
            case 3: tbl = B0D3_SEG; break;
        }
        return tbl[seg];
    }
    if (block == 1) {
        const uint8_t* tbl = nullptr;
        switch (pos) {
            case 0: tbl = B1D0_SEG; break;
            case 1: tbl = B1D1_SEG; break;
            case 2: tbl = B1D2_SEG; break;
            case 3: tbl = B1D3_SEG; break;
        }
        return tbl[seg];
    }
    return 0;
}

// ---- Центральные точки ----
constexpr uint8_t DOT1 = 93;   // dot  (блок 0/левый)  -> DD1.OUT30
constexpr uint8_t DOT2 = 61;   // dot2 (блок 1/правый) -> DD2.OUT62

// ---- Текстовые плашки ----
constexpr uint8_t LABEL_DATA  = 13;  // "ДАТА" -> DD2.OUT14
constexpr uint8_t LABEL_TV    = 29;   // "ТВ"   -> DD2.OUT30
constexpr uint8_t LABEL_PV    = 115;   // "ПВ"   -> DD1.OUT52
constexpr uint8_t LABEL_VVOD  = 45;   // "ВВОД" -> DD2.OUT46
constexpr uint8_t LABEL_SEC   = 70;   // "СЕК"  -> DD1.OUT7

// ---- Подсветка циферблата ----
constexpr uint8_t DISK = 78;   // "диск" -> DD1.OUT15

} // namespace seg
