#include "IoTFluviusMeterReader.h"

void init(bool inverse)
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
    if (inverse)
    {
        // invert rx and tx
        uart_set_line_inverse(UART_NUM_1, UART_SIGNAL_RXD_INV | UART_SIGNAL_TXD_INV);
    }
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

int sendData(const char *logName, const char *data)
{
    const int len = strlen(data);
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
    ESP_LOGI(logName, "Wrote %d bytes", txBytes);
    return txBytes;
}

// From example
static void example_tx_task(void *arg)
{
    static const char *TX_TASK_TAG = "TX_TASK";
    esp_log_level_set(TX_TASK_TAG, ESP_LOG_INFO);
    while (1)
    {
        sendData(TX_TASK_TAG, "Hello world");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

static void example_rx_task(void *arg)
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    uint8_t *data = (uint8_t *)malloc(RX_BUF_SIZE + 1);
    while (1)
    {
        const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_RATE_MS);
        if (rxBytes > 0)
        {
            data[rxBytes] = 0;
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
            ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
        }
    }
    free(data);
}

void read_datagram(uartData *arg)
{
    size_t data_len;
    static bool startDetected;
    static int readPointer;
    char incomingByte;
    char datagramBuffer[1024];
    uart_get_buffered_data_len(UART_NUM_1, &data_len);
    if (data_len > 0)
    {
        uart_read_bytes(UART_NUM_1, &incomingByte, 1, 1000 / portTICK_RATE_MS);

        // Look for the start of the datagram
        if (!startDetected && incomingByte == '/')
        {
            startDetected = true;
            ESP_LOGI("UART", "Detected start of a datagram");
        }

        // Ignore all data on serial port if start was not detected
        if (startDetected)
        {
            datagramBuffer[readPointer++] = incomingByte;

            // Send incoming bytes to the serial monitor for debugging.
            ESP_LOGI("UART", "%c", incomingByte);

            // Look for the end of the datagram
            if (incomingByte == '\n' && datagramBuffer[readPointer - 7] == '!')
            {
                ESP_LOGI("UART", "Read in full datagram");
                arg->currentState = DATAGRAM_READY;
                strncpy(datagramBuffer, arg->data, RX_BUF_SIZE);
                readPointer = 0;
                startDetected = false;
            }
            // End of datagram not found
            if (readPointer > 1024)
            {
                ESP_LOGI("UART", "Invalid Datagram > No end detected");
                arg->currentState = IDLE;
                readPointer = 0;
                startDetected = false;
            }
        }
    }
}

// Parse value of single OBIS reference
double ParseDataValue(char *datagramBuffer, char *key, int datablock)
{
    // look for the OBIS reference
    char *position = strstr(datagramBuffer, key);

    // if OBIS reference found
    if (position)
    {
        // look for the start position of the value for this OBIS reference
        char *start = position + strlen(key) + 1;

        // if there are two value blocks then look for the start of the second one
        if (datablock == 2)
        {
            char *start = strchr(start, '(') + 1;
        }

        // look for the end position of the value for this OBIS reference
        char *end = strchr(start, '*') - 1;

        // get the value of the OBIS reference
        char buffer[64] = {0};
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
        return false;
    }

    // 1-0:32.7.0 = OBIS reference for Voltage Line 1
    datagram->UL1 = ParseDataValue(datagramBuffer, "1-0:32.7.0", 1);

    // 1-0:52.7.0 = OBIS reference for Voltage Line 2
    datagram->UL2 = ParseDataValue(datagramBuffer, "1-0:52.7.0", 1);

    // 1-0:72.7.0 = OBIS reference for Voltage Line 3
    datagram->UL3 = ParseDataValue(datagramBuffer, "1-0:72.7.0", 1);

    // 1-0:31.7.0 = OBIS reference for Current Line 1
    datagram->IL1 = ParseDataValue(datagramBuffer, "1-0:31.7.0", 1);

    // 1-0:51.7.0 = OBIS reference for Current Line 2
    datagram->IL2 = ParseDataValue(datagramBuffer, "1-0:51.7.0", 1);

    // 1-0:71.7.0 = OBIS reference for Current Line 3
    datagram->IL3 = ParseDataValue(datagramBuffer, "1-0:71.7.0", 1);

    // 1-0:1.8.2 = OBIS reference for electricity delivered to client in low tariff
    datagram->CONSUMPTION_LOW_TARIF = ParseDataValue(datagramBuffer, "1-0:1.8.2", 1);

    // 1-0:1.8.1 = OBIS reference for electricity delivered to client in high tariff
    datagram->CONSUMPTION_HIGH_TARIF = ParseDataValue(datagramBuffer, "1-0:1.8.1", 1);

    // 1-0:2.8.2 = OBIS reference for electricity delivered by client in low tariff
    datagram->PRODUCTION_LOW_TARIF = ParseDataValue(datagramBuffer, "1-0:2.8.2", 1);

    // 1-0:2.8.1 = OBIS reference for electricity delivered by client in high tariff
    datagram->PRODUCTION_HIGH_TARIF = ParseDataValue(datagramBuffer, "1-0:2.8.1", 1);

    // 1-0:1.7.0 = OBIS reference actual total power delivered to client
    datagram->TOTAL_POWER_CONSUMPTION = ParseDataValue(datagramBuffer, "1-0:1.7.0", 1);

    // 1-0:2.7.0 = OBIS reference actual total power delivered by client
    datagram->TOTAL_POWER_PRODUCTION = ParseDataValue(datagramBuffer, "1-0:2.7.0", 1);

    // 0-1:24.2.3 = OBIS reference gas delivered to client with temperature correction , 0-n where the n is the device number, possibly you need to change this number for your configuration
    datagram->GAS_METER_M3 = ParseDataValue(datagramBuffer, "0-1:24.2.3", 2);

    // 0-2:24.2.1 = OBIS reference water delivered to client, 0-n where the n is the device number, possibly you need to change this number for your configuration
    datagram->WATER_METER_M3 = ParseDataValue(datagramBuffer, "0-2:24.2.1", 2);
    return true;
}