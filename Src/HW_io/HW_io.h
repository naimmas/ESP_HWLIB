/***************************************************************************************************
* File Name: HW_io.h
* Module: HW_io
* Abstract: Interface definition for "/lib/HW_io/HW_io.c" module.
* Author: Naim ALMASRI
* Date: 10.06.2024
* Revision: 1.0
***************************************************************************************************/

#ifndef HW_IO_H
#define HW_IO_H

/***************************************************************************************************
* Header files.
***************************************************************************************************/

#include "HW_comm.h"
#include "esp32-hal-gpio.h"

/***************************************************************************************************
* Macro definitions.
***************************************************************************************************/

/***************************************************************************************************
* External type declarations.
***************************************************************************************************/

typedef enum signal_state_enum
{
    SIGNAL_LOW = 0x00U,
    SIGNAL_HIGH = 0x01U,
    SIGNAL_INVALID = 0x0FU,
} signal_state_t;

typedef enum led_color_enum
{
    LED_RED = 0,
    LED_GREEN,
    LED_BLUE,
    LED_WHITE,
    LED_YELLOW,
    LED_CYAN,
    LED_MAGENTA,
    LED_OFF,
} led_color_t;

typedef struct pin_config_t_struct
{
    gpio_num_t pin;
    uint8_t mode;
} pin_config_t;

/***************************************************************************************************
* External data declarations.
***************************************************************************************************/

/***************************************************************************************************
* External function declarations.
***************************************************************************************************/

void init_io(void);
signal_state_t get_sbc_ctrl_state(void);
signal_state_t get_sbc_sys_state(void);
signal_state_t get_lid_state(bool p_force_update);
void set_led(led_color_t p_color);
void set_buzzer(signal_state_t p_state);
void pwm_set_duty_cycle(uint8_t p_duty_cycle_percent);

#endif /* HW_IO_H */