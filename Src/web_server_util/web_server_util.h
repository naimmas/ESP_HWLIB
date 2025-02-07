/***************************************************************************************************
* File Name: web_server_util.h
* Module: web_server_util
* Abstract: Interface definition for "/lib/web_server_util/web_server_util.c" module.
* Author: Naim ALMASRI
* Date: 23.06.2024
***************************************************************************************************/

#ifndef WEB_SERVER_UTIL_H
#define WEB_SERVER_UTIL_H

/***************************************************************************************************
* Header files.
***************************************************************************************************/

#include "debug_logger.h"
#include "ESPAsyncWebServer.h"

/***************************************************************************************************
* Macro definitions.
***************************************************************************************************/

/***************************************************************************************************
* External type declarations.
***************************************************************************************************/

typedef bool (*web_server_cb_t)(AsyncWebServerRequest * p_ptr_request);

/***************************************************************************************************
* External data declarations.
***************************************************************************************************/

/***************************************************************************************************
* External function declarations.
***************************************************************************************************/

/**
 * @brief This function initializes async web server on port 80 and SPIFFS module.
 * 
 * @param p_get_cb 
 * @retval true if the initialization is successful.
 * @retval false if the initialization is failed.
 */
bool web_server_init(web_server_cb_t p_get_cb);

/**
 * @brief This function starts the web server.
 */
void web_server_start();

/**
 * @brief This function stops the web server and SPIFFS module.
 */
void web_server_stop();

/**
 * @brief This function checks if the received request has a specific parameter.
 * 
 * @param p_ptr_request pointer to the request struct.
 * @param p_ptr_param_name pointer to the parameter name.
 * @retval true if the parameter is found.
 * @retval false if the parameter is not found.
 */
bool web_server_request_has_param(AsyncWebServerRequest *p_ptr_request, const char *p_ptr_param_name);

/**
 * @brief This function gets the numeric value of a specific parameter from the received request.
 * 
 * @param p_ptr_request pointer to the request struct.
 * @param p_ptr_param_name pointer to the parameter name.
 * @retval the numeric value of the parameter.
 */
int web_server_request_get_param(AsyncWebServerRequest *p_ptr_request, const char *p_ptr_param_name);

#endif /* WEB_SERVER_UTIL_H */
