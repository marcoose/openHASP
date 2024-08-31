/* MIT License - Copyright (c) 2019-2022 Francis Van Roie
   For full license information read the LICENSE file in the project folder */

// USAGE: - Copy this file and rename it to my_custom.cpp
//        - Change false to true on line 9

#include "hasplib.h"

#include "hasp_debug.h"
#include <Wire.h>
#include "VL53L0X.h"

#define I2C_SDA GPIO_NUM_17
#define I2C_SCL GPIO_NUM_18

TwoWire I2C_VL53 = TwoWire(15);
VL53L0X vl53;


bool initialized = false;
uint16_t dist = 0;

// OpenHASP Hooks

void custom_setup() {
    LOG_DEBUG(TAG_CUSTOM, "Custom setup");
    I2C_VL53.begin(I2C_SDA, I2C_SCL, 100000);
    LOG_DEBUG(TAG_CUSTOM, "I2C Turned On");
    vl53.setBus(&I2C_VL53);
    vl53.setTimeout(500);
    LOG_DEBUG(TAG_CUSTOM, "Bus and timeout set");
    // pinMode(I2C_SCL, OUTPUT);
    // pinMode(I2C_SDA, OUTPUT);
}

void custom_loop() {
    // Non-blocking code here, this should execute very fast!
}

void custom_every_second() {
    if (!initialized) {
        if (vl53.init(true)) {
            LOG_DEBUG(TAG_CUSTOM, "VL53L0X Initialized");
            initialized = true;
        } else {
            LOG_DEBUG(TAG_CUSTOM, "VL35L0X Init Fail");
        }
    } else {
        dist = vl53.readRangeSingleMillimeters();
        if (vl53.timeoutOccurred()) {
            LOG_DEBUG(TAG_CUSTOM, "VL53L0X Timeout");
        } else {
            LOG_DEBUG(TAG_CUSTOM, "Dist %d", dist);
            if (dist < 300) {
                if (hasp_get_sleep_state() == HASP_SLEEP_OFF) {
                    LOG_DEBUG(TAG_CUSTOM, "Detected, but already on");
                } else {
                    LOG_DEBUG(TAG_CUSTOM, "Was off - dispatch wakeup");
                    dispatch_wakeup(0);
                }
            }
        }
    }
}

void custom_every_5seconds() {
    // if (initialized) {
    //     LOG_DEBUG(TAG_CUSTOM, "Pins high");
    //     digitalWrite(I2C_SDA, HIGH);
    //     digitalWrite(I2C_SCL, HIGH);
    //     initialized = false;
    // } else {
    //     LOG_DEBUG(TAG_CUSTOM, "Pins low");
    //     digitalWrite(I2C_SDA, LOW);
    //     digitalWrite(I2C_SCL, LOW);
    //     initialized = true;
    // }


    // not used
//   byte error, address;
//   int nDevices;
//   LOG_DEBUG(TAG_CUSTOM, "Scanning...");
//   nDevices = 0;
//   for(address = 1; address < 127; address++ ) {
//     I2C_VL53.begin(address, I2C_SDA, I2C_SCL, 100000);
//     error = I2C_VL53.endTransmission();
//     if (error == 0) {
//       LOG_DEBUG(TAG_CUSTOM, "I2C device found at address 0x%d", address);
//       nDevices++;
//     }
//     else if (error==4) {
//       LOG_DEBUG(TAG_CUSTOM, "Unknown error at address 0x%d", address);
//     }
//   }
//   if (nDevices == 0) {
//     LOG_DEBUG(TAG_CUSTOM, "No I2C devices found");
//   }
}

bool custom_pin_in_use(uint8_t pin) {
    switch (pin) {
        case I2C_SCL:
        case I2C_SDA:
            return true;
    }
    return false;
}

void custom_get_sensors(JsonDocument& doc) {
    // not used
}

void custom_topic_payload(const char* topic, const char* payload, uint8_t source){
    // not used
}

void custom_state_subtopic(const char* subtopic, const char* payload) {
    // not used
}
