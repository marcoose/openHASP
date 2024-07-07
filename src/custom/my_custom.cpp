/* MIT License - Copyright (c) 2019-2022 Francis Van Roie
   For full license information read the LICENSE file in the project folder */

// USAGE: - Copy this file and rename it to my_custom.cpp
//        - Change false to true on line 9

#include "hasplib.h"

#if defined(HASP_USE_CUSTOM) && true // <-- set this to true in your code

#include "hasp_debug.h"
#include "ath10.h"

#define HYST 0.5
#define PRESET_HOME 1
#define PRESET_AWAY 0

#define ACTION_ERROR 0
#define ACTION_IDLE 1
#define ACTION_HEATING 2

unsigned int preset = PRESET_AWAY;
unsigned int action = ACTION_ERROR; // default so upon initialization it switches to idle and dispatches
unsigned int target_action = ACTION_IDLE;
unsigned int seconds_until_state_change = 0;

float temp_preset[2] = { 15, 20.5 };
float target_temp;
float turn_on_temp;
float turn_off_temp;

float temperature = 255;
float humidity = 255;
int light = 0;
int prev_light = 0;
bool initialized = false;
unsigned int update_every_minute_counter = 10;
AHTxx aht10(AHTXX_ADDRESS_X38, AHT1x_SENSOR); //sensor address, sensor type


void setAction(int target) {
    if (target_action != target) {
        LOG_INFO(TAG_CUSTOM, "Target state: %d", target);
        target_action = target;
    }
}

void setTemp(float t) {
    target_temp = t;
    turn_on_temp = t - HYST;
    turn_off_temp = t + HYST;
    char buffer[6] = "";
    snprintf_P(buffer, sizeof(buffer), PSTR("%.1f"), target_temp);
    dispatch_state_subtopic("set_temp", buffer);
    LOG_INFO(TAG_CUSTOM, "Target temp: %s", buffer);
}

void setPreset(int p) {
    if (preset != p) {
        preset = p;
        LOG_INFO(TAG_CUSTOM, "Mode: %p", preset);
        setTemp(temp_preset[preset]);
    }
}

void readAht10() {
    temperature = aht10.readTemperature();
    humidity = aht10.readHumidity();
    if (temperature != AHTXX_ERROR) {
        char buffer[8] = "";
        snprintf_P(buffer, sizeof(buffer), PSTR("%.1f"), temperature);
        dispatch_state_subtopic("temperature", buffer);
    } else {
        LOG_ERROR(TAG_CUSTOM, "Error reading AHT10");
        if (aht10.softReset() != true) {
            // failed to recover
            seconds_until_state_change = 0;
            setAction(ACTION_ERROR);
            initialized = false;
        }
        return;
    }
    if (humidity != AHTXX_ERROR) {
        char buffer[8] = "";
        snprintf_P(buffer, sizeof(buffer), PSTR("%.0f"), humidity);
        dispatch_state_subtopic("humidity", buffer);
    }
}

// OpenHASP Hooks

void custom_setup() {
    // Initialization code here
    pinMode(GPIO_NUM_34, INPUT);
    digitalWrite(GPIO_NUM_26, LOW);
    aht10.begin();
}

void custom_loop() {
    // Non-blocking code here, this should execute very fast!
}

void custom_every_second()
{
    if (!initialized) {
        if (aht10.getStatus() == AHTXX_NO_ERROR) {
            LOG_INFO(TAG_CUSTOM, "AHT10 initialized");
            setAction(ACTION_IDLE);
            initialized = true;
            readAht10();
        }
    }
    if (seconds_until_state_change > 0) {
        seconds_until_state_change--;
    }
    if ((seconds_until_state_change == 0) &&
        (action != target_action)) {
        switch (target_action) {
            case ACTION_IDLE:
                // turn off
                action = ACTION_IDLE;
                digitalWrite(GPIO_NUM_26, LOW);
                dispatch_state_subtopic("action", "idle");
                break;
            case ACTION_HEATING:
                // turn on
                action = ACTION_HEATING;
                digitalWrite(GPIO_NUM_26, HIGH);
                dispatch_state_subtopic("action", "heating");
                break;
            case ACTION_ERROR:
                action = ACTION_IDLE;
                digitalWrite(GPIO_NUM_26, LOW);
                dispatch_state_subtopic("action", "off");
        }
        seconds_until_state_change = 300;
    }
}

void custom_every_5seconds() {
    // Read sensors
    light = analogRead(GPIO_NUM_34) / 100;
    if (light != prev_light) {
        prev_light = light;
        char buffer[8] = "";
        snprintf_P(buffer, sizeof(buffer), PSTR("%d"), light);
        dispatch_state_subtopic("light", buffer);
    }
    update_every_minute_counter++;
    if (update_every_minute_counter > 12) {
        update_every_minute_counter = 0;
        readAht10();
    }

    // Determine action
    if (action == ACTION_IDLE && temperature <= turn_on_temp) {
        setAction(ACTION_HEATING);
    }

    if (action == ACTION_HEATING && temperature >= turn_off_temp) {
       setAction(ACTION_IDLE);
    }
}

bool custom_pin_in_use(uint8_t pin) {
    switch (pin) {
        case 21:
        case 22:
            return true;
    }
    return false;
}

void custom_get_sensors(JsonDocument& doc) {
}

void custom_topic_payload(const char* topic, const char* payload, uint8_t source){
    bool update = strlen(payload) > 0;
    LOG_ERROR(TAG_CUSTOM, "Handling custom message: %s => %s", topic, payload);

    if (update) {
        if (!strcmp(topic, "preset_mode")) {
            if (!strcmp(payload, "home")) {
                setPreset(PRESET_HOME);
            }
            if (!strcmp(payload, "away")) {
                setPreset(PRESET_AWAY);
            }
        }
        if (!strcmp(topic, "temperature_set")) {
            setTemp(atof(payload));
        }
    }
}

#endif // HASP_USE_CUSTOM