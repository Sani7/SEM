#include "IoTFluviusMeterReader.h"

void uart_init(void)
{
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    // We won't use a buffer for sending data.
    // invert rx and tx
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_line_inverse(UART_NUM_1, UART_SIGNAL_RXD_INV);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

int sendData(const char *logName, const char *data)
{
    const int len = strlen(data);
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
    ESP_LOGI(logName, "Wrote %d bytes", txBytes);
    return txBytes;
}

bool read_datagram(char *data)
{
    bool ret = true;
    size_t data_len;
    bool startDetected = false;
    bool fullDatagram = false;
    int readPointer = 0;
    char incomingByte[1];

    uart_read_bytes(UART_NUM_1, incomingByte, 1, 10000);

    while (fullDatagram != 1)
    {
        if (!startDetected && incomingByte[0] == '/')
        {
            startDetected = true;
            printf("UART Detected start of a datagram\n");
        }

        if (startDetected)
        {
            data[readPointer++] = incomingByte[0];
            // Look for the end of the datagram
            if (data[readPointer - 1] == '\n' && data[readPointer - 6] == '!')
            {
                data[readPointer - 1] = 0;
                printf("UART Read in full datagram\n");
                readPointer = 0;
                startDetected = false;
                fullDatagram = true;
                ret = false;
                break;
            }
            // End of datagram not found
            if (readPointer > 2048)
            {
                printf("UART Invalid Datagram > No end detected\n");
                readPointer = 0;
                startDetected = false;
                ret = true;
            }
        }
        uart_read_bytes(UART_NUM_1, incomingByte, 1, 100);
        ESP_ERROR_CHECK(uart_get_buffered_data_len(UART_NUM_1, &data_len));
    }
    ESP_ERROR_CHECK(uart_get_buffered_data_len(UART_NUM_1, &data_len));
    ESP_LOGI("DEBUG", "%d", data_len);
    uart_flush(UART_NUM_1);
    return ret;
}

// Parse value of single OBIS reference
double ParseDataValue(char *datagramBuffer, char *key, int datablock)
{
    // look for the OBIS reference
    char *position = strstr(datagramBuffer, key);
    char *start;
    char *end;
    char buffer[64] = {0};
    
    // if OBIS reference found
    if (position)
    {
        // look for the start position of the value for this OBIS reference
        start = position + strlen(key) + 1;

        // if there are two value blocks then look for the start of the second one
        if (datablock == 2)
        {
            start = strchr(start, '(') + 1;
        }

        // look for the end position of the value for this OBIS reference
        end = strchr(start, '*') - 1;

        // get the value of the OBIS reference
        strncpy(buffer, start, end - start + 1);

        // return the value
        return atof(buffer);
    }
    else
    {
        // return invalid if OBIS reference not found
        return -1;
    }
}

// Decode the datagram
bool decode_datagram(char *datagramBuffer, fluviusData *datagram)
{
    // Find the start of the datagram
    char *startChar = strstr(datagramBuffer, "/");

    // Find the end of the datagram
    char *endChar = strstr(datagramBuffer, "!");

    // If the end or start are not found we have a invalid datagram
    if (!startChar || !endChar)
    {
        ESP_LOGI("Decode datagram", "Invalid datagram");
        return true;
    }

    // 1-0:32.7.0 = OBIS reference for Voltage Line 1
    datagram->UL1 = ParseDataValue(datagramBuffer, "1-0:32.7.0", 1);
    ESP_LOGI(TAG, "UL1 = %f", datagram->UL1);

    // 1-0:52.7.0 = OBIS reference for Voltage Line 2
    datagram->UL2 = ParseDataValue(datagramBuffer, "1-0:52.7.0", 1);
    ESP_LOGI(TAG, "UL2 = %f", datagram->UL2);

    // 1-0:72.7.0 = OBIS reference for Voltage Line 3
    datagram->UL3 = ParseDataValue(datagramBuffer, "1-0:72.7.0", 1);
    ESP_LOGI(TAG, "UL3 = %f", datagram->UL3);

    // 1-0:31.7.0 = OBIS reference for Current Line 1
    datagram->IL1 = ParseDataValue(datagramBuffer, "1-0:31.7.0", 1);
    ESP_LOGI(TAG, "IL1 = %f", datagram->IL1);

    // 1-0:51.7.0 = OBIS reference for Current Line 2
    datagram->IL2 = ParseDataValue(datagramBuffer, "1-0:51.7.0", 1);
    ESP_LOGI(TAG, "IL2 = %f", datagram->IL2);

    // 1-0:71.7.0 = OBIS reference for Current Line 3
    datagram->IL3 = ParseDataValue(datagramBuffer, "1-0:71.7.0", 1);
    ESP_LOGI(TAG, "IL3 = %f", datagram->IL3);

    // 1-0:1.8.2 = OBIS reference for electricity delivered to client in low tariff
    datagram->CONSUMPTION_LOW_TARIF = ParseDataValue(datagramBuffer, "1-0:1.8.2", 1);
    ESP_LOGI(TAG, "CLT = %f", datagram->CONSUMPTION_LOW_TARIF);

    // 1-0:1.8.1 = OBIS reference for electricity delivered to client in high tariff
    datagram->CONSUMPTION_HIGH_TARIF = ParseDataValue(datagramBuffer, "1-0:1.8.1", 1);
    ESP_LOGI(TAG, "CHT = %f", datagram->CONSUMPTION_HIGH_TARIF);

    // 1-0:2.8.2 = OBIS reference for electricity delivered by client in low tariff
    datagram->PRODUCTION_LOW_TARIF = ParseDataValue(datagramBuffer, "1-0:2.8.2", 1);
    ESP_LOGI(TAG, "PLT = %f", datagram->PRODUCTION_LOW_TARIF);

    // 1-0:2.8.1 = OBIS reference for electricity delivered by client in high tariff
    datagram->PRODUCTION_HIGH_TARIF = ParseDataValue(datagramBuffer, "1-0:2.8.1", 1);
    ESP_LOGI(TAG, "PHT = %f", datagram->PRODUCTION_HIGH_TARIF);

    // 1-0:1.7.0 = OBIS reference actual total power delivered to client
    datagram->TOTAL_POWER_CONSUMPTION = ParseDataValue(datagramBuffer, "1-0:1.7.0", 1);
    ESP_LOGI(TAG, "TPC = %f", datagram->TOTAL_POWER_CONSUMPTION);

    // 1-0:2.7.0 = OBIS reference actual total power delivered by client
    datagram->TOTAL_POWER_PRODUCTION = ParseDataValue(datagramBuffer, "1-0:2.7.0", 1);
    ESP_LOGI(TAG, "TPP = %f", datagram->TOTAL_POWER_PRODUCTION);

    // 0-1:24.2.3 = OBIS reference gas delivered to client with temperature correction , 0-n where the n is the device number, possibly you need to change this number for your configuration
    datagram->GAS_METER_M3 = ParseDataValue(datagramBuffer, "0-1:24.2.3", 2);
    ESP_LOGI(TAG, "GM = %f", datagram->GAS_METER_M3);

    // 0-2:24.2.3 = OBIS reference water delivered to client, 0-n where the n is the device number, possibly you need to change this number for your configuration
    datagram->WATER_METER_M3 = ParseDataValue(datagramBuffer, "0-2:24.2.3", 2);
    ESP_LOGI(TAG, "WM = %f", datagram->WATER_METER_M3);

    return false;
}