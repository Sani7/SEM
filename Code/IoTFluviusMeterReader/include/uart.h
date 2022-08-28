#pragma once

void uart_init(gpio_num_t tx_pin, gpio_num_t rx_pin, uint32_t baud, bool inv);
int sendData(const char *logName, const char *data);
bool read_datagram(char *data, int len);
double ParseDataValue(char* datagramBuffer, char *key, int datablock);
bool decode_datagram(char* datagramBuffer, fluviusData* datagram);