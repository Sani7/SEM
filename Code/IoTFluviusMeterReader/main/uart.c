#include "IoTFluviusMeterReader.h"

/**
 * @brief initialize uart1
 * 
 */
void uart_init(gpio_num_t tx_pin, gpio_num_t rx_pin, uint32_t baud, bool inv)
{
    const uart_config_t uart_config = {
        .baud_rate = baud,
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
    if (inv)
    {
        uart_set_line_inverse(UART_NUM_1, UART_SIGNAL_RXD_INV);
        ESP_LOGI("UART", "Set line inverse to true");
    }
    uart_set_pin(UART_NUM_1, tx_pin, rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

/**
 * @brief send data over uart1
 * 
 * @param logName Tag name for loging
 * @param data data to write to the uart
 * @return int the amount of bytes written
 */
int sendData(const char *logName, const char *data)
{
    const int len = strlen(data);
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
    ESP_LOGI(logName, "Wrote %d bytes", txBytes);
    return txBytes;
}

/**
 * @brief read the datagram of the uart bus and copy it to the data variable
 * 
 * @param data the buffer to write the data to
 * @param len the sizeof the buffer
 * @return true not able to read a valid datagram
 * @return false was able to read a valid datagram
 */
bool read_datagram(char *data, int len)
{
    bool ret = true;
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
            ESP_LOGI("UART", "Detected start of a datagram\n");
        }

        if (startDetected)
        {
            data[readPointer++] = incomingByte[0];
            // Look for the end of the datagram
            if (data[readPointer - 1] == '\r' && data[readPointer - 6] == '!')
            {
                data[readPointer - 1] = 0;
                ESP_LOGI("UART", "Read in full datagram\n");
                readPointer = 0;
                startDetected = false;
                fullDatagram = true;
                ret = false;
                break;
            }
            // End of datagram not found
            if (readPointer == len - 1)
            {
                ESP_LOGI("UART", "Invalid Datagram > No end detected\n");
                readPointer = 0;
                startDetected = false;
                ret = true;
                memset(data, 0, len);
                break;
            }
        }
        uart_read_bytes(UART_NUM_1, incomingByte, 1, 100);
    }
    uart_flush(UART_NUM_1);
    return ret;
}
/**
 * @brief Convert data from UART to doubles
 * 
 * @param datagramBuffer the data from the UART
 * @param key what OBIS referece
 * @param datablock which datablock 
 * @return double value
 */
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

/**
 * @brief decode datagram with obis references
 * 
 * @param datagramBuffer data from UART
 * @param datagram The place to write the doubles to
 * @return true no end or beginning found
 * @return false decoded succesfully
 */
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