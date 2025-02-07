/***************************************************************************************************
* File Name: web_server_util.c
* Module: web_server_util
* Abstract: Implementation of "/lib/web_server_util/web_server_util.h" module.
* Author: Naim ALMASRI
* Date: 23.06.2024
***************************************************************************************************/

/***************************************************************************************************
* Header files.
***************************************************************************************************/

#include "web_server_util.h"
#include "Arduino.h"
#include <DNSServer.h>
#include <WiFi.h>
#include "AsyncTCP.h"
#include "SPIFFS.h"

/***************************************************************************************************
* Macro definitions.
***************************************************************************************************/

/***************************************************************************************************
* Local type definitions.
***************************************************************************************************/

/***************************************************************************************************
 * Local data definitions.
***************************************************************************************************/

static AsyncWebServer s_async_web_server(80);

static const String s_html_error_page = PROGMEM(
"<center>"
    "<h3 style=color:red;font-weight:bold;>"
        "An error has occurred while getting or saving the parameters."
        "<br>"
        "<a href=\"/\">HOME</a>"
    "</h3>"
"</center>");

/***************************************************************************************************
* Local function definitions.
***************************************************************************************************/

/***************************************************************************************************
* External data definitions.
***************************************************************************************************/

/***************************************************************************************************
* External function definitions.
***************************************************************************************************/

bool web_server_init(web_server_cb_t p_get_cb)
{
    bool ret_val = false;

    if (!SPIFFS.begin())
    {
        logger_e("An Error has occurred while mounting SPIFFS\n");
    }
    else
    {
        s_async_web_server.on
        (
            "/", HTTP_GET, 
            [](AsyncWebServerRequest *request)
            {
                request->send(SPIFFS, "/index.html", "text/html", false);
                logger_d(NULL, "Client connected\n");
            }
        );
        s_async_web_server.on
        (
            "/style.css", HTTP_GET, 
            [](AsyncWebServerRequest *request)
            { 
                request->send(SPIFFS, "/style.css", "text/css"); 
            }
        );
        s_async_web_server.on
        (
            "/civan.png", HTTP_GET,
            [](AsyncWebServerRequest *request)
            {
                request->send(SPIFFS, "/civan.png", "image/png");
            }
        );
        s_async_web_server.on
        (
            "/get", HTTP_GET,
            [p_get_cb](AsyncWebServerRequest *request)
            {
                if (p_get_cb(request) == true)
                {
                    request->send(200, "text/plain", "true");
                    web_server_stop();
                }
                else
                {
                    request->send(200, "text/html", s_html_error_page);
                }
            }
        );
        ret_val = true;
    }
    return ret_val;
}

void web_server_start()
{
    s_async_web_server.begin();
}

void web_server_stop()
{
    s_async_web_server.end();
    SPIFFS.end();
}

bool web_server_request_has_param(AsyncWebServerRequest *p_ptr_request, const char *p_ptr_param_name)
{
    return p_ptr_request->hasParam(p_ptr_param_name);
}

int web_server_request_get_param(AsyncWebServerRequest *p_ptr_request, const char *p_ptr_param_name)
{
    return p_ptr_request->getParam(p_ptr_param_name)->value().toInt();
}