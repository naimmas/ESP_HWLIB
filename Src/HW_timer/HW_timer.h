/***************************************************************************************************
* File Name: HW_timer.h
* Module: Hw_timer
* Abstract: Interface definition for "lib/HW_timer/HW_timer.c" module.
* Author: Naim ALMASRI
* Date: 10.06.2024
* Revision: 1.0
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

extern void sys_timer_main();

extern void sys_timer_init();

extern timer_handler_t * timer_allocate(uint32_t p_time_period,
                                time_unit_t p_time_unit,
                                timer_callback_t p_timer_cb,
                                bool p_one_shot);

extern void timer_activate(timer_id_t p_timer_id);

extern void timer_deactivate(timer_id_t p_timer_id);

extern void timer_update_period(timer_id_t p_timer_id ,uint32_t p_time_period, time_unit_t p_time_unit);

extern void timer_update_oneshot(timer_id_t p_timer_id, bool p_one_shot);

extern void timer_clear(timer_id_t p_timer_id);

extern void hard_delay(uint16_t p_delay_time_ms);

#endif /* HW_TIMER_H */