/***************************************************************************************************
* File Name: HW_eeprom.c
* Module: HW_eeprom
* Abstract: Implementation of "/lib/HW_eeprom/HW_eeprom.h" module.
* Author: Naim ALMASRI
* Date: 15.06.2024
***************************************************************************************************/

/***************************************************************************************************
* Header files.
***************************************************************************************************/

#include <Arduino.h>
#include "HW_eeprom.h"
#include "EEPROM.h"
#include "debug_logger.h"

/***************************************************************************************************
* Macro definitions.
***************************************************************************************************/

#define MAGIC_NUMBER_VAL ((uint16_t)0xdeadU)
#define MAGIC_NUMBER_ADRS (0U)
#define MAGIC_NUMBER_SIZE (2U)
#define CHECKSUM_ADRS (MAGIC_NUMBER_ADRS + MAGIC_NUMBER_SIZE + 1U)
#define CHECKSUM_SIZE (2U)
#define DATA_START_ADRS (CHECKSUM_SIZE + CHECKSUM_ADRS + 1U)

/***************************************************************************************************
* Local type definitions.
***************************************************************************************************/

/***************************************************************************************************
 * Local data definitions.
***************************************************************************************************/

static bool is_eeprom_init = false;

/***************************************************************************************************
* Local function definitions.
***************************************************************************************************/

static uint16_t calculate_checksum(const uint8_t * p_ptr_data_buffer, uint32_t p_data_size)
{
    uint32_t sum = 0;

    // Sum all the bytes
    for (size_t i = 0; i < p_data_size; i++)
    {
        sum += p_ptr_data_buffer[i];
    }

    // Add the carry bits to the sum
    while (sum >> 16)
    {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    // Return the one's complement of the sum
    return (uint16_t)(~sum);
}

static uint16_t read_magic_number(void)
{
    uint16_t magic_no = 0U;
    magic_no = EEPROM.readUShort(MAGIC_NUMBER_ADRS);
    return magic_no;
}

static bool write_magic_number(void)
{
    EEPROM.writeUShort(MAGIC_NUMBER_ADRS, MAGIC_NUMBER_VAL);
    return EEPROM.commit();
}

static bool is_magic_number_valid(void)
{
    return (MAGIC_NUMBER_VAL == read_magic_number());
}

static bool write_checksum(uint16_t p_checksum)
{
    EEPROM.writeUShort(CHECKSUM_ADRS, p_checksum);
    return EEPROM.commit();
}

static uint16_t read_checksum(void)
{
    uint16_t checksum = 0U;
    checksum = EEPROM.readUShort(CHECKSUM_ADRS);
    return checksum;
}

/***************************************************************************************************
* External data definitions.
***************************************************************************************************/

/***************************************************************************************************
* External function definitions.
***************************************************************************************************/

/*
 * @brief
 * @param p_data_size
*/
bool eeprom_init(uint32_t p_data_size)
{
    bool ret_val = false;
    if (false == is_eeprom_init)
    {
        ret_val = EEPROM.begin(p_data_size + DATA_START_ADRS);
        is_eeprom_init = ret_val;
    }
    else
    {
        ret_val = true;
    }
    return ret_val;
}

void eeprom_deinit(void)
{
    EEPROM.end();
}

data_validity_t eeprom_read_data(uint8_t * p_ptr_data_buffer, 
                                 uint32_t p_data_size,
                                 uint32_t p_starting_address)
{
    data_validity_t data_vld = DATA_INVALID;
    uint8_t buffer_ptr[p_data_size];
    if(NULL != p_ptr_data_buffer && 0U != p_data_size && p_starting_address >= DATA_START_ADRS)
    {
        logger_d(__func__, "EEPROM read data\n");
        if(true == is_magic_number_valid())
        {
            logger_d(__func__, "Magic number is valid\n");
            EEPROM.readBytes(p_starting_address, buffer_ptr, p_data_size);
            uint16_t checksum = calculate_checksum((const uint8_t*)buffer_ptr, p_data_size);
            uint16_t r_checksum = read_checksum();
            if(checksum == r_checksum)
            {
                logger_d(__func__, "Checksum is valid\n");
                memcpy(p_ptr_data_buffer, (const uint8_t*)buffer_ptr, p_data_size);
                data_vld = DATA_VALID;
            }
            else
            {
                logger_d(__func__, "Checksum is invalid\n");
            }
        }
        else
        {
            
            logger_d(__func__, "Magic number is invalid\tNo data to read\n");
        }
    }
    else
    {
        logger_d(__func__, "Invalid input parameters\n");
    }
    return data_vld;
}

bool eeprom_write_data(uint8_t * p_ptr_data_buffer, 
                                  uint32_t p_data_size,
                                  uint32_t p_starting_address)
{
    bool data_vld = false;
    bool write_status = true;
    uint16_t checksum = 0;
    uint16_t r_checksum = 0;
    if(NULL != p_ptr_data_buffer && 0U != p_data_size && p_starting_address >= DATA_START_ADRS)
    {
        if(false == is_magic_number_valid())
        {
            logger_d(__func__, "No magic number\n");
            write_status = write_magic_number();
        }
        else
        {
            logger_d(__func__, "Magic number is exist\n");
        }
        if(true == write_status)
        {
            logger_d(__func__, "Magic number is written\n");
            checksum = calculate_checksum(p_ptr_data_buffer, p_data_size);
            write_status = write_checksum(checksum);
            if(true == write_status)
            {
                r_checksum = read_checksum();
                if(r_checksum == checksum)
                {
                    logger_d(__func__, "Checksum is written\n");
                    EEPROM.writeBytes(p_starting_address, p_ptr_data_buffer, p_data_size);
                    data_vld = EEPROM.commit();
                }
                else
                {
                    logger_d_p2(__func__, "Checksum is invalid\tExpected: %d - Received: %d\n", checksum, r_checksum);
                }
            }
        }
    }
    else
    {
        logger_d(__func__, "Invalid input parameters\n");
    }

    return data_vld;
}

uint32_t eeprom_get_data_start_adrs(void)
{
    return DATA_START_ADRS;
}
