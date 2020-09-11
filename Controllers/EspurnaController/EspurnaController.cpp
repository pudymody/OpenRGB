/*---------------------------------------------------------*\
|  Processing Code for Espurna Interface                    |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 9/11/2020        |
\*---------------------------------------------------------*/

#include "EspurnaController.h"

#include <fstream>
#include <iostream>
#include <string>

EspurnaController::EspurnaController()
{

}

EspurnaController::~EspurnaController()
{
}

void EspurnaController::Initialize(char* ledstring)
{
    LPSTR   apikey = NULL;
    LPSTR   numleds = NULL;
    LPSTR   source = NULL;
    LPSTR   udpport_baud = NULL;
    LPSTR   next = NULL;
    
    //Assume serial device unless a different protocol is specified
    bool    serial = TRUE;
    
    source = strtok_s(ledstring, ",", &next);
    
    //Check for either the UDP port or the serial baud rate
    if (strlen(next))
    {
        udpport_baud = strtok_s(next, ",", &next);
    }

    //Espurna protocol requires API key
    if (strlen(next))
    {
        apikey = strtok_s(next, ",", &next);
    }

    InitializeEspurna(source, udpport_baud, apikey);
}

void EspurnaController::InitializeEspurna(char * clientname, char * port, char * apikey)
{
    strcpy(client_name, clientname);
    strcpy(port_name, port);
    strcpy(espurna_apikey, apikey);
    tcpport = new net_port;
    tcpport->tcp_client(client_name, port_name);
}

void EspurnaController::SetLEDs(std::vector<RGBColor> colors)
{
    if (tcpport != NULL)
    {
        RGBColor color = colors[0];

        char get_request[1024];
        snprintf(get_request, 1024, "GET /api/rgb?apikey=%s&value=%%23%02X%02X%02X HTTP/1.1\r\nHost: %s\r\n\r\n", espurna_apikey, RGBGetRValue(color), RGBGetGValue(color), RGBGetBValue(color), client_name);
        tcpport->tcp_client_connect();
        tcpport->tcp_client_write(get_request, strlen(get_request));
        tcpport->tcp_close();
    }
}
