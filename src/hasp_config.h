/* MIT License - Copyright (c) 2019-2021 Francis Van Roie
   For full license information read the LICENSE file in the project folder */

#if HASP_USE_CONFIG > 0

#ifndef HASP_CONFIG_H
#define HASP_CONFIG_H

#include "hasp_conf.h"
#include "ArduinoJson.h"
#include "hasp_debug.h" // for TAG_CONF

/* ===== Default Event Processors ===== */
void configSetup(void);
void configLoop(void);
void configEverySecond(void);
void configStart(void);
void configStop(void);

/* ===== Special Event Processors ===== */
void configWrite(void);
void configOutput(const JsonObject& settings, uint8_t tag = TAG_CONF);
bool configClearEeprom(void);

/* ===== Getter and Setter Functions ===== */
bool configSet(int8_t& value, const JsonVariant& setting, const __FlashStringHelper* fstr_name);
bool configSet(uint8_t& value, const JsonVariant& setting, const __FlashStringHelper* fstr_name);
bool configSet(uint16_t& value, const JsonVariant& setting, const __FlashStringHelper* fstr_name);

/* ===== Read/Write Configuration ===== */
void configSetConfig(JsonObject& settings);
void configGetConfig(JsonDocument& settings);

/* json keys used in the configfile */
const char FP_CONFIG_STARTPAGE[] PROGMEM = "startpage";
const char FP_CONFIG_STARTDIM[] PROGMEM  = "startdim";
const char FP_CONFIG_THEME[] PROGMEM     = "theme";
const char FP_CONFIG_HUE[] PROGMEM       = "hue";
const char FP_CONFIG_ZIFONT[] PROGMEM    = "font";
const char FP_CONFIG_PAGES[] PROGMEM     = "pages";
const char FP_CONFIG_ENABLE[] PROGMEM    = "enable";
const char FP_CONFIG_HOST[] PROGMEM      = "host";
const char FP_CONFIG_PORT[] PROGMEM      = "port";
const char FP_CONFIG_NAME[] PROGMEM      = "name";
const char FP_CONFIG_USER[] PROGMEM      = "user";
const char FP_CONFIG_PASS[] PROGMEM      = "pass";
const char FP_CONFIG_SSID[] PROGMEM      = "ssid";
const char FP_CONFIG_GROUP[] PROGMEM     = "group";
const char FP_CONFIG_BAUD[] PROGMEM      = "baud";
const char FP_CONFIG_LOG[] PROGMEM       = "log";
const char FP_CONFIG_PROTOCOL[] PROGMEM  = "proto";
const char FP_GUI_ROTATION[] PROGMEM     = "rotate";
const char FP_GUI_INVERT[] PROGMEM       = "invert";
const char FP_GUI_TICKPERIOD[] PROGMEM   = "tick";
const char FP_GUI_IDLEPERIOD1[] PROGMEM  = "idle1";
const char FP_GUI_IDLEPERIOD2[] PROGMEM  = "idle2";
const char FP_GUI_CALIBRATION[] PROGMEM  = "calibration";
const char FP_GUI_BACKLIGHTPIN[] PROGMEM = "bckl";
const char FP_GUI_POINTER[] PROGMEM      = "cursor";
const char FP_DEBUG_TELEPERIOD[] PROGMEM = "tele";
const char FP_GPIO_CONFIG[] PROGMEM      = "config";

const char FP_HASP_CONFIG_FILE[] PROGMEM = "/config.json";

const char FP_WIFI[] PROGMEM  = "wifi";
const char FP_MQTT[] PROGMEM  = "mqtt";
const char FP_HTTP[] PROGMEM  = "http";
const char FP_GPIO[] PROGMEM  = "gpio";
const char FP_MDNS[] PROGMEM  = "mdns";
const char FP_HASP[] PROGMEM  = "hasp";
const char FP_GUI[] PROGMEM   = "gui";
const char FP_DEBUG[] PROGMEM = "debug";

#endif

#endif // HASP_USE_CONFIG