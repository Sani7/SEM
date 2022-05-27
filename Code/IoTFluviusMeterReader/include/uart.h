#pragma once

void init(bool inverse);
int sendData(const char *logName, const char *data);
void read_datagram(uartData *arg);
double ParseDataValue(char* datagramBuffer, char *key, int datablock);
bool decode_datagram(char* datagramBuffer, fluviusData* datagram);