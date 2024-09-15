#include "hasplib.h"
#include "hasp_debug.h"

void custom_setup()
{
    pinMode(GPIO_NUM_34, INPUT);
}

void custom_loop()
{
    // Non-blocking code here, this should execute very fast!
}

void custom_every_second()
{

}

void custom_every_5seconds()
{

}

bool custom_pin_in_use(uint8_t pin)
{
    switch (pin) {
      case GPIO_NUM_34:
        return true;
    }
    return false;
}

void custom_get_sensors(JsonDocument& doc)
{
    int light = analogRead(GPIO_NUM_34);
    doc[F("light")] = light;
}

void custom_topic_payload(const char* topic, const char* payload, uint8_t source){
    // Not used
}

void custom_state_subtopic(const char* subtopic, const char* payload){
    // Not used
}
