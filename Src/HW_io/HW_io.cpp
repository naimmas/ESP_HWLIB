/***************************************************************************************************
* File Name: HW_io.c
* Module: HW_io
* Abstract: Implementation of "/lib/HW_io/HW_io.h" module.
* Author: Naim ALMASRI
* Date: 10.06.2024
***************************************************************************************************/

/***************************************************************************************************
* Header files.
***************************************************************************************************/

#include "Arduino.h"
#include "HW_io.h"
#include "debug_logger.h"
#include "esp32-hal-gpio.h"
#include "pin_def.h"

/***************************************************************************************************
* Macro definitions.
***************************************************************************************************/

/* Setting the signal to valid */
#define SET_SIGNAL_VALID(signal) (signal |= 0xF0)
/* Setting the signal to invalid */
#define SET_SIGNAL_INVALID(signal) (signal &= 0x0F)
/* Checking if the signal is valid */
#define IS_SIGNAL_VALID(signal) (signal & 0xF0)
/* Getting the signal value */
#define GET_SIGNAL(signal) (signal & 0x0F)
/* Debounce time for mechanical switches */
#define DEBOUNCE_TIME_MS (100U)

/***************************************************************************************************
* Local type definitions.
***************************************************************************************************/

typedef struct input_pins_hndlr_t_struct
{
  volatile uint32_t last_time;
  volatile signal_state_t signal;
  bool debounce;
} input_pins_hndlr_t;

/***************************************************************************************************
 * Local data definitions.
***************************************************************************************************/

volatile input_pins_hndlr_t g_input_pins_hndlrs[GPIO_NUM_MAX];

/***************************************************************************************************
* Local function definitions.
***************************************************************************************************/

static IRAM_ATTR void isr_gpio_cb(void *arg)
{
  volatile input_pins_hndlr_t * pin_hndlr_ptr = &g_input_pins_hndlrs[(uint32_t)arg];
  uint8_t state = digitalRead((uint32_t)arg);
  pin_hndlr_ptr->signal = SIGNAL_INVALID;
  uint32_t current_time = (uint32_t)millis();
  if (false == pin_hndlr_ptr->debounce ||
      (pin_hndlr_ptr->last_time >= current_time ||
      current_time - pin_hndlr_ptr->last_time >= DEBOUNCE_TIME_MS))
  {
    if (state == HIGH)
    {
      pin_hndlr_ptr->signal = SIGNAL_HIGH;
    }
    else
    {
      pin_hndlr_ptr->signal = SIGNAL_LOW;
    }
    pin_hndlr_ptr->last_time = millis();
  }
}

/***************************************************************************************************
* External data definitions.
***************************************************************************************************/

/***************************************************************************************************
* External function definitions.
***************************************************************************************************/

void init_input_pins(input_pins_t const *p_ptr_in_pins, uint8_t pin_count)
{
  logger_d("Initializing Input pins\n");
  
  // zero-initialize the config structure.
  gpio_config_t io_conf = {};
  for (uint8_t i = 0; i < pin_count; i++)
  {
    switch (p_ptr_in_pins[i].int_mode)
    {
    case INT_MODE_DISABLED:
      io_conf.intr_type = GPIO_INTR_DISABLE;
      break;
    case INT_MODE_RISING:
      io_conf.intr_type = GPIO_INTR_POSEDGE;
      break;
    case INT_MODE_FALLING:
      io_conf.intr_type = GPIO_INTR_NEGEDGE;
      break;
    case INT_MODE_CHANGE:
      io_conf.intr_type = GPIO_INTR_ANYEDGE;
      break;
    case INT_MODE_AT_LOW:
      io_conf.intr_type = GPIO_INTR_LOW_LEVEL;
      break;
    case INT_MODE_AT_HIGH:
      io_conf.intr_type = GPIO_INTR_HIGH_LEVEL;
      break;
    default:
      break;
    }
    io_conf.mode = GPIO_MODE_INPUT;
    // bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = digitalPinToBitMask(p_ptr_in_pins[i].pin);

    switch (p_ptr_in_pins[i].mode)
{
    case PIN_MODE_INPUT_PULLUP:
      // disable pull-down mode
      io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
      // enable pull-up mode
      io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
      break;
    case PIN_MODE_INPUT_PULLDOWN:
      // enable pull-down mode
      io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
      // disable pull-up mode
      io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
      break;
    case PIN_MODE_INPUT:
    default:
      break;
}
    // configure GPIO with the given settings
    gpio_config(&io_conf);
    g_input_pins_hndlrs[p_ptr_in_pins[i].pin].signal = SIGNAL_INVALID;
    g_input_pins_hndlrs[p_ptr_in_pins[i].pin].last_time = 0U;
  }
  // install gpio isr service
  gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
  for (uint8_t i = 0; i < pin_count; i++)
  {
    gpio_isr_handler_add(p_ptr_in_pins[i].pin, isr_gpio_cb, (void *)p_ptr_in_pins[i].pin);
  }
  logger_d("Input pins are initialized\n");
}

/*
 * @brief Initialize IO pins
 */
void init_output_pins(output_pins_t const *p_ptr_out_pins, uint8_t p_pin_count)
{
  logger_d("Initializing Output pins\n");

  for (uint8_t i = 0; i < p_pin_count; i++)
  {
    pinMode(p_ptr_out_pins[i].pin, p_ptr_out_pins[i].mode);
  }
  logger_d("Output pins are initialized\n");
}

signal_state_t get_input_state(gpio_num_t input_pin, bool p_force_update)
{
  signal_state_t ret_val = SIGNAL_INVALID;

  if (p_force_update == true)
  {
    ret_val = (signal_state_t)digitalRead(PINI_LID_SWITCH);
  }
  else if (SIGNAL_INVALID != g_input_pins_hndlrs[input_pin].signal)
  {
    ret_val = g_input_pins_hndlrs[input_pin].signal;
    g_input_pins_hndlrs[input_pin].signal = SIGNAL_INVALID;
  }
  return ret_val;
}

void set_led(led_color_t p_color)
{
  switch (p_color)
  {
  case LED_RED:
    digitalWrite(PINO_LED_RED, HIGH);
    digitalWrite(PINO_LED_GREEN, LOW);
    digitalWrite(PINO_LED_BLUE, LOW);
    break;
  case LED_GREEN:
    digitalWrite(PINO_LED_RED, LOW);
    digitalWrite(PINO_LED_GREEN, HIGH);
    digitalWrite(PINO_LED_BLUE, LOW);
    break;
  case LED_BLUE:
    digitalWrite(PINO_LED_RED, LOW);
    digitalWrite(PINO_LED_GREEN, LOW);
    digitalWrite(PINO_LED_BLUE, HIGH);
    break;
  case LED_WHITE:
    digitalWrite(PINO_LED_RED, HIGH);
    digitalWrite(PINO_LED_GREEN, HIGH);
    digitalWrite(PINO_LED_BLUE, HIGH);
    break;
  case LED_YELLOW:
    digitalWrite(PINO_LED_RED, HIGH);
    digitalWrite(PINO_LED_GREEN, HIGH);
    digitalWrite(PINO_LED_BLUE, LOW);
    break;
  case LED_CYAN:
    digitalWrite(PINO_LED_RED, LOW);
    digitalWrite(PINO_LED_GREEN, HIGH);
    digitalWrite(PINO_LED_BLUE, HIGH);
    break;
  case LED_MAGENTA:
    digitalWrite(PINO_LED_RED, HIGH);
    digitalWrite(PINO_LED_GREEN, LOW);
    digitalWrite(PINO_LED_BLUE, HIGH);
    break;
  case LED_OFF:
    digitalWrite(PINO_LED_RED, LOW);
    digitalWrite(PINO_LED_GREEN, LOW);
    digitalWrite(PINO_LED_BLUE, LOW);
    break;
  default:
    break;
  }
}
