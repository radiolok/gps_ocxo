/*
 * Прошивка для проверки индикатора ИЛЦ1-8/7ЛВ (советский ВЛИ часового типа)
 * на базе Arduino Nano + PlatformIO.
 *
 * Управление: 2x SN755870 в цепочке SPI.
 *
 * Выбор демо-режима — раскомментируйте нужную строку в setup().
 */

#include <Arduino.h>
#include "display/ilc1_8.h"
#include "demos/demo_seconds.h"
#include "demos/demo_clock.h"
#include "demos/demo_test_all.h"
#include "demos/demo_discovery.h"
#include "demos/demo_blink.h"
#include "demos/demo_scan.h"
#include "demos/demo_full_test.h"
#include "demos/demo_calibrate.h"

ILC1_8Display gDisplay;

// Демо-объекты (создаются в setup() чтобы избежать static init order fiasco)
demo::DemoSeconds*   pDemoSeconds   = nullptr;
demo::DemoClock*     pDemoClock     = nullptr;
demo::DemoTestAll*   pDemoTestAll   = nullptr;
demo::DemoDiscovery* pDemoDiscovery = nullptr;
demo::DemoBlink*     pDemoBlink     = nullptr;
demo::DemoScan*      pDemoScan      = nullptr;
demo::DemoFullTest*  pDemoFullTest  = nullptr;
demo::DemoCalibrate* pDemoCalibrate = nullptr;

// Выбор активного демо
enum ActiveDemo {
    DEMO_SECONDS,
    DEMO_CLOCK,
    DEMO_TEST_ALL,
    DEMO_DISCOVERY,
    DEMO_BLINK,
    DEMO_SCAN,
    DEMO_FULL_TEST,
    DEMO_CALIBRATE
};

ActiveDemo gActiveDemo = DEMO_TEST_ALL;  // <-- ВЫБОР ДЕМО

void setup() {
    gDisplay.begin();
    gDisplay.filamentOn();
    gDisplay.setBrightness(128);  // 50% яркости для начала

    switch (gActiveDemo) {
        case DEMO_SECONDS: {
            pDemoSeconds = new demo::DemoSeconds(gDisplay);
            pDemoSeconds->begin();
            break;
        }
        case DEMO_CLOCK: {
            pDemoClock = new demo::DemoClock(gDisplay);
            pDemoClock->begin();
            break;
        }
        case DEMO_TEST_ALL: {
            pDemoTestAll = new demo::DemoTestAll(gDisplay);
            pDemoTestAll->begin();
            break;
        }
        case DEMO_DISCOVERY: {
            pDemoDiscovery = new demo::DemoDiscovery(gDisplay);
            pDemoDiscovery->begin();
            break;
        }
        case DEMO_BLINK: {
            pDemoBlink = new demo::DemoBlink(gDisplay);
            pDemoBlink->begin();
            break;
        }
        case DEMO_SCAN: {
            pDemoScan = new demo::DemoScan(gDisplay);
            pDemoScan->begin();
            break;
        }
        case DEMO_FULL_TEST: {
            pDemoFullTest = new demo::DemoFullTest(gDisplay);
            pDemoFullTest->begin();
            break;
        }
        case DEMO_CALIBRATE: {
            pDemoCalibrate = new demo::DemoCalibrate(gDisplay);
            pDemoCalibrate->begin();
            break;
        }
    }
}

void loop() {
    switch (gActiveDemo) {
        case DEMO_SECONDS:
            if (pDemoSeconds) pDemoSeconds->run();
            break;
        case DEMO_CLOCK:
            if (pDemoClock) pDemoClock->run();
            break;
        case DEMO_TEST_ALL:
            if (pDemoTestAll) pDemoTestAll->run();
            break;
        case DEMO_DISCOVERY:
            if (pDemoDiscovery) pDemoDiscovery->run();
            break;
        case DEMO_BLINK:
            if (pDemoBlink) pDemoBlink->run();
            break;
        case DEMO_SCAN:
            if (pDemoScan) pDemoScan->run();
            break;
        case DEMO_FULL_TEST:
            if (pDemoFullTest) pDemoFullTest->run();
            break;
        case DEMO_CALIBRATE:
            if (pDemoCalibrate) pDemoCalibrate->run();
            break;
    }
}
