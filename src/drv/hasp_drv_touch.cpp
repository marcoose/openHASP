#include "hasp_drv_touch.h"
#include "hasp/hasp.h"
#include "lvgl.h"

#if TOUCH_DRIVER == 2046
#if defined(USE_FSMC)
#else
#include "tft_espi_drv.h"
#endif

#elif TOUCH_DRIVER == 2046
#include "indev/XPT2046.h"
#elif TOUCH_DRIVER == 0x2046B
#include "drv/touch/hasp_drv_xpt2046.h"
#elif TOUCH_DRIVER == 911
#include "drv/touch/hasp_drv_gt911.h"
#elif TOUCH_DRIVER == 0xADC
#include "drv/touch/hasp_drv_ft6336u.h"
#elif TOUCH_DRIVER == 5206
#include "drv/touch/hasp_drv_ft5206.h"
#elif TOUCH_DRIVER == 6336
#include "drv/touch/hasp_drv_ft6336u.h"
#else
//#include "tp_i2c.h"
//#include "ft6x36.h"
#endif

#include "../hasp/hasp.h" // for hasp_sleep_state
extern uint8_t hasp_sleep_state;
static uint8_t drv_touch_rotation;

void drv_touch_init(uint8_t rotation)
{
    drv_touch_rotation = rotation;

#if TOUCH_DRIVER == 2046 // XPT2046 Resistive touch panel driver
#if defined(USE_FSMC)
    xpt2046_init(rotation);
#else
    // The display driver takes care of all initializations
    // tft_espi_init(rotation);
#endif

#elif TOUCH_DRIVER == 911
    GT911_init();

#elif TOUCH_DRIVER == 0xADC // Analog Digital Touch Conroller
    // Touch_init();

#elif TOUCH_DRIVER == 5206
    FT5206_init();

#elif TOUCH_DRIVER == 6336
    FT6336U_init();

#else
    // xpt2046_alt_drv_read(indev_driver, data);
    // xpt2046_read(indev_driver, data);
    // if(data->state && guiSleeping != HASP_SLEEP_OFF) guiCheckSleep();
#endif
}

static inline bool drv_touchpad_getXY(int16_t * touchX, int16_t * touchY)
{
    bool touched;
    int16_t normal_x;
    int16_t normal_y;
#if TOUCH_DRIVER == 2046 // XPT2046 Resistive touch panel driver
    touched = tft_espi_get_touch(&normal_x, &normal_y, 300u);

#elif TOUCH_DRIVER == 0x2046B
    touched = XPT2046_getXY(&normal_x, &normal_y, true);

#elif TOUCH_DRIVER == 911
    touched = GT911_getXY(&normal_x, &normal_y, true);

#elif TOUCH_DRIVER == 0xADC // Analog Digital Touch Conroller
    touched = Touch_getXY(&normal_x, &normal_y, false);

#elif TOUCH_DRIVER == 5206
    touched = FT5206_getXY(&normal_x, &normal_y, false);

#elif TOUCH_DRIVER == 6336
    touched = FT6336U_getXY(&normal_x, &normal_y, true);

#else
    // xpt2046_alt_drv_read(indev_driver, data);
    // xpt2046_read(indev_driver, data);
    // if(data->state && guiSleeping != HASP_SLEEP_OFF) guiCheckSleep();
    touched = false;
#endif

    *touchX = normal_x;
    *touchY = normal_y;

    /*
        // TFT_ROTATION values:
        // 0 - 0 deg (= default when not set)
        // 1 - 90 deg anti-clockwise (from 0 deg)
        // 2 - 180 deg anti-clockwise
        // 3 - 270 deg anti-clockwise
        // 4 - mirror content, and rotate 180 deg anti-clockwise
        // 5 - mirror content, and rotate 270 deg anti-clockwise
        // 6 - mirror content, and rotate 0 deg anti-clockwise
        // 7 - mirror content, and rotate 90 deg anti-clockwise

        int16_t inv_x = TFT_WIDTH - normal_x;
        int16_t inv_y = TFT_HEIGHT - normal_y;
        // inv_y can be negative for area below screen of M5Core2
        switch(drv_touch_rotation) {
            case 0:
                *touchX = normal_x;
                *touchY = normal_y;
                break;
            case 1:
                *touchX = normal_y;
                *touchY = inv_x;
                break;
            case 2:
                *touchX = inv_x;
                *touchY = inv_y;
                break;
            case 3:
                *touchX = inv_y;
                *touchY = normal_x;
                break;

            // rotations 4-7 are mirrored
            case 4:
                *touchX = inv_y;
                *touchY = inv_x;
                break;
            case 5:
                *touchX = normal_x;
                *touchY = inv_y;
                break;
            case 6:
                *touchX = normal_y;
                *touchY = normal_x;
                break;
            case 7:
                *touchX = inv_x;
                *touchY = normal_y;
                break;
        }*/

    return touched;
}

bool touch_rotate   = false;
bool touch_invert_x = false;
bool touch_invert_y = false;

bool drv_touch_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data)
{
#if TOUCH_DRIVER > 0
    int16_t touchX = 0;
    int16_t touchY = 0;
    bool touched;

    if(touch_rotate) {
        touched = drv_touchpad_getXY(&touchY, &touchX);
    } else {
        touched = drv_touchpad_getXY(&touchX, &touchY);
    }

    // Ignore first press?

    if(touched && hasp_sleep_state != HASP_SLEEP_OFF) hasp_update_sleep_state(); // update Idle

    if(touch_invert_x) {
        touchX = indev_driver->disp->driver.hor_res - touchX;
    }

    if(touch_invert_y) {
        touchY = indev_driver->disp->driver.ver_res - touchY;
    }

    /*Save the state and save the pressed coordinate for cursor position */
    data->state = touched ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
    if(touched) {
        data->point.x = touchX;
        data->point.y = touchY;
    }
#endif

    /*Return `false` because we are not buffering and no more data to read*/
    return false;
}

void drv_touch_loop()
{
#if TOUCH_DRIVER == 911
    GT911_loop();
#endif
}