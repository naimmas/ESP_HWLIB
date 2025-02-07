/***************************************************************************************************
* File Name: string_util.c
* Module: string_util
* Abstract: Implementation of "/lib/string_util/string_util.h" module.
* Author: Naim ALMASRI
* Date: 15.06.2024
***************************************************************************************************/

/***************************************************************************************************
* Header files.
***************************************************************************************************/

#include "Arduino.h"
#include "string_util.h"

/***************************************************************************************************
* Macro definitions.
***************************************************************************************************/

#define MAX_STRING_LEN (1000U)

/***************************************************************************************************
* Local type definitions.
***************************************************************************************************/

/***************************************************************************************************
 * Local data definitions.
***************************************************************************************************/

/***************************************************************************************************
* Local function definitions.
***************************************************************************************************/

/***************************************************************************************************
* External data definitions.
***************************************************************************************************/

/***************************************************************************************************
* External function definitions.
***************************************************************************************************/

void string_reverse(char * p_ptr_str, uint32_t p_str_len)
{
    uint32_t start = 0;
    uint32_t end = p_str_len - 1;
    char temp = (char)0;

    if(p_ptr_str != NULL && p_str_len <= MAX_STRING_LEN)
    {
        while (start < end)
        {
            temp = p_ptr_str[start];
            p_ptr_str[start] = p_ptr_str[end];
            p_ptr_str[end] = temp;
            start++;
            end--;
        }
    }
}

uint32_t string_itoa(int32_t p_num, char * p_ptr_str, uint32_t p_digit, base_t p_base)
{
    uint32_t i = 0;
    bool is_negative = false;

    if (p_ptr_str != NULL)
    {
        if (p_num == 0)
        {
            p_ptr_str[i++] = '0';
            p_ptr_str[i] = '\0';
        }
        else
        {
            if (p_num < 0 && p_base == BASE_10)
            {
                is_negative = true;
                p_num = -p_num;
            }
            uint32_t num_pos = (uint32_t)p_num;
            while (num_pos != 0)
            {
                uint32_t rem = num_pos % p_base;
                p_ptr_str[i] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
                i++;
                num_pos = num_pos / p_base;
            }

            while (i < p_digit)
            {
                p_ptr_str[i] = '0';
                i++;
            }

            if (is_negative)
            {
                p_ptr_str[i] = '-';
                i++;
            }

            string_reverse(p_ptr_str, i);

            p_ptr_str[i] = '\0';
        }
    }
    else
    {
        /* Do nothing */
    }

    return i;
}

uint32_t string_ftoa(float p_fnum, char * p_ptr_str, uint32_t p_after_point)
{
    int32_t i_part = 0;
    uint32_t i = 0;
    float f_part = 0.0f;
    
    if(p_ptr_str != NULL)
    {
        i_part = (int32_t)p_fnum;
        i = string_itoa(i_part, p_ptr_str, 0, 10);
        p_ptr_str[i++] = '.';
        f_part = p_fnum - (float)i_part;
        if (f_part < 0)
        {
            f_part = -f_part;
        }
        for (uint32_t j = 0; j < p_after_point; j++)
        {
            f_part *= 10;
            i_part = (int32_t)f_part;
            p_ptr_str[i++] = (char)(i_part + '0');
            f_part -= (float)i_part;
        }
        p_ptr_str[i] = '\0';
    }
    else
    {
        /* Do nothing */
    }
    return i;
}