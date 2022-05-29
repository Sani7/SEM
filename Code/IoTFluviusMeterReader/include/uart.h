#pragma once

void uart_init(void);
int sendData(const char *logName, const char *data);
bool read_datagram(char *data, int len);
double ParseDataValue(char* datagramBuffer, char *key, int datablock);
bool decode_datagram(char* datagramBuffer, fluviusData* datagram);