/***************************************************************************************************
* File Name: HW_eeprom.h
* Module: HW_eeprom
* Abstract: Interface definition for "/lib/HW_eeprom/HW_eeprom.c" module.
* Author: Naim ALMASRI
* Date: 15.06.2024
***************************************************************************************************/

#ifndef HW_EEPROM_H
#define HW_EEPROM_H

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

typedef enum data_validity_t_enum
{
    DATA_VALID,
    DATA_INVALID
}data_validity_t;

/***************************************************************************************************
* External data declarations.
***************************************************************************************************/

/***************************************************************************************************
* External function declarations.
***************************************************************************************************/

/**
 * @brief This function initializes the EEPROM module.
 * 
 * @param p_data_size input: The size of the data to be stored in the EEPROM in bytes
 * @retval true if the EEPROM is initialized successfully
 * @retval false if the EEPROM is not initialized successfully
 */
extern bool ardal_eeprom_init(uint32_t p_data_size);

/**
 * @brief This function deinitializes the EEPROM module.
 */
extern data_validity_t ardal_eeprom_read_data(uint8_t * p_ptr_data_buffer, 
                                        uint32_t p_data_size,
                                        uint32_t p_starting_address);

/**
 * @brief This function writes data to the EEPROM, and calculates the checksum.
 * 
 * @param p_ptr_data_buffer input: Pointer to the data buffer
 * @param p_data_size input: The size of the data to be stored in the EEPROM in bytes
 * @param p_starting_address input: The starting address of the data in the EEPROM
 * @return true 
 * @return false 
 */
extern bool ardal_eeprom_write_data(uint8_t * p_ptr_data_buffer, 
                              uint32_t p_data_size,
                              uint32_t p_starting_address);
extern uint32_t ardal_eeprom_get_data_start_adrs(void);

#endif /* HW_EEPROM_H */