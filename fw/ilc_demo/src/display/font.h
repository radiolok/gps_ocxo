#pragma once

#include <stdint.h>

/*
 * Шрифт для 7-сегментных индикаторов.
 *
 * Раскладка сегментов:
 *      a
 *    ┌───┐
 *  f │   │ b
 *    ├─g─┤
 *  e │   │ c
 *    └───┘
 *      d
 *
 * Битовая маска: бит 0=a, 1=b, 2=c, 3=d, 4=e, 5=f, 6=g
 * Полярность: 1 = сегмент горит (OUT = H на SN755870).
 */

namespace font {

// Цифры 0-9
constexpr uint8_t DIGIT_0 = 0b0111111;  // a,b,c,d,e,f
constexpr uint8_t DIGIT_1 = 0b0000110;  // b,c
constexpr uint8_t DIGIT_2 = 0b1011011;  // a,b,d,e,g
constexpr uint8_t DIGIT_3 = 0b1001111;  // a,b,c,d,g
constexpr uint8_t DIGIT_4 = 0b1100110;  // b,c,f,g
constexpr uint8_t DIGIT_5 = 0b1101101;  // a,c,d,f,g
constexpr uint8_t DIGIT_6 = 0b1111101;  // a,c,d,e,f,g
constexpr uint8_t DIGIT_7 = 0b0000111;  // a,b,c
constexpr uint8_t DIGIT_8 = 0b1111111;  // все
constexpr uint8_t DIGIT_9 = 0b1101111;  // a,b,c,d,f,g

constexpr uint8_t DIGIT_MINUS = 0b1000000;  // g

// Буквы (ограниченный набор для 7-сегментного индикатора)
constexpr uint8_t LETTER_A = 0b1110111;  // a,b,c,e,f,g
constexpr uint8_t LETTER_C = 0b0111001;  // a,d,e,f
constexpr uint8_t LETTER_E = 0b1111001;  // a,d,e,f,g
constexpr uint8_t LETTER_F = 0b1110001;  // a,e,f,g
constexpr uint8_t LETTER_H = 0b1110110;  // b,c,e,f,g
constexpr uint8_t LETTER_L = 0b0111000;  // d,e,f
constexpr uint8_t LETTER_P = 0b1110011;  // a,b,e,f,g
constexpr uint8_t LETTER_U = 0b0111110;  // b,c,d,e,f

constexpr uint8_t BLANK   = 0b0000000;

// Получить маску для цифры
inline uint8_t digit(uint8_t n) {
    if (n > 9) return BLANK;
    constexpr uint8_t digits[] = {
        DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4,
        DIGIT_5, DIGIT_6, DIGIT_7, DIGIT_8, DIGIT_9
    };
    return digits[n];
}

} // namespace font
