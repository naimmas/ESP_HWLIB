/***************************************************************************************************
* File Name: HW_timer.h
* Module: Hw_timer
* Abstract: Interface definition for "lib/HW_timer/HW_timer.c" module.
* Author: Naim ALMASRI
* Date: 10.06.2024
***************************************************************************************************/

#ifndef HW_TIMER_H
#define HW_TIMER_H

/***************************************************************************************************
* Header files.
***************************************************************************************************/

#include "HW_comm.h"

/***************************************************************************************************
* Macro definitions.
***************************************************************************************************/

/***************************************************************************************************
* External type declarations.
***************************************************************************************************/

typedef void (*timer_callback_t)(void);

typedef enum time_unit_t_enum
{
    TIME_UNIT_MS = 1,
    TIME_UNIT_S = 10,
    TIME_UNIT_MIN = 600,
} time_unit_t;

typedef enum timer_id_t_enum
{
    NO_TIMER = -1,
    TIMER_ID_0,
    TIMER_ID_1,
    TIMER_ID_2,
    TIMER_ID_3,
    TIMER_ID_4,
    TIMER_ID_5,
    TIMER_ID_6,
    TIMER_ID_7,
    TIMERS_COUNT,
} timer_id_t;

typedef struct timer_handler_t_struct
{
    timer_id_t timer_id;
    bool timer_flag;
} timer_handler_t;

/***************************************************************************************************
* External data declarations.
***************************************************************************************************/

/***************************************************************************************************
* External function declarations.
***************************************************************************************************/

/**
 * @brief This function is the main function of the timer module. 
 * It is responsible for checking the user timers and firing the callback functions.
 * It should be called in the main loop.
 */
extern void sys_timer_main();

/**
 * @brief This function initializes the system timer module.
 * It should be called before allocating any user timers.
 */
extern void sys_timer_init();

/**
 * @brief This function allocates a user timer, the minimum time period is 100ms.
 * @param p_time_period input: The period of the timer in the selected time unit.
 * @param p_time_unit   input: The time unit of the timer.
 * @param p_timer_cb    input: The callback function of the timer.
 * @param p_one_shot    input: The one shot flag of the timer, if true the timer will be run only once.
 * @return The ID of the allocated timer;
 *         NO_TIMER if no timer is available.
 */
extern timer_handler_t * timer_allocate(uint32_t p_time_period,
                                time_unit_t p_time_unit,
                                timer_callback_t p_timer_cb,
                                bool p_one_shot);

/**
 * @brief This function restarts the user timer, has no effect on multiple calls.
 * @param p_timer_id input: The ID of the timer.
 */
extern void timer_activate(timer_id_t p_timer_id);

/**
 * @brief This function stops the user timer.
 * @param p_timer_id input: The ID of the timer.
 */
extern void timer_deactivate(timer_id_t p_timer_id);

/**
 * @brief This function sets the period of the user timer.
 * @param p_timer_id    input: The ID of the timer.
 * @param p_time_period input: The period of the timer in the selected time unit.
 * @param p_time_unit   input: The time unit of the timer.
 */
extern void timer_update_period(timer_id_t p_timer_id ,uint32_t p_time_period, time_unit_t p_time_unit);

/**
 * @brief This function sets the one shot flag of the timer.
 * 
 * @param p_timer_id input: The ID of the timer.
 * @param p_one_shot input: The one shot flag of the timer, 
 *                          if true the timer will be run only once
 *                          if false the timer will be run periodically
 */
extern void timer_update_oneshot(timer_id_t p_timer_id, bool p_one_shot);

/**
 * @brief This function resets timer to 0
 * 
 * @param p_timer_id input: The ID of the timer.
 */
extern void timer_reset(timer_id_t p_timer_id); 

/**
 * @brief This function deletes the user timer.
 * @param p_timer_id input: The ID of the timer.
 */
extern void timer_clear(timer_id_t p_timer_id);

/**
 * @brief This function pauses the program for the amount of time (in milliseconds) 
 * 
 * @param p_delay_time_ms the number of milliseconds to pause.
 * @attention allowed maximum value is 1000ms
 */
extern void hard_delay(uint16_t p_delay_time_ms);

#endif /* HW_TIMER_H */