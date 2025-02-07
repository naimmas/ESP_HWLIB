/***************************************************************************************************
* File Name: debug_logger.h
* Module: debug_logger
* Abstract: Interface definition for "/lib/debug_logger/debug_logger.c" module.
* Author: Naim ALMASRI
* Date: 13.06.2024
***************************************************************************************************/

#ifndef DEBUG_LOGGER_H
#define DEBUG_LOGGER_H

/***************************************************************************************************
* Header files.
***************************************************************************************************/

#include "HW_comm.h"
#include "stddef.h"

/***************************************************************************************************
* Macro definitions.
***************************************************************************************************/

#define MAX_DBG_MSG_LEN 512

#ifdef DEBUG_EN
#define logger_i(p_msg) logger(DBG_LVL_INFO, NULL, (p_msg), 0, 0, 0)
#define logger_w(p_msg) logger(DBG_LVL_WARN, NULL, (p_msg), 0, 0, 0)
#define logger_e(p_msg) logger(DBG_LVL_ERR, NULL, (p_msg), 0, 0, 0)
#define logger_d(p_msg) logger(DBG_LVL_DEBUG, __func__, (p_msg), 0, 0, 0)
#define logger_p(p_msg) logger(DBG_LVL_PERIODIC, NULL, (p_msg), 0, 0, 0)
#define logger_ext(p_msg) logger(DBG_LVL_EXT, NULL, (p_msg), 0, 0, 0)

#define logger_i_p1(p_msg, p_param_1) logger(DBG_LVL_INFO, NULL, (p_msg), (p_param_1), 0U, 0U)
#define logger_w_p1(p_msg, p_param_1) logger(DBG_LVL_WARN, NULL, (p_msg), (p_param_1), 0U, 0U)
#define logger_e_p1(p_msg, p_param_1) logger(DBG_LVL_ERR, NULL, (p_msg), (p_param_1), 0U, 0U)
#define logger_d_p1(p_msg, p_param_1) logger(DBG_LVL_DEBUG, __func__, (p_msg), (p_param_1), 0U, 0U)
#define logger_p_p1(p_msg, p_param_1) logger(DBG_LVL_PERIODIC, NULL, (p_msg), (p_param_1), 0U, 0U)
#define logger_ext_p1(p_msg, p_param_1) logger(DBG_LVL_EXT, NULL, (p_msg), (p_param_1), 0U, 0U)

#define logger_i_p2(p_msg, p_param_1, p_param_2) logger(DBG_LVL_INFO, NULL, (p_msg), (p_param_1), (p_param_2), 0U)
#define logger_w_p2(p_msg, p_param_1, p_param_2) logger(DBG_LVL_WARN, NULL, (p_msg), (p_param_1), (p_param_2), 0U)
#define logger_e_p2(p_msg, p_param_1, p_param_2) logger(DBG_LVL_ERR, NULL, (p_msg), (p_param_1), (p_param_2), 0U)
#define logger_d_p2(p_msg, p_param_1, p_param_2) logger(DBG_LVL_DEBUG, __func__, (p_msg), (p_param_1), (p_param_2), 0U)
#define logger_p_p2(p_msg, p_param_1, p_param_2) logger(DBG_LVL_PERIODIC, NULL, (p_msg), (p_param_1), (p_param_2), 0U)
#define logger_ext_p2(p_msg, p_param_1, p_param_2) logger(DBG_LVL_EXT, NULL, (p_msg), (p_param_1), (p_param_2), 0U)

#define logger_i_p3(p_msg, p_param_1, p_param_2, p_param_3) logger(DBG_LVL_INFO, NULL, (p_msg), (p_param_1), (p_param_2), p_param_3)
#define logger_w_p3(p_msg, p_param_1, p_param_2, p_param_3) logger(DBG_LVL_WARN, NULL, (p_msg), (p_param_1), (p_param_2), p_param_3)
#define logger_e_p3(p_msg, p_param_1, p_param_2, p_param_3) logger(DBG_LVL_ERR, NULL, (p_msg), (p_param_1), (p_param_2), p_param_3)
#define logger_d_p3(p_msg, p_param_1, p_param_2, p_param_3) logger(DBG_LVL_DEBUG, __func__, (p_msg), (p_param_1), (p_param_2), p_param_3)
#define logger_p_p3(p_msg, p_param_1, p_param_2, p_param_3) logger(DBG_LVL_PERIODIC, NULL, (p_msg), (p_param_1), (p_param_2), p_param_3)
#define logger_ext_p3(p_msg, p_param_1, p_param_2, p_param_3) logger(DBG_LVL_EXT, NULL, (p_msg), (p_param_1), (p_param_2), p_param_3)

#else
#define logger_i(p_msg) ((void)0)
#define logger_w(p_msg) ((void)0)
#define logger_e(p_msg) ((void)0)
#define logger_d(p_msg) ((void)0)
#define logger_p(p_msg) ((void)0)
#define logger_ext(p_msg) ((void)0)

#define logger_i_p1(p_msg, p_param_1) ((void)0)
#define logger_w_p1(p_msg, p_param_1) ((void)0)
#define logger_e_p1(p_msg, p_param_1) ((void)0)
#define logger_d_p1(p_msg, p_param_1) ((void)0)
#define logger_p_p1(p_msg, p_param_1) ((void)0)
#define logger_ext_p1(p_msg, p_param_1) ((void)0)

#define logger_i_p2(p_msg, p_param_1, p_param_2) ((void)0)
#define logger_w_p2(p_msg, p_param_1, p_param_2) ((void)0)
#define logger_e_p2(p_msg, p_param_1, p_param_2) ((void)0)
#define logger_d_p2(p_msg, p_param_1, p_param_2) ((void)0)
#define logger_p_p2(p_msg, p_param_1, p_param_2) ((void)0)
#define logger_ext_p2(p_msg, p_param_1, p_param_2) ((void)0)

#define logger_i_p3(p_msg, p_param_1, p_param_2, p_param_3) ((void)0)
#define logger_w_p3(p_msg, p_param_1, p_param_2, p_param_3) ((void)0)
#define logger_e_p3(p_msg, p_param_1, p_param_2, p_param_3) ((void)0)
#define logger_d_p3(p_msg, p_param_1, p_param_2, p_param_3) ((void)0)
#define logger_p_p3(p_msg, p_param_1, p_param_2, p_param_3) ((void)0)
#define logger_ext_p3(p_msg, p_param_1, p_param_2, p_param_3) ((void)0)

#endif /* DEBUG_EN */

/***************************************************************************************************
* External type declarations.
***************************************************************************************************/

typedef enum debug_level_t_enum
{
    DBG_LVL_EXT = 0,
    DBG_LVL_ERR,
    DBG_LVL_WARN,
    DBG_LVL_INFO,
    DBG_LVL_PERIODIC,
    DBG_LVL_DEBUG,
} debug_level_t;

/***************************************************************************************************
* External data declarations.
***************************************************************************************************/

/***************************************************************************************************
* External function declarations.
***************************************************************************************************/

/**
 * @brief This function initializes the debug messages print interfaces.
 * 
 */
void debug_agents_init();

/**
 * @brief This function sets the debug level threshold.
 *        any message with a level less than the threshold will not be printed.
 * @param p_lvl input: The debug level threshold.
 */
void debug_agents_set_threshold(debug_level_t p_lvl);

/**
 * @brief This function prints the log message with the specified log level.
 * 
 * @param p_lvl 
 * @param p_ptr_func_name input: Log level
 * @param p_ptr_msg       input: Log message
 * @param p_param_1       input: if format contains %d, %f or %x the value of the first parameter. 0 otherwise
 * @param p_param_2       input: if format contains %d, %f or %x the value of the second parameter. 0 otherwise
 * @param p_param_3       input: if format contains %d, %f or %x the value of the third parameter. 0 otherwise
 */
void logger(debug_level_t p_lvl,
            const char * p_ptr_func_name, 
            const char * p_ptr_msg,
            float p_param_1, 
            float p_param_2,
            float p_param_3);

#endif /* DEBUG_LOGGER_H */
