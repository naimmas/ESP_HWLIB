/***************************************************************************************************
* File Name: string_util.h
* Module: string_util
* Abstract: Interface definition for "/lib/string_util/string_util.c" module.
* Author: Naim ALMASRI
* Date: 15.06.2024
***************************************************************************************************/

#ifndef STRING_UTIL_H
#define STRING_UTIL_H

/***************************************************************************************************
* Header files.
***************************************************************************************************/

/***************************************************************************************************
* Macro definitions.
***************************************************************************************************/

/***************************************************************************************************
* External type declarations.
***************************************************************************************************/
typedef enum base_t_enum
{
    BASE_10 = 10,
    BASE_16 = 16,
} base_t;
/***************************************************************************************************
* External data declarations.
***************************************************************************************************/

/***************************************************************************************************
* External function declarations.
***************************************************************************************************/

/**
 * @brief This function reverses the input string.
 * 
 * @param p_ptr_str input: pointer to the input string.
 * @param p_str_len input: length of the input string.
 */
void string_reverse(char * p_ptr_str, uint32_t p_str_len);

/**
 * @brief This function converts an integer number to a string.
 * 
 * @param p_num input: The number to be converted.
 * @param p_ptr_str output: The string that will hold the converted number.
 * @param p_digit input: The number of digits in the converted number.
 * @param p_base input: The base of the converted number, 10 or 16.
 * @return The length of the converted string.
 */
uint32_t string_itoa(int32_t p_num, char * p_ptr_str, uint32_t p_digit, base_t p_base);

/**
 * @brief This function converts a float number to a string.
 * 
 * @param p_fnum input: the float number to be converted.
 * @param p_ptr_str output: the string that will hold the converted number.
 * @param p_after_point input: the number of digits after the point.
 * @retval The length of the converted string.
 */
uint32_t string_ftoa(float p_fnum, char * p_ptr_str, uint32_t p_after_point);

#endif /* STRING_UTIL_H */
