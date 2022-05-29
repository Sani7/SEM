#pragma once

typedef struct http_server_info
{
    char ip[16];
    int port;
    char uri[20];
    char api_key[20];
} http_server_info;

typedef struct fluviusData
{
    double UL1;
    double UL2;
    double UL3;
    double IL1;
    double IL2;
    double IL3;
    double CONSUMPTION_LOW_TARIF;
    double CONSUMPTION_HIGH_TARIF;
    double PRODUCTION_LOW_TARIF;
    double PRODUCTION_HIGH_TARIF;
    double TOTAL_POWER_CONSUMPTION;
    double TOTAL_POWER_PRODUCTION;
    double GAS_METER_M3;
    double WATER_METER_M3;
} fluviusData;

esp_err_t _http_event_handler(esp_http_client_event_t *evt);
void HTTP_Post(http_server_info info, char* data);