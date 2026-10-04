#!/usr/bin/env python3
"""Генератор структурных схем DOROGO в SVG (светлый фон, читается и в тёмной теме GitHub)."""
import os
import sys
from xml.sax.saxutils import escape

OUT = sys.argv[1] if len(sys.argv) > 1 else "."

FONT = "Inter, 'Segoe UI', 'DejaVu Sans', Arial, sans-serif"
BG = "#ffffff"
INK = "#1f2328"
QUIET = "#57606a"
EDGE = "#8c959f"
FRAME = "#afb8c1"
ACC = "#1f6feb"
ACC_FILL = "#ddf4ff"
ALT = "#bf8700"
ALT_FILL = "#fff8c5"


class Svg:
    def __init__(self, w, h, title):
        self.w, self.h = w, h
        self.parts = []
        self.title = title

    def frame(self, x, y, w, h, label, anchor="start"):
        self.parts.append(
            f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="10" fill="none" '
            f'stroke="{FRAME}" stroke-width="1.2" stroke-dasharray="5 4"/>')
        tx = x + 16 if anchor == "start" else x + w - 16
        self.parts.append(
            f'<text x="{tx}" y="{y + 24}" text-anchor="{anchor}" font-size="13" '
            f'font-weight="600" fill="{QUIET}">{escape(label)}</text>')

    def box(self, cx, cy, w, h, name, lines=(), style="plain"):
        x, y = cx - w / 2, cy - h / 2
        if style == "main":
            fill, stroke, sw = ACC_FILL, ACC, 2
        elif style == "alt":
            fill, stroke, sw = ALT_FILL, ALT, 1.6
        else:
            fill, stroke, sw = BG, EDGE, 1.3
        self.parts.append(
            f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="8" fill="{fill}" '
            f'stroke="{stroke}" stroke-width="{sw}"/>')
        n = 1 + len(lines)
        total = 17 + 15 * (n - 1)
        ty = cy - total / 2 + 13
        self.parts.append(
            f'<text x="{cx}" y="{ty}" text-anchor="middle" font-size="14" '
            f'font-weight="600" fill="{INK}">{escape(name)}</text>')
        for i, ln in enumerate(lines):
            self.parts.append(
                f'<text x="{cx}" y="{ty + 17 + 15 * i}" text-anchor="middle" '
                f'font-size="11.5" fill="{QUIET}">{escape(ln)}</text>')

    def wire(self, pts, both=False, dashed=False, color=EDGE):
        d = "M" + " L".join(f"{x} {y}" for x, y in pts)
        extra = ' stroke-dasharray="4 3"' if dashed else ""
        start = ' marker-start="url(#arr)"' if both else ""
        self.parts.append(
            f'<path d="{d}" fill="none" stroke="{color}" stroke-width="1.4"{extra}'
            f'{start} marker-end="url(#arr)"/>')

    def line(self, pts, color=EDGE):
        d = "M" + " L".join(f"{x} {y}" for x, y in pts)
        self.parts.append(f'<path d="{d}" fill="none" stroke="{color}" stroke-width="1.4"/>')

    def dot(self, x, y):
        self.parts.append(f'<circle cx="{x}" cy="{y}" r="3.2" fill="{EDGE}"/>')

    def label(self, x, y, text, anchor="start", size=11.5, color=QUIET, weight="400"):
        self.parts.append(
            f'<text x="{x}" y="{y}" text-anchor="{anchor}" font-size="{size}" '
            f'font-weight="{weight}" fill="{color}">{escape(text)}</text>')

    def heading(self, text, sub=None):
        self.label(28, 38, text, size=17, color=INK, weight="700")
        if sub:
            self.label(28, 60, sub, size=12.5)

    def save(self, name):
        body = "\n  ".join(self.parts)
        svg = (
            f'<svg xmlns="http://www.w3.org/2000/svg" width="{self.w}" height="{self.h}" '
            f'viewBox="0 0 {self.w} {self.h}" font-family="{FONT}" role="img">\n'
            f'  <title>{escape(self.title)}</title>\n'
            f'  <defs><marker id="arr" viewBox="0 0 10 10" refX="9" refY="5" '
            f'markerWidth="7" markerHeight="7" orient="auto-start-reverse">'
            f'<path d="M0 0 L10 5 L0 10 z" fill="{EDGE}"/></marker></defs>\n'
            f'  <rect width="100%" height="100%" fill="{BG}"/>\n  {body}\n</svg>\n')
        with open(os.path.join(OUT, name), "w", encoding="utf-8") as f:
            f.write(svg)


# ---------------------------------------------------------------- 1. Система
def system():
    s = Svg(900, 640, "DOROGO: системная архитектура")
    s.heading("AD9545 дисциплинирует все выходы по GNSS PPS, OCXO бежит свободно",
              "Две платы, одна петля частоты; STM32 подводит только фазу PHC")
    c1, c2, c3 = 160, 450, 740
    W, H = 230, 62
    r1, r2, r3, r4 = 160, 262, 430, 534
    s.frame(28, 92, 844, 216, "Плата №1 — MCU + ETH + GNSS", "end")
    s.frame(28, 356, 844, 236, "Плата №2 — OCXO + AD9545 + выходы", "end")
    s.box(c1, r1, W, H, "ZED-F9T", ["PPS + qErr по UBX"])
    s.box(c2, r1, W, H, "TIM2, захват", ["GNSS PPS против метки PHC"])
    s.box(c3, r1, W, H, "phc_servo", ["только фаза PHC (addend)"])
    s.box(c1, r2, W, H, "Тактирование", ["HSE 25 МГц (PH0), LMK1C1102"])
    s.box(c2, r2, W, H, "PHC в MAC STM32", ["PTP GM, NTP, индикатор"])
    s.box(c3, r2, W, H, "ad9545_ctl", ["SPI3: профиль, статус, FTW"])
    s.box(c1, r3, W, H, "AD9545", ["DPLL по PPS, sysclk от OCXO"], "main")
    s.box(c2, r3, W, H, "OCXO 10 МГц", ["TCO-6920N, свободный ход"])
    s.box(c3, r3, W, H, "EFC", ["опора + подстроечник, вручную"], "alt")
    s.box(c1, r4, W, H, "Цифровые выходы", ["25 МГц, 2 × TTL, PPS"])
    s.box(c2, r4, W, H, "Синусные выходы", ["10 / 5 / 1 МГц, ФНЧ, 50 Ом"])
    # плата 1
    s.wire([(c1 + W / 2, r1), (c2 - W / 2, r1)])
    s.wire([(c2 + W / 2, r1), (c3 - W / 2, r1)])
    s.wire([(c3 - 60, r1 + H / 2), (c3 - 60, 212), (c2 + 60, 212), (c2 + 60, r2 - H / 2)])
    s.wire([(c2, r2 - H / 2), (c2, r1 + H / 2)])
    s.label(c2 + 8, 216, "ITR1")
    s.wire([(c1 + W / 2, r2), (c2 - W / 2, r2)])
    # PPS вниз на REF
    s.wire([(c1 - W / 2 + 18, r1 + H / 2), (c1 - W / 2 + 18, r3 - H / 2)])
    s.label(c1 - W / 2 + 26, 336, "PPS → REF")
    # SPI3
    s.wire([(c3, r2 + H / 2), (c3, 338), (c1 + 70, 338), (c1 + 70, r3 - H / 2)], both=True)
    s.label(c3 + 8, 330, "SPI3")
    # 25 МГц вверх
    s.wire([(c1 + 20, r3 - H / 2), (c1 + 20, r2 + H / 2)])
    s.label(c1 + 28, 318, "25 МГц")
    # OCXO -> AD9545, EFC -> OCXO
    s.wire([(c2 - W / 2, r3), (c1 + W / 2, r3)])
    s.label(c1 + W / 2 + 10, r3 - 8, "sysclk")
    s.wire([(c3 - W / 2, r3), (c2 + W / 2, r3)], dashed=True)
    # выходы
    s.wire([(c1, r3 + H / 2), (c1, r4 - H / 2)])
    s.wire([(c1 + 80, r3 + H / 2), (c1 + 80, 486), (c2, 486), (c2, r4 - H / 2)])
    s.save("system.svg")


# ---------------------------------------------------------------- 2. Две петли
def loops():
    s = Svg(900, 600, "DOROGO: кто что корректирует")
    s.heading("AD9545 держит частоту выходов, STM32 — время суток в PHC",
              "Петля частоты — аппаратно и непрерывно; петля времени — раз в секунду")
    s.frame(28, 88, 470, 488, "Петля частоты — AD9545")
    s.frame(526, 88, 346, 488, "Петля времени — STM32")
    s.box(140, 160, 180, 66, "ZED-F9T", ["TP1 → PPS", "UART → UBX"])
    s.box(140, 330, 180, 56, "OCXO 10 МГц", ["свободный ход"])
    s.box(370, 330, 220, 124, "AD9545",
          ["REF: GNSS PPS", "sysclk: OCXO", "DPLL (мГц) → NCO → APLL", "статус и FTW → SPI3"], "main")
    s.box(370, 500, 220, 66, "Выходы", ["синус 10/5/1, 2 × TTL, PPS", "25 МГц → HSE STM32 и PHY"])
    X = 700
    s.box(X, 160, 220, 56, "TIM2", ["PA15: PPS · ITR1: PHC"])
    s.box(X, 250, 220, 56, "gnss_ubx", ["номер секунды, UTC, qErr"])
    s.box(X, 340, 220, 56, "phc_servo", ["step один раз, затем addend"])
    s.box(X, 430, 220, 56, "PHC в MAC", ["PTP, NTP, индикатор"])
    s.box(X, 520, 220, 56, "ad9545_ctl", ["профиль, статус, FTW"])
    # PPS
    s.wire([(230, 148), (X - 110, 148)])
    s.dot(370, 148)
    s.wire([(370, 148), (370, 268)])
    s.label(380, 140, "GNSS PPS")
    # UBX
    s.wire([(230, 176), (300, 176), (300, 214), (560, 214), (560, 250), (X - 110, 250)])
    s.label(310, 206, "UBX, USART1")
    s.wire([(140 + 90, 330), (370 - 110, 330)])
    # internal STM32
    s.wire([(X, 278), (X, 312)])
    s.label(X + 8, 300, "qErr")
    s.wire([(X + 110, 160), (852, 160), (852, 340), (X + 110, 340)])
    s.wire([(X, 368), (X, 402)])
    s.wire([(X + 110, 430), (862, 430), (862, 148), (X + 110, 148)])
    s.label(866, 290, "ITR1", size=11)
    # 25 MHz to PHC
    s.wire([(480, 482), (560, 482), (560, 430), (X - 110, 430)])
    s.label(488, 474, "25 МГц")
    # SPI3
    s.wire([(X - 110, 520), (540, 520), (540, 370), (480, 370)], both=True)
    s.label(548, 540, "SPI3")
    s.wire([(370, 392), (370, 467)])
    s.save("loops.svg")


# ---------------------------------------------------------------- 3. Структура ПО
def software():
    s = Svg(940, 660, "DOROGO: структура ПО на Zephyr")
    s.heading("Все потоки времени сходятся в timekeeper",
              "Сверху вниз: сервисы · ядро времени · драйверы и ISR · периферия и выводы")
    cols = [110, 290, 470, 650, 830]
    rows = [130, 270, 410, 560]
    W, H = 160, 76
    lay = [
        [("log + settings", ["FTW, TE, температура", "NVS"]),
         ("ptp_gm", ["G.8275.1, L2", "clockClass"]),
         ("ntp_srv + http_srv", ["UDP 123, HTTP", "/api/status, /metrics"]),
         ("fw_update", ["MCUboot, слот 2", "по Ethernet"]),
         ("ui + shell + devcfg", ["индикатор, меню", "USB CDC, Telnet"])],
        [("gnss_ubx", ["UTC, leap, qErr", "TIM-TP, NAV-*"]),
         ("phc_servo", ["фаза PHC", "step, addend"]),
         ("timekeeper", ["TAI, UTC-offset", "единое время"]),
         ("ad9545_ctl", ["профиль, статус", "FTW раз в 1 с"]),
         None],
        [("USART1 + DMA", ["кольцевой буфер", "парсер UBX"]),
         ("pps_capture", ["ISR TIM2", "высший приоритет"]),
         ("ETH MAC + PHC", ["MII, метки 1588", "PTP trigger"]),
         ("SPI3 + DMA", ["таблица регистров", "ACE-профиль"]),
         ("USART2 + EXTI", ["кадр ИЛЦ по DMA", "кнопки, USB CDC"])],
        [("ZED-F9T", ["UART PB6/PB7", "U.FL ANT"]),
         ("GNSS PPS", ["PA15 TIM2_CH1", "ITR1: PHC"]),
         ("DP83640", ["MII, AF11", "RST PB2"]),
         ("AD9545", ["SPI3 PC10–PC12", "CS PA4, RST PC9"]),
         ("Панель + USB", ["PD4–PD7, PD0–PD3", "USB PA11/PA12"])],
    ]
    for r, row in enumerate(lay):
        for c, cell in enumerate(row):
            if cell:
                s.box(cols[c], rows[r], W, H, cell[0], cell[1],
                      "main" if cell[0] == "timekeeper" else "plain")
    h = H / 2
    # периферия -> драйверы
    for c in range(5):
        both = c >= 2
        s.wire([(cols[c], rows[3] - h), (cols[c], rows[2] + h)], both=both)
    # драйверы -> ядро
    s.wire([(cols[0], rows[2] - h), (cols[0], rows[1] + h)])
    s.wire([(cols[1], rows[2] - h), (cols[1], rows[1] + h)])
    s.wire([(cols[2], rows[2] - h), (cols[2], rows[1] + h)], both=True)
    s.wire([(cols[3], rows[2] - h), (cols[3], rows[1] + h)], both=True)
    s.wire([(cols[4], rows[2] - h), (cols[4], rows[0] + h)], both=True)
    # горизонтали в ядре
    s.wire([(cols[0] + W / 2, rows[1]), (cols[1] - W / 2, rows[1])])
    s.wire([(cols[1] + W / 2, rows[1]), (cols[2] - W / 2, rows[1])])
    s.wire([(cols[3] - W / 2, rows[1]), (cols[2] + W / 2, rows[1])])
    # servo -> MAC
    s.wire([(cols[1] + 40, rows[1] + h), (cols[1] + 40, 340), (cols[2] - 40, 340),
            (cols[2] - 40, rows[2] - h)])
    s.label(cols[1] + 48, 334, "addend")
    # MAC -> pps_capture (ITR1)
    s.wire([(cols[2] - W / 2, rows[2] + 20), (cols[1] + W / 2, rows[2] + 20)])
    s.label(cols[2] - W / 2 - 4, rows[2] + 8, "ITR1", "end")
    # шина timekeeper -> сервисы
    bus_y = 200
    s.line([(cols[2], rows[1] - h), (cols[2], bus_y)])
    s.line([(cols[0], bus_y), (cols[4] - 30, bus_y)])
    for c in range(5):
        x = cols[c] if c < 4 else cols[4] - 30
        if c == 3:
            continue
        s.wire([(x, bus_y), (x, rows[0] + h)])
    s.label(cols[3] - 70, bus_y - 6, "zbus: время и состояние")
    s.save("software.svg")


# ---------------------------------------------------------------- 4. Стенд
def stand():
    s = Svg(900, 560, "DOROGO: стенд платы №1")
    s.heading("Стенд платы №1: антенна и приборы общие на два экземпляра",
              "Показан один стенд; второй получает второй выход сплиттера и второй канал Rigol")
    L, R, BX = 150, 750, 450
    s.box(BX, 320, 220, 400, "Плата №1",
          ["× 2 идентичных стенда", "", "доработки 1 и 10", "R15 пока на месте",
           "без платы питания"], "main")
    left = [(150, "Антенна L1/L2", ["активный сплиттер 1:2", "второй выход → стенд B"], "ANT"),
            (320, "ЛБП 3,3 В, 0,5 А", ["X2: 13–14 +3V3", "X2: 17–18 GND"], "X2"),
            (490, "Rigol DG1022", ["CH1 25 МГц → стенд A", "CH2 → стенд B"], "25MHz")]
    for y, n, ln, lab in left:
        s.box(L, y, 220, 70, n, ln)
        s.wire([(L + 110, y), (BX - 110, y)])
        s.label((L + 110 + BX - 110) / 2, y - 8, lab, "middle")
    right = [(150, "Коммутатор + ПК", ["PTP / NTP", "сервер образов"], "RJ45", True),
             (260, "ПК по USB", ["USB-B: shell STM32", "micro-USB: u-center"], "USB × 2", True),
             (380, "ST-LINK V3", ["SWD, SWO, nRESET", "через переходник X1"], "X1", True),
             (490, "Анализатор / TIC", ["трасса ETM, PPS A и B", "SLogic16U3 · Hantek 4032L"], "PPS, трасса", False)]
    for y, n, ln, lab, both in right:
        s.box(R, y, 220, 70, n, ln)
        s.wire([(BX + 110, y), (R - 110, y)], both=both)
        s.label((BX + 110 + R - 110) / 2, y - 8, lab, "middle")
    s.save("stand.svg")


if __name__ == "__main__":
    os.makedirs(OUT, exist_ok=True)
    system()
    loops()
    software()
    stand()
    print("ok")
