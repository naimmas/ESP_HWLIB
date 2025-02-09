/***************************************************************************************************
* File Name: HW_io.h
* Module: HW_io
* Abstract: Interface definition for "/lib/HW_io/HW_io.c" module.
* Author: Naim ALMASRI
* Date: 10.06.2024
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

typedef enum signal_state_t_enum
{
    SIGNAL_LOW = 0x00U,
    SIGNAL_HIGH = 0x01U,
    SIGNAL_INVALID = 0x0FU,
} signal_state_t;

typedef enum led_color_t_enum
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

typedef enum out_pin_mode_t_enum
{
    PIN_MODE_OUTPUT = OUTPUT,
    PIN_MODE_OUTPUT_OPEN_DRAIN = OUTPUT_OPEN_DRAIN,
    PIN_MODE_ANALOG_IN = ANALOG,
}out_pin_mode_t;

typedef enum in_pin_mode_t_enum
{
    PIN_MODE_INPUT = INPUT,
    PIN_MODE_INPUT_PULLUP = INPUT_PULLUP,
    PIN_MODE_INPUT_PULLDOWN = INPUT_PULLDOWN,
    PIN_MODE_INPUT_OPEN_DRAIN = OPEN_DRAIN,
    PIN_MODE_ANALOG_OUT = ANALOG,
}in_pin_mode_t;

typedef enum interrupt_mode_t_enum
{
    INT_MODE_DISABLED,
    INT_MODE_RISING,
    INT_MODE_FALLING,
    INT_MODE_CHANGE,
    INT_MODE_AT_LOW,
    INT_MODE_AT_HIGH,
}interrupt_mode_t;

typedef struct input_pins_t_struct
{
    gpio_num_t pin;
    in_pin_mode_t mode;
    interrupt_mode_t int_mode;
    bool debounce_en;
} input_pins_t;

typedef struct output_pins_t_struct
{
    gpio_num_t pin;
    out_pin_mode_t mode;
} output_pins_t;

/***************************************************************************************************
* External data declarations.
***************************************************************************************************/

/***************************************************************************************************
* External function declarations.
***************************************************************************************************/

void init_input_pins(input_pins_t const *p_ptr_in_pins, uint8_t pin_count);
void init_output_pins(output_pins_t const *p_ptr_out_pins, uint8_t pin_count);
signal_state_t get_input_state(gpio_num_t input_pin, bool p_force_update);
void set_led(led_color_t p_color);

#endif /* HW_IO_H */