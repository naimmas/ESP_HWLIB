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

bool web_server_init(web_server_cb_t p_get_cb);
void web_server_start();
void web_server_stop();
bool web_server_request_has_param(AsyncWebServerRequest *p_ptr_request, const char *p_ptr_param_name);
int web_server_request_get_param(AsyncWebServerRequest *p_ptr_request, const char *p_ptr_param_name);

#endif /* WEB_SERVER_UTIL_H */
