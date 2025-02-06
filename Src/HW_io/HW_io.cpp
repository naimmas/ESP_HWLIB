/***************************************************************************************************
* File Name: HW_io.c
* Module: HW_io
* Abstract: Implementation of "/lib/HW_io/HW_io.h" module.
* Author: Naim ALMASRI
* Date: 10.06.2024
* Revision: 1.0
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

/* Lid switch signal state holder */
static uint8_t s_lid_switch_sig = 0;
/* SBC analysis control signal state holder */
static uint8_t s_sbc_ctrl_sig = 0;

/***************************************************************************************************
 * Local data definitions.
***************************************************************************************************/

/***************************************************************************************************
* Local function definitions.
***************************************************************************************************/

/*
 * @brief Interrupt service routine for the lid switch.
 * @brief This function is called when the lid switch state changes.
*/
static IRAM_ATTR void isr_lid_switch(void)
{
  static uint32_t s_lid_switch_time_prev = 0;
  uint32_t lid_switch_time = millis();
  
  if (lid_switch_time - s_lid_switch_time_prev > DEBOUNCE_TIME_MS)
  {
    s_lid_switch_sig = digitalRead(PINI_LID_SWITCH);
    SET_SIGNAL_VALID(s_lid_switch_sig);
    s_lid_switch_time_prev = lid_switch_time;
  }
}

/*
 * @brief Interrupt service routine for the SBC analysis control signal.
 * @brief This function is called when the SBC control signal state changes to HIGH.
*/
static IRAM_ATTR void isr_sbc_sig_cntrl(void)
{
  s_sbc_ctrl_sig = digitalRead(PINI_SBC_CONTROL_SIG);
  SET_SIGNAL_VALID(s_sbc_ctrl_sig);
}

/***************************************************************************************************
* External data definitions.
***************************************************************************************************/

/***************************************************************************************************
* External function definitions.
***************************************************************************************************/

/*
 * @brief Initialize IO pins
*/
void init_io(void)
{
  logger_d(__func__, "Initializing IO pins\n");
  
  for (uint8_t i = 0; i < TOTAL_IO; i++)
  {
    pinMode(g_io_pins[i].pin, g_io_pins[i].mode);
  }

  ledcSetup(PWM_CHN, PWM_FRQ, PWM_RES);
  ledcAttachPin(PINP_MOTOR, PWM_CHN);
  attachInterrupt(digitalPinToInterrupt(PINI_LID_SWITCH), isr_lid_switch, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PINI_SBC_CONTROL_SIG), isr_sbc_sig_cntrl, RISING);
  logger_d(NULL, "IO pins are initialized\n");
}

/*
  * @brief This function returns the state of the SBC analyze control signal
  * @return SIGNAL_LOW if the SBC analysis is not ready, 
  *         SIGNAL_HIGH if the SBC analysis is ready,
  *         SIGNAL_INVALID no change in the SBC control signal
*/
signal_state_t get_sbc_ctrl_state(void)
{
  signal_state_t sbc_ctrl_state = SIGNAL_INVALID;

  if (IS_SIGNAL_VALID(s_sbc_ctrl_sig))
  {
    sbc_ctrl_state = (signal_state_t)GET_SIGNAL(s_sbc_ctrl_sig);
    SET_SIGNAL_INVALID(s_sbc_ctrl_sig);
  }

  return sbc_ctrl_state;
}

/*
  * @brief This function returns the state of the SBC system signal
  * @return SIGNAL_LOW if the SBC system is not ready or down, 
  *         SIGNAL_HIGH if the SBC system is ready,
  *         SIGNAL_INVALID no change in the SBC system signal
*/
signal_state_t get_sbc_sys_state(void)
{
  signal_state_t sbc_sys_state = SIGNAL_INVALID;

  if (digitalRead(PINI_SBC_SIG_SYS) == HIGH)
  {
    sbc_sys_state = SIGNAL_HIGH;
  }
  else
  {
    sbc_sys_state = SIGNAL_LOW;
  }

  return sbc_sys_state;
}

/*
  * @brief This function returns the state of the lid switch
  * @param p_force_update input: true to force update the lid state (+debounce),
  *                              false to return the value from the last ISR update
  * @return SIGNAL_LOW if the lid is close, 
  *         SIGNAL_HIGH if the lid is open,
  *         SIGNAL_INVALID no change in the lid state
*/
signal_state_t get_lid_state(bool p_force_update)
{
  signal_state_t lid_state = SIGNAL_INVALID;
  uint32_t sw_time = millis();

  if (p_force_update == true)
  {
    lid_state = (signal_state_t)digitalRead(PINI_LID_SWITCH);
    while (millis() - sw_time < DEBOUNCE_TIME_MS)
    {
      ;
    }

    if (digitalRead(PINI_LID_SWITCH) != lid_state)
    {
      lid_state = SIGNAL_INVALID;
      SET_SIGNAL_INVALID(s_lid_switch_sig);
    }
    
  }
  else
  {
    if (IS_SIGNAL_VALID(s_lid_switch_sig))
    {
      lid_state = (signal_state_t)GET_SIGNAL(s_lid_switch_sig);
      SET_SIGNAL_INVALID(s_lid_switch_sig);
    }
  }

  return lid_state;
}

/*
  * @brief This function sets the duty cycle of the PWM signal
  * @param p_duty_cycle_percent input: The duty cycle percentage
*/
void pwm_set_duty_cycle(uint8_t p_duty_cycle_percent)
{
/* Convert speed from percentage to duty cycle */
#define SPD_TO_DUTY(spd) (uint32_t)((uint32_t)spd * ((1U << PWM_RES) - 1U) / 100U)

  if(p_duty_cycle_percent > 100)
  {
    p_duty_cycle_percent = 100;
  }
  
  ledcWrite(PWM_CHN, SPD_TO_DUTY(p_duty_cycle_percent));
}

void set_buzzer(signal_state_t p_state)
{
  if (p_state != SIGNAL_INVALID)
  {
    digitalWrite(PINO_BUZZER, p_state);
  }
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
