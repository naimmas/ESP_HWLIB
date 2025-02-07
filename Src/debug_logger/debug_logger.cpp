/***************************************************************************************************
 * File Name: debug_logger.c
 * Module: debug_logger
 * Abstract: Implementation of "/lib/debug_logger/debug_logger.h" module.
 * Author: Naim ALMASRI
 * Date: 13.06.2024
  ***************************************************************************************************/

/***************************************************************************************************
 * Header files.
 ***************************************************************************************************/

#include "debug_logger.h"
#include "string_util.h"
#include <Arduino.h>

/***************************************************************************************************
 * Macro definitions.
 ***************************************************************************************************/

#define DBG_LOG_COLOR_E "\033[0;31m[ERROR]\033[0m\t" // ERROR    RED
#define DBG_LOG_COLOR_W "\033[0;33m[ WARN]\033[0m\t" // INFO     GREEN
#define DBG_LOG_COLOR_I "\033[0;32m[ INFO]\033[0m\t" // WARNING  YELLOW
#define DBG_LOG_COLOR_D "\033[0;36m[DEBUG]\033[0m\t" // DEBUG    CYAN
#define DBG_LOG_COLOR_P "\033[0;35m[PRDIC]\033[0m\t" // PERIODIC Mageenta

#define DBG_LOG_FUNC "\033[1m[FUNC: "
#define DBG_LOG_RESET "]\033[0m ->"

#if BLT_DEBUG
#define BLT_SERIAL_NAME "BLT_DBG"
#define BLT_SERIAL Serial_BT
#endif
#if UART_DEBUG
#define UART_SERIAL Serial
#endif

/***************************************************************************************************
 * Local type definitions.
 ***************************************************************************************************/

/***************************************************************************************************
 * Local data definitions.
 ***************************************************************************************************/

#if BLT_DEBUG
#include "BluetoothSerial.h"
BluetoothSerial Serial_BT;
#endif

static char g_debug_msg[MAX_DBG_MSG_LEN] = {'\0'};
static debug_level_t g_debug_thld = DBG_LVL_DEBUG;

/***************************************************************************************************
 * Local function definitions.
 ***************************************************************************************************/

static uint16_t check_param_qualifier(const char * p_ptr_msg)
{
    const char qualifiers[] = "fdx";
    uint16_t msg_len = 0;
    if(p_ptr_msg[0] == '%')
    {
        for(uint8_t i = 1; ((p_ptr_msg[i] != '\0' && msg_len == 0) && p_ptr_msg[i] != ' '); i++)
        {
            for(uint8_t j = 0; (qualifiers[j] != '\0' && msg_len == 0); j++)
            {
                if(p_ptr_msg[i] == qualifiers[j])
                {
                    msg_len = i;
                }
            }
        }
    }
    return msg_len;
}

static uint16_t add_function_name(const char * p_ptr_func_name,
                                  char * p_ptr_msg)
{
    uint16_t bytes_written = 0;
    uint16_t func_name_len = strlen(p_ptr_func_name);
    memcpy(&p_ptr_msg[bytes_written], DBG_LOG_FUNC, sizeof(DBG_LOG_FUNC) - 1);
    bytes_written = sizeof(DBG_LOG_FUNC) - 1;

    for(uint16_t i = 0; i < func_name_len; i++)
    {
        p_ptr_msg[bytes_written++] = p_ptr_func_name[i];
    }
    memcpy(&p_ptr_msg[bytes_written], DBG_LOG_RESET, sizeof(DBG_LOG_RESET) - 1);
    bytes_written += sizeof(DBG_LOG_RESET) - 1;
    return bytes_written;
}

/***************************************************************************************************
 * External data definitions.
 ***************************************************************************************************/

/***************************************************************************************************
 * External function definitions.
 ***************************************************************************************************/

void debug_agents_init()
{
#ifdef DEBUG_EN 
#if UART_DEBUG
    UART_SERIAL.begin(256000);
    delay(100);
    logger_d("UART is ready\n");
#endif /* UART_DEBUG */
#if BLT_DEBUG
    BLT_SERIAL.begin(BLT_SERIAL_NAME);
    delay(1000);
    BLT_SERIAL.println("Bluetooth is ready");
#endif /* BLT_DEBUG */
delay(100);
#endif /* DEBUG_EN */
}

void debug_agents_set_threshold(debug_level_t p_lvl)
{
    if (p_lvl != DBG_LVL_EXT)
    {
        g_debug_thld = p_lvl;
    }
}

void logger(debug_level_t p_lvl,
            const char * p_ptr_func_name, 
            const char * p_ptr_msg,
            float p_param_1, 
            float p_param_2,
            float p_param_3)
{
    uint16_t bytes_written = 0U;
    uint8_t param_count = 0U;
    float current_param = 0.0;
    float const p_params_list[] = {p_param_1, p_param_2, p_param_3};
    uint16_t qualifier_idx = 0;
    const char * log_strings[] = {
        "",
        DBG_LOG_COLOR_E,
        DBG_LOG_COLOR_W,
        DBG_LOG_COLOR_I,
        DBG_LOG_COLOR_P,
        DBG_LOG_COLOR_D
    };

    if(p_lvl <= g_debug_thld)
    {
        memset(g_debug_msg, '\0', MAX_DBG_MSG_LEN);
        if(p_ptr_msg[0] == '\n' && p_ptr_msg[1] == '\0')
        {
            g_debug_msg[bytes_written++] = '\n';
        }
        else
        {
            if (p_ptr_msg[0] == '\n')
            {
                g_debug_msg[bytes_written++] = '\n';
            }
            
            strcpy(&g_debug_msg[bytes_written], log_strings[p_lvl]);
            bytes_written = strlen(log_strings[p_lvl]);

            if(p_ptr_func_name != NULL)
            {
                bytes_written += add_function_name(p_ptr_func_name, &g_debug_msg[bytes_written]);
            }
            for (uint16_t i = 0; (i < MAX_DBG_MSG_LEN) && (p_ptr_msg[i] != '\0'); i++)
            {
                if (p_ptr_msg[i] == '%' && param_count < 3)
                {
                    current_param = p_params_list[param_count++];
                    qualifier_idx = check_param_qualifier(&p_ptr_msg[i]);
                    if (qualifier_idx > 0)
                    {
                        switch (p_ptr_msg[i + qualifier_idx])
                        {
                        case 'f':
                            qualifier_idx = string_ftoa(current_param, &g_debug_msg[bytes_written], 3);
                            break;
                        case 'd':
                            qualifier_idx = string_itoa(current_param, &g_debug_msg[bytes_written], 0, 10);
                            break;
                        case 'x':
                            qualifier_idx = string_itoa(current_param, &g_debug_msg[bytes_written], 0, 16);
                            break;
                        }
                        bytes_written += qualifier_idx;
                        i++;
                    }
                }
                else
                {
                    g_debug_msg[bytes_written++] = p_ptr_msg[i];
                }
            }
        }

#if UART_DEBUG
    UART_SERIAL.print(g_debug_msg);
#endif /* UART_DEBUG */
#if BLT_DEBUG
    BLT_SERIAL.print(g_debug_msg);
#endif /* BLT_DEBUG */
    }
}