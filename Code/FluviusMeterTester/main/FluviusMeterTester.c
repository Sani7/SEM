#include "FluviusMeterTester.h"

static const char *TAG = "FluviusMeterTester";

void esp_uart_init(void)
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
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_line_inverse(UART_NUM_1, UART_SIGNAL_RXD_INV | UART_SIGNAL_TXD_INV);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

int sendData(const char *logName, const char *data)
{
    const int len = strlen(data);
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
    uart_wait_tx_done(UART_NUM_1, 100);
    ESP_LOGI(logName, "Wrote %d bytes", txBytes);
    return txBytes;
}

void configure_pin(gpio_num_t pin, gpio_mode_t mode)
{
    ESP_LOGI(TAG, "Configured pin %d as %s", pin, mode < 2 ? "Input" : "Output");
    // zero-initialize the config structure.
    gpio_config_t io_conf = {};
    // disable interrupt
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    // set as output mode
    io_conf.mode = mode;
    // bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = 1ULL << pin;
    // configure GPIO with the given settings
    gpio_config(&io_conf);
    ESP_LOGI(TAG, "Test");
}

void app_main(void)
{
    gpio_set_direction(ENABLE_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(ENABLE_PIN, GPIO_PULLDOWN_ENABLE | GPIO_PULLUP_DISABLE);
    gpio_set_intr_type(ENABLE_PIN, GPIO_INTR_DISABLE);

    esp_uart_init();
    // start gpio task
    long startMillis;
    while (1)
    {
        startMillis = esp_timer_get_time();
        if (gpio_get_level(ENABLE_PIN))
        {
            ESP_LOGI(TAG, "GPIO gone high");
            sendData(TAG, "/ISk5\\2MT382-1000\n1-3:0.2.8(50)\n0-0:1.0.0(101209113020W)\n0-0:96.1.1(4B384547303034303436333935353037)\n1-0:1.8.1(123456.789*kWh)\n1-0:1.8.2(123456.789*kWh)\n1-0:2.8.1(123456.789*kWh)\n1-0:2.8.2(123456.789*kWh)\n0-0:96.14.0(0002)\n1-0:1.7.0(01.193*kW)\n1-0:2.7.0(00.000*kW)\n0-0:96.7.21(00004)\n0-0:96.7.9(00002)\n1-0:99.97.0(2)(0-0:96.7.19)(101208152415W)(0000000240*s)(101208151004W)(0000000301*s)\n1-0:32.32.0(00002)\n1-0:52.32.0(00001)\n1-0:72.32.0(00000)\n1-0:32.36.0(00000)\n1-0:52.36.0(00003)\n1-0:72.36.0(00000)\n0-0:96.13.0(303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F)\n1-0:32.7.0(220.1*V)\n1-0:52.7.0(220.2*V)\n1-0:72.7.0(220.3*V)\n1-0:31.7.0(001*A)\n1-0:51.7.0(002*A)\n1-0:71.7.0(003*A)\n1-0:21.7.0(01.111*kW)\n1-0:41.7.0(02.222*kW)\n1-0:61.7.0(03.333*kW)\n1-0:22.7.0(04.444*kW)\n1-0:42.7.0(05.555*kW)\n1-0:62.7.0(06.666*kW)\n0-1:24.1.0(003)\n0-1:96.1.0(3232323241424344313233343536373839)\n0-1:24.2.3(101209112500W)(12785.123*m3)\n!EF2F\n");
        }
        while (esp_timer_get_time() < (startMillis + 1000000))
        {
            vTaskDelay(1);
        }
    }
}
