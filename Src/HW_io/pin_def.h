/***************************************************************************************************
* File Name: pin_def.h
* Module: 
* Abstract: Definition of system pins.
* Author: Naim ALMASRI
* Date: 02.06.2024
* Revision: 1.0
***************************************************************************************************/

#ifndef PIN_DEF_H
#define PIN_DEF_H

/***************************************************************************************************
* Header files.
***************************************************************************************************/

#include "HW_io.h"

/***************************************************************************************************
* Macro definitions.
***************************************************************************************************/

/*
 * @brief PINI = Pin Input
 * @brief PINO = Pin Output
 * @brief PINP = Pin PWM
 * @brief PINA = Pin Analog
*/

/* SBC system is ready signal input pin = 1 means ready, connected to 35 on SBC */
#define PINI_SBC_SIG_SYS        (GPIO_NUM_35)
/* SBC analysis result signal input pin, connected to 31 on SBC*/
#define PINI_SBC_SIG_ANLZ       (GPIO_NUM_34)
/* SBC analysis result ready input pin = 1 means analysis is done, connected to 29 on SBC*/
#define PINI_SBC_CONTROL_SIG    (GPIO_NUM_32) 
/* Lid switch input pin*/
#define PINI_LID_SWITCH         (GPIO_NUM_5)
/* SBC analysis starting trigger output pin = 1 means start analysis, connected to 33 on SBC*/
#define PINO_SBC_START_ANLZ     (GPIO_NUM_33)
/* LED output pins */
#define PINO_LED_GREEN   (GPIO_NUM_18)
#define PINO_LED_RED     (GPIO_NUM_19)
#define PINO_LED_BLUE    (GPIO_NUM_21)
/* Buzzer output pin */
#define PINO_BUZZER      (GPIO_NUM_16)
/* Motor output pins */
#define PINP_MOTOR       (GPIO_NUM_4)
#define PINO_MOTOR_DIR_L (GPIO_NUM_14)
#define PINO_MOTOR_DIR_R (GPIO_NUM_27)

/* PWM frequency */
#define PWM_FRQ ((uint32_t) 20000U)
/* PWM output channel */
#define PWM_CHN ((uint8_t) 0U)
/* PWM resolution 8 bits */
/* ! incresing it will decrease the max allowed PWM frequency */
#define PWM_RES ((uint8_t) 8U)

#define TOTAL_IO   (sizeof(g_io_pins) / sizeof(pin_config_t))

/***************************************************************************************************
* External type declarations.
***************************************************************************************************/

const pin_config_t g_io_pins[] = {
    { PINI_SBC_SIG_SYS      , INPUT_PULLDOWN},
    { PINI_SBC_SIG_ANLZ     , INPUT_PULLDOWN },
    { PINI_SBC_CONTROL_SIG  , INPUT_PULLDOWN },
    { PINI_LID_SWITCH       , INPUT_PULLDOWN },
    { PINO_SBC_START_ANLZ   , OUTPUT },
    { PINO_LED_GREEN        , OUTPUT },
    { PINO_LED_RED          , OUTPUT },
    { PINO_LED_BLUE         , OUTPUT },
    { PINO_BUZZER           , OUTPUT },
    { PINO_MOTOR_DIR_L      , OUTPUT },
    { PINO_MOTOR_DIR_R      , OUTPUT }
};

#endif /* PIN_DEF_H */