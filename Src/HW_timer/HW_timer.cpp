/***************************************************************************************************
* File Name: HW_timer.c
* Module: HW_timer
* Abstract: Implementation of "lib/HW_timer/HW_timer.h" module.
* Author: Naim ALMASRI
* Date: 10.06.2024
***************************************************************************************************/

/***************************************************************************************************
* Header files.
***************************************************************************************************/

#include "Arduino.h"
#include "HW_timer.h"
#include "debug_logger.h"
#include "esp32-hal-timer.h"

/***************************************************************************************************
* Macro definitions.
***************************************************************************************************/

/***************************************************************************************************
* Local type definitions.
***************************************************************************************************/

typedef struct main_timer_t_struct
{
    uint32_t timer_period;
    time_unit_t time_unit;
    hw_timer_t * timer_hnd_ptr;
    bool is_active;
} main_timer_t;

typedef struct user_timer_t_struct
{
    bool is_active;
    bool one_shot;
    timer_handler_t user_timer_hnd;
    time_unit_t time_unit;
    uint32_t timer_period;
    timer_callback_t timer_cb;
    uint64_t timer_counter;
} user_timer_t;

/***************************************************************************************************
 * Local data definitions.
***************************************************************************************************/

volatile static SemaphoreHandle_t s_timer_semaphore;
static uint32_t s_timer_counter_100ms = 0U;
static main_timer_t s_main_timer = {.timer_period = 100U,
                                    .time_unit = TIME_UNIT_MS,
                                    .timer_hnd_ptr = NULL,
                                    .is_active = false};
static user_timer_t s_user_timers[TIMERS_COUNT];

/***************************************************************************************************
* Local function definitions.
***************************************************************************************************/

/*
 * @brief This function is the ISR of the system timer module.
*/
static void timer_isr()
{
    s_timer_counter_100ms++;

    /* release the semaphore for the main loop */
    xSemaphoreGiveFromISR(s_timer_semaphore, NULL);
}

/***************************************************************************************************
* External data definitions.
***************************************************************************************************/

/***************************************************************************************************
* External function definitions.
***************************************************************************************************/

/*
 * @brief This function is the main function of the timer module. 
 * It is responsible for checking the user timers and firing the callback functions.
 * It should be called in the main loop.
*/
void sys_timer_main()
{
    /* Check if the semaphore is taken */
    if(xSemaphoreTake(s_timer_semaphore, 10) == pdTRUE)
    {
        /* Iterate over the user timers */
        for(uint8_t i = 0; i < TIMERS_COUNT; i++)
        {
            if(s_user_timers[i].is_active == true)
            {
                s_user_timers[i].timer_counter++;
                /* Check if the timer is fired */
                if((uint64_t)(s_user_timers[i].timer_period * 
                   (uint32_t)s_user_timers[i].time_unit) == 
                    s_user_timers[i].timer_counter
                )
                {
                    logger_d_p1(__func__, "User timer %d is fired\n", i);
                    /* If the timer is one shot, deactivate it */
                    if(s_user_timers[i].one_shot == true)
                    {
                        s_user_timers[i].is_active = false;
                    }
                    /* Reset the timer counter and invoke the callback function */
                    s_user_timers[i].timer_counter = 0;
                    s_user_timers[i].user_timer_hnd.timer_flag = true;
                    if (s_user_timers[i].timer_cb != NULL)
                    {
                        s_user_timers[i].timer_cb();
                    }
                }
            }
        }
    }
}

/*
 * @brief This function initializes the system timer module.
 * It should be called before allocating any user timers.
*/
void sys_timer_init()
{
    /* Check if the main timer is not initialized */
    if(s_main_timer.is_active == false)
    {
        logger_d(__func__, "System timer is initializing...\n");
        /* Initialize the user timers */
        for(uint8_t i = 0; i < TIMERS_COUNT; i++)
        {
            s_user_timers[i].is_active = false;
            s_user_timers[i].one_shot = false;
            s_user_timers[i].user_timer_hnd.timer_flag = false;
            s_user_timers[i].user_timer_hnd.timer_id = NO_TIMER;
            s_user_timers[i].time_unit = TIME_UNIT_MS;
            s_user_timers[i].timer_period = 0U;
            s_user_timers[i].timer_cb = NULL;
            s_user_timers[i].timer_counter = 0U;
        }
        s_timer_semaphore = xSemaphoreCreateBinary();
        /* initialize the main timer and set the timer prescaler equal
         * to (SYSTEM_CPU_FREQ_MHZ)
         */
        s_main_timer.timer_hnd_ptr = timerBegin(0, (SYSTEM_CPU_FREQ_MHZ), true);
        timerAttachInterrupt(s_main_timer.timer_hnd_ptr, &timer_isr, true);
        /* Set the timer period according to the following formula: 
         * timer_period = (100000) to convert the period to 100ms x
         *                      target time in milli seconds x (cpu frequency / prescaler)
         */
        timerAlarmWrite(s_main_timer.timer_hnd_ptr, (500UL * s_main_timer.timer_period * s_main_timer.time_unit), true);
        s_main_timer.is_active = true;
        timerAlarmEnable(s_main_timer.timer_hnd_ptr);

        logger_d(__func__, "System timer is initialized...\n");
    }
    else
    {
        logger_d(__func__, "System timer is already initialized\n");
    }
}

/*
 * @brief This function allocates a user timer, the minimum time period is 100ms.
 * @param p_time_period input: The period of the timer in the selected time unit.
 * @param p_time_unit   input: The time unit of the timer.
 * @param p_timer_cb    input: The callback function of the timer.
 * @param p_one_shot    input: The one shot flag of the timer, if true the timer will be run only once.
 * @return The ID of the allocated timer;
 *         NO_TIMER if no timer is available.
*/
timer_handler_t * timer_allocate(uint32_t p_time_period,
                          time_unit_t p_time_unit,
                          timer_callback_t p_timer_cb,
                          bool p_one_shot)
{
    timer_handler_t * user_timer = NULL;
    for(timer_id_t i = TIMER_ID_0; i < TIMERS_COUNT; i=(timer_id_t)(i+1U))
    {
        if(s_user_timers[i].user_timer_hnd.timer_id == NO_TIMER)
        {
            s_user_timers[i].timer_cb = p_timer_cb;
            s_user_timers[i].timer_period = p_time_period;
            s_user_timers[i].time_unit = p_time_unit;
            s_user_timers[i].one_shot = p_one_shot;
            s_user_timers[i].user_timer_hnd.timer_id = i;
            user_timer = &(s_user_timers[i].user_timer_hnd);
            logger_d_p1(__func__, "User timer %d is allocated\n", i);
            break;
        }
    }
    return user_timer;
}

/*
 * @brief This function starts the user timer.
 * @param p_timer_id input: The ID of the timer.
*/
void timer_activate(timer_id_t p_timer_id)
{
    if (p_timer_id > NO_TIMER && p_timer_id < TIMERS_COUNT)
    {
        s_user_timers[p_timer_id].timer_counter = 0;
        s_user_timers[p_timer_id].is_active = true;
    }
}

/*
 * @brief This function stops the user timer.
 * @param p_timer_id input: The ID of the timer.
*/
void timer_deactivate(timer_id_t p_timer_id)
{
    if (p_timer_id > NO_TIMER && p_timer_id < TIMERS_COUNT)
    {
        s_user_timers[p_timer_id].is_active = false;
        s_user_timers[p_timer_id].timer_counter = 0U;
        s_user_timers[p_timer_id].user_timer_hnd.timer_flag = false;
    }
}

/*
 * @brief This function sets the period of the user timer.
 * @param p_timer_id    input: The ID of the timer.
 * @param p_time_period input: The period of the timer in the selected time unit.
 * @param p_time_unit   input: The time unit of the timer.
*/
void timer_update_period(timer_id_t p_timer_id ,uint32_t p_time_period, time_unit_t p_time_unit)
{
    if (p_timer_id > NO_TIMER && p_timer_id < TIMERS_COUNT)
    {
        s_user_timers[p_timer_id].timer_counter = 0;
        s_user_timers[p_timer_id].timer_period = p_time_period;
        s_user_timers[p_timer_id].time_unit = p_time_unit;
    }
}

void timer_update_oneshot(timer_id_t p_timer_id, bool p_one_shot)
{
    if (p_timer_id > NO_TIMER && p_timer_id < TIMERS_COUNT)
    {
        s_user_timers[p_timer_id].one_shot = p_one_shot;
        s_user_timers[p_timer_id].timer_counter = 0;
    }
}

/*
 * @brief This function deletes the user timer.
 * @param p_timer_id input: The ID of the timer.
*/
void timer_clear(timer_id_t p_timer_id)
{
    if(p_timer_id > NO_TIMER && p_timer_id < TIMERS_COUNT)
    {
        s_user_timers[p_timer_id].timer_cb = NULL;
        s_user_timers[p_timer_id].timer_period = 0U;
        s_user_timers[p_timer_id].timer_counter = 0U;
        s_user_timers[p_timer_id].time_unit = TIME_UNIT_MS;
        s_user_timers[p_timer_id].is_active = false;
        s_user_timers[p_timer_id].one_shot = false;
        s_user_timers[p_timer_id].user_timer_hnd.timer_flag = false;
        s_user_timers[p_timer_id].user_timer_hnd.timer_id = NO_TIMER;
        
    }
}

void hard_delay(uint16_t p_delay_time_ms)
{
    if (p_delay_time_ms < 1000)
    {
        delay(p_delay_time_ms);
    }
}